from maix import camera, display, image, app, time, uart, pinmap
import math, struct

# ---------------- 配置区域 (仅修改此处参数以更新逻辑) ----------------

# LAB 色域阈值 - 黑白 - 绿红 - 蓝黄 [L_min, L_max, A_min, A_max, B_min, B_max]
RED_THRESH   = [[0, 100, 10, 80, -5, 80]]
BLUE_THRESH  = [[0, 100, -20, 40, -80, -10]]
GREEN_THRESH = [[0, 100, -55, -5, 0, 45]]

COLOR_THRESHOLDS = {
    "Blue_T": BLUE_THRESH,
    "Green_T": GREEN_THRESH,
    "Red_T": RED_THRESH,
}

distance_table = [
    (155, 0.0),
    (150, 1.0),
    (145, 2.0),  # X
    (140, 2.7),  # 3.0
    (130, 4.8),  # 5.5
    (120, 6.0),  # 8.0
    (110, 8.0),  # 10.0
    (100, 10.0), # 12.5
    (90, 12.5),  # 15.0
    (80, 14.7),  # 16.5
    (70, 17.2),  # 18.0
    (65, 18.6),  # 19.5
    (60, 21.0),  # 22.0
    (55, 21.8),  # 23.0
    (50, 23.8),  # 24.2
    (45, 26.5)   # 25.0
]

IMG_WIDTH  = 480  # 图像宽度
IMG_HEIGHT = 200  # 图像高度

H_RELATIVE     = 21.5  # 摄像头高度 X 
L_CLAW_OFFSET  = 28.0  # 摄像头 - 爪子 投影距离 X 
INSTALL_PITCH = 30.0  # 摄像头倾角 X

HFOV = 86.0  # 水平视场角
VFOV = 41.8  # 垂直视场角 X

CX_REF = 260  # 爪子中心
CY_REF = 100  # 水平中心 X 

AREA_MAX = 4000  # 面积限幅
AREA_MIN = 500   # 面积限幅
ASPECT_MAX = 1.0 # 长宽比限幅
ASPECT_MIN = 0.4 # 长宽比限幅

# 串口初始化
pinmap.set_pin_function("A18", "UART1_RX")
pinmap.set_pin_function("A19", "UART1_TX")
serial = uart.UART("/dev/ttyS1", 115200)

cam = camera.Camera(IMG_WIDTH, IMG_HEIGHT)
disp = display.Display()

target_map = {0x01: "Red_T", 0x02: "Blue_T", 0x03: "Green_T"}
DRAW_COLORS = {"Red_T": image.COLOR_RED, "Blue_T": image.COLOR_BLUE, "Green_T": image.COLOR_GREEN}

active_cmd = None
rx_state = 0
last_sent_str = "None"
last_send_time = 0
fps = 0
prev_time = time.ticks_ms()

# ------------------------------------------------------------------

# 获取距离
def get_dist_by_lut(cy):

    # 上下限限幅
    if cy >= distance_table[0][0]: return distance_table[0][1]
    if cy <= distance_table[-1][0]: return distance_table[-1][1]
    
    # 线性映射
    for i in range(len(distance_table) - 1):
        y_high, d_near = distance_table[i]
        y_low, d_far = distance_table[i+1]
        
        if y_high >= cy > y_low:
            dist = d_near + (cy - y_high) * (d_far - d_near) / (y_low - y_high)
            return round(dist, 2)

    # 识别失败兜底
    return None

# 遍历图像或许最小色块并进行密度筛查
def Find_all_blobs(img, thresholds, label, density_min=0.4):
    get_blobs = img.find_blobs(thresholds, pixels_threshold=30, area_threshold=30, merge=False)
    out_blobs = []
    for i in get_blobs:
        density = i.pixels() / (i.w() * i.h())
        if density > density_min:
            out_blobs.append(i)
    return out_blobs

def detect_all_targets(img):
    current_frame_results = {}
    for label, thresholds in COLOR_THRESHOLDS.items():

        # 密度筛选 - 获取纯净小色块
        density_gate = 0.60 if label == "Green_T" else 0.50
        blobs = Find_all_blobs(img, thresholds, label, density_min=density_gate)
        
        # 2. 三维限幅：长宽比、面积、宽度限制
        valid_blobs = [b for b in blobs if (ASPECT_MIN <= b.w() / b.h() <= ASPECT_MAX) and (AREA_MIN < b.pixels() < AREA_MAX) and (b.w() < 120)]
        
        if valid_blobs:

            # 取最大的色块
            main_b = max(valid_blobs, key=lambda b: b.pixels())
            
            c = DRAW_COLORS.get(label, image.COLOR_WHITE)
            cx, cy = main_b.cx(), main_b.cy()
            
            # 角度计算
            h_angle = ((cx - CX_REF) / IMG_WIDTH) * HFOV

            # 距离计算 + 崩溃保护
            dist = get_dist_by_lut(cy)
            dist_str = f"{dist:.1f}cm" if dist is not None else "dist: None"
            
            # --- 宝物识别绘制 - 识别框 | 中心点 | 密度 ---
            img.draw_rect(*main_b.rect(), color=c, thickness=3)
            img.draw_circle(cx, cy, 3, color=image.COLOR_ORANGE, thickness=-1) 
            img.draw_string(main_b.x(), main_b.y()-15, f"D:{main_b.pixels()/(main_b.w()*main_b.h()):.2f}", color=c, scale=1.0)
            img.draw_string(main_b.x(), main_b.y()+5, dist_str, color=image.COLOR_WHITE, scale=1.0)

            current_frame_results[label] = {"h": h_angle, "d": dist, "cx": cx, "cy": cy}

    return current_frame_results

