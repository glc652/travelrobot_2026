import sensor
import image
import ustruct
from machine import Pin, UART

# ---------------------- 硬件配置 ----------------------
# 摄像头采集分辨率（降低分辨率让远距离二维码占比更大）
CAM_WIDTH = 320
CAM_HEIGHT = 240

# 串口配置
uart = UART(1, baudrate=115200, timeout_char=1000)

# ---------------------- 通信格式约定----------------------
FRAME_HEAD = 0xA3
FRAME_FUNC = 0x12
FRAME_TAIL = 0xB3
MAX_3DIGIT = 999
TRIGGER_CMD = b'\x01'

"""
摄像头初始化（高分辨率：320×240）
"""
def init_camera():
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)  # 320×240高分辨率，保证二维码识别精度
    sensor.skip_frames(time=300)
    sensor.set_auto_exposure(True)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(False)
    print(f"摄像头初始化完成（采集分辨率：{CAM_WIDTH}×{CAM_HEIGHT}）")

"""
整数转2字节小端序
"""
def int_to_2bytes(num):
    num = max(0, min(num, 65535))
    return bytes([num & 0xFF, (num >> 8) & 0xFF])

"""
打包并发送二维码数据
"""
def send_qr_data(payload_int):
    if uart is None:
        print("串口未初始化，跳过发送")
        return
    if payload_int < 0 or payload_int > MAX_3DIGIT:
        print(f"三位数字超出范围：{payload_int}，强制设为0")
        payload_int = 0
    data_pack = (
        bytes([FRAME_HEAD]) +
        bytes([FRAME_FUNC]) +
        int_to_2bytes(payload_int) +
        bytes([FRAME_TAIL])
    )
    uart.write(data_pack)
    print(f"发送数据：{data_pack}")

"""
检测触发指令
"""
def check_trigger_cmd():
    rx_byte = uart.read(1)
    if rx_byte == TRIGGER_CMD:
        print("\n收到触发指令，开始识别二维码...")
        return True
    return False

# ---------------------- 主循环----------------------
if __name__ == "__main__":
    init_camera()
    last_payload = None
    payload_int = None
    while True:
        # 1. 采集320×240高分辨率图像（保证二维码识别精度）
        img_original = sensor.snapshot()

        # 2. 检测触发指令并处理二维码（高分辨率下识别更准）
        # if check_trigger_cmd():
        qr_codes = img_original.find_qrcodes()
        if payload_int != None:
            send_qr_data(payload_int)
        if qr_codes:
            qr = qr_codes[0]
            # 在原始高分辨率图像上绘制矩形框（位置精准）
            img_original.draw_rectangle(qr.rect(), color=0xF800, thickness=3)
            current_payload = qr.payload().strip()

            if current_payload.isdigit() and len(current_payload) == 3:
                payload_int = int(current_payload)
                # 只在识别到新的二维码时打印，之后持续发送
                if current_payload != last_payload:
                    print(f"识别到新二维码：{payload_int}")
                    last_payload = current_payload
            elif current_payload.isdigit() and len(current_payload) != 3:
                print(f"非三位数字二维码：{current_payload}")
        else:
            # 没有识别到二维码时清空last_payload
            last_payload = None

        # OpenArt IDE仍显示完整320×240画面，方便调试
