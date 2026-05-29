from maix import camera, display, image, nn, app, time, uart, pinmap
import math
import struct

# ---------------- 配置区域 (仅修改此处参数以更新逻辑) ----------------

pinmap.set_pin_function("A18", "UART1_RX")
pinmap.set_pin_function("A19", "UART1_TX")
serial = uart.UART("/dev/ttyS1", 115200)

IMG_WIDTH  = 480
IMG_HEIGHT = 200
HFOV = 86.0

distance_table = [
    (155, 0.0),
    (150, 1.0),
    (140, 3.0),
    (130, 5.5),
    (120, 8.0),
    (110, 10.0),
    (100, 12.5),
    (90, 15.0), 
    (80, 16.5),
    (70, 18.0),
    (65, 19.5),
    (60, 22.0),
    (55, 23.0),
    (50, 24.2),
    (45, 25.0)
]

smoothed_coords = {
    "Red_T": [0, 0],
    "Blue_T": [0, 0],
    "Green_T": [0, 0]
}

target_map = {
    0x01: "Red_T",
    0x02: "Blue_T",
    0x03: "Green_T"
}

color_map = {
    "Red_T": image.COLOR_RED,
    "Blue_T": image.COLOR_BLUE,
    "Green_T": image.COLOR_GREEN
}

# 通讯变量初始化
active_cmd = None  
last_send_time = 0
rx_state = 0 

# 其他初始化
last_sent_str = "None"
fps = 0
last_time = time.ticks_ms()

#detector = nn.YOLOv5(model="/root/model-270953.maixcam/model_270953.mud", dual_buff=False)
detector = nn.YOLOv5(model="/root/model-274974.maixcam/model_274974.mud", dual_buff=False)
cam = camera.Camera(IMG_WIDTH, IMG_HEIGHT, detector.input_format())
disp = display.Display()

# ------------------------------------------------------------------

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

# 主循环
while not app.need_exit():

    # 接收指令
    rx_data = serial.read() 

    # 通讯 (0x0A + 指令)
    if rx_data:
        for byte in rx_data:

            if rx_state == 0 and byte == 0x0A: rx_state = 1

            elif rx_state == 1:
                if 0x01 <= byte <= 0x03 or byte == 0x06 or byte in [0x14, 0x24, 0x34]: active_cmd = byte
                elif byte == 0x05: active_cmd = None
                rx_state = 0

    # 图像获取
    img = cam.read()

    # YOLO识别 - 置信度阈值 - 重叠框阈值
    objs = detector.detect(img, conf_th=0.2, iou_th=0.2)
    
    current_frame_results = {}

    for obj in objs:

        # 获取目标名称
        label = detector.labels[obj.class_id]

        # 获取目标置信度
        score = obj.score  
        
        # 获取中心点坐标
        cx = int(obj.x + obj.w / 2)
        cy = int(obj.y + obj.h / 2)

        # 角度计算
        h_angle = ((cx - 260) / 480) * HFOV

        # 距离计算 + 崩溃保护
        dist = get_dist_by_lut(cy)
        dist_show = f"D:{dist:.1f}cm" if dist is not None else "dist: None"

        # YOLO框
        rect_color = color_map.get(label, image.COLOR_WHITE)
        img.draw_rect(obj.x, obj.y, obj.w, obj.h, color=rect_color, thickness=2)

        # 白色中心点 - Y坐标数值 - 测量距离 - 宝物置信度
        img.draw_circle(cx, cy, 3, color=image.COLOR_ORANGE, thickness=-1) 
        img.draw_string(obj.x, obj.y + 5,  dist_show, color=image.COLOR_YELLOW, scale=1.0)
        img.draw_string(obj.x, obj.y + 20,  f"A:{h_angle:.2f}", color=image.COLOR_YELLOW, scale=1.0)
        img.draw_string(obj.x, obj.y + obj.h - 30, f"Y:{cy}", color=image.COLOR_YELLOW, scale=1.0)
        img.draw_string(obj.x, obj.y + obj.h - 15, f"$:{score*100:.1f}%", color=image.COLOR_YELLOW, scale=1.0)

        # 数据存储 : 水平倾角 - 物理距离 - 中心点像素坐标
        current_frame_results[label] = {"h": h_angle, "d": dist, "cx": cx, "cy": cy, "score": score}

    curr_ms = time.ticks_ms()

    # 串口通讯 - 限速 10 ms
    if active_cmd and (curr_ms - last_send_time >= 10):

        # 角度识别
        if 0x01 <= active_cmd <= 0x03:

            # 指令转化名称
            color_name = target_map.get(active_cmd)
            
            # 寻找数据中对应宝物讯息
            res = current_frame_results.get(color_name)

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
            res = current_frame_results.get(target_label)

            if res and res["d"] is not None:
                send_custom_package(active_cmd, 0x01, res["d"])
            else:
                send_custom_package(active_cmd, 0x00, 0.0)

        # 获取正确宝物颜色
        elif active_cmd == 0x06:

            # 保证有且仅有一个宝物
            if len(current_frame_results) == 1:

                # 获取对应宝物名称
                color_name = list(current_frame_results.keys())[0]
                
                # 名称转化指令
                if   color_name  == "Red_T"   :  c_id = 1
                elif color_name  == "Blue_T"  :  c_id = 2
                elif color_name  == "Green_T" :  c_id = 3
                else:                            c_id = 0

                send_custom_package(0x06, 0x01, c_id)
            else:
                send_custom_package(0x06, 0x00, 0)

        last_send_time = curr_ms

    # 帧率计算
    current_time = time.ticks_ms()
    fps = 1000 / (current_time - last_time) if (current_time - last_time) > 0 else 0
    last_time = current_time

    # 参考线显示 - 摄像头中线(黄) - 爪子中线(橙)
    #img.draw_line(240, 0, 240, 200, color=image.COLOR_YELLOW, thickness=2)
    #img.draw_line(260, 0, 260, 200, color=image.COLOR_ORANGE, thickness=2)

    # 信息显示 - 发送内容 - 接收指令 - 帧率
    #img.draw_string(10, 10, f"SENT: {last_sent_str}", color=image.COLOR_PURPLE, scale=1.2)
    #img.draw_string(10, 30, f"TASK: {hex(active_cmd) if active_cmd else 'IDLE'}", color=image.COLOR_RED, scale=1.2)
    #img.draw_string(10, 50, f"FPS: {fps:.1f}", color=image.COLOR_WHITE, scale=1.2)
    
    # 图片显示
    disp.show(img)