#  串口接收
def send_custom_package(cmd, status_val, value):

    global last_sent_str

    try:
        if cmd == 0x06:
            val_to_send = int(value)
            data_bytes = struct.pack('<i', val_to_send)
            v_str = f"{val_to_send}"
        else:
            val_to_send = float(value)
            data_bytes = struct.pack('<f', val_to_send)
            v_str = f"{val_to_send:.2f}"
        
        last_sent_str = f"ID:{hex(cmd)} S:{hex(status_val)} V:{v_str}"
        package = b'\xAA' + bytes([cmd]) + bytes([status_val]) + data_bytes + b'\xBB'
        serial.write(package)
        
    # 报错处理 - 防止程序卡死
    except:
        last_sent_str = f"Wait: {hex(cmd)}"

# 主循环
def main():

    global active_cmd, rx_state, last_send_time, fps, prev_time
    
    while not app.need_exit():

        # 计算帧率
        curr_time = time.ticks_ms()
        dt = curr_time - prev_time
        if dt > 0: fps = 1000 / dt
        prev_time = curr_time

        # 接收指令
        rx_data = serial.read()

        # 通讯 (0x0A + 指令)
        if rx_data:
            for byte in rx_data:

                if rx_state == 0 and byte == 0x0A: rx_state = 1

                elif rx_state == 1:
                    if (0x01 <= byte <= 0x03) or byte == 0x06 or byte in [0x14, 0x24, 0x34]: active_cmd = byte
                    elif byte == 0x05: active_cmd = None
                    rx_state = 0

        # 3. 图像处理与识别
        img = cam.read()
        results = detect_all_targets(img)
        
        # 串口通讯 - 限速 10 ms
        if active_cmd and (time.ticks_ms() - last_send_time > 10):

            # 角度识别
            if 0x01 <= active_cmd <= 0x03:

                # 指令转化名称
                name = target_map.get(active_cmd)

                # 寻找数据中对应宝物讯息
                res = results.get(name)

                if res:
                    send_custom_package(active_cmd, 0x01, res["h"])
                else:
                    send_custom_package(active_cmd, 0x00, 0.0)
            
            # 距离识别
            elif active_cmd in [0x14, 0x24, 0x34]:

                # 指令转化名称
                if active_cmd == 0x14: target_label = "Red_T"
                elif active_cmd == 0x24: target_label = "Blue_T"
                else: target_label = "Green_T"

                # 寻找数据中对应宝物讯息
                res = results.get(target_label)

                if res and res["d"] is not None:
                    send_custom_package(active_cmd, 0x01, res["d"])
                else:
                    send_custom_package(active_cmd, 0x00, 0.0)

            # 获取正确宝物颜色
            elif active_cmd == 0x06:

                # 保证有且仅有一个宝物
                if len(results) == 1:
                    
                    # 获取对应宝物名称
                    color_name = list(results.keys())[0]

                    # 名称转化指令
                    c_id = {"Red_T": 1, "Blue_T": 2, "Green_T": 3}.get(color_name, 0)

                    send_custom_package(0x06, 0x01, c_id)
                else:
                    send_custom_package(0x06, 0x00, 0)

            last_send_time = time.ticks_ms()

        # 参考线显示 - 摄像头中线(橙) - 爪子中线(黄)
        img.draw_line(240, 0, 240, 200, color=image.COLOR_ORANGE, thickness=2)
        img.draw_line(260, 0, 260, 200, color=image.COLOR_YELLOW, thickness=2)

        # 信息显示 - 发送内容 - 接收指令 - 帧率
        img.draw_string(10, 10, f"SENT: {last_sent_str}", color=image.COLOR_PURPLE, scale=1.2)
        img.draw_string(10, 30, f"TASK: {hex(active_cmd) if active_cmd else 'IDLE'}", color=image.COLOR_RED, scale=1.2)
        img.draw_string(10, 50, f"FPS : {fps:.1f}", color=image.COLOR_WHITE, scale=1.2)

        # 图片显示
        disp.show(img)

if __name__ == "__main__":
    main()