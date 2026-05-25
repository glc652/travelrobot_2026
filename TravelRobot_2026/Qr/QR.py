# # This work is licensed under the MIT license.
# # Copyright (c) 2013-2023 OpenMV LLC. All rights reserved.
# # https://github.com/openmv/openmv/blob/master/LICENSE
# #
# # LCD Example
# #
# # Note: To run this example you will need a LCD Shield for your OpenMV Cam.
# #
# # The LCD Shield allows you to view your OpenMV Cam's frame buffer on the go.

# import sensor
# import display

# sensor.reset()  # Initialize the camera sensor.
# sensor.set_pixformat(sensor.RGB565)  # or sensor.GRAYSCALE
# sensor.set_framesize(sensor.QQVGA2)  # Special 128x160 framesize for LCD Shield.
# # Initialize the lcd screen.
# # Note: A DAC or a PWM backlight controller can be used to control the
# # backlight intensity if supported:
# #  lcd = display.SPIDisplay(backlight=display.DACBacklight(channel=2))
# #  lcd.backlight(25) # 25% intensity
# # Otherwise the default GPIO (on/off) controller is used.
# lcd = display.SPIDisplay()

# while True:
#     lcd.write(sensor.snapshot())  # Take a picture and display the image.

import sensor, image, ustruct,machine
import display
from machine import Pin

# ---------------------- 硬件配置 ----------------------
# LCD屏分辨率：128×160
LCD_WIDTH = 128
LCD_HEIGHT = 160
# 摄像头采集分辨率（降低分辨率让远距离二维码占比更大）
CAM_WIDTH = 320
CAM_HEIGHT = 240

# 计算中间区域的ROI（x, y, w, h）
ROI_X = (CAM_WIDTH - LCD_WIDTH) // 2
ROI_Y = (CAM_HEIGHT - LCD_HEIGHT) // 2
LCD_ROI = (ROI_X, ROI_Y, LCD_WIDTH, LCD_HEIGHT)

# 串口配置
uart = machine.UART(3, baudrate=115200, timeout_char=0)
# uart = machine.UART(1, baudrate=115200, timeout_char=100)

# LCD初始化（128×160）
lcd = display.SPIDisplay(width=LCD_WIDTH, height=LCD_HEIGHT)
lcd.clear()

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
        #print("串口未初始化，跳过发送")
        return
    if payload_int < 0 or payload_int > MAX_3DIGIT:
        #print(f"三位数字超出范围：{payload_int}，强制设为0")
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
检测控制指令
0x01: 开始识别并发送
0xBB: 停止识别和发送
"""
def check_control_cmd():
    rx_byte = uart.read(1)
    if rx_byte is None:
        return None
        #return 'start'
    if rx_byte == b'\x01':
        print("\n收到开始指令，开始识别二维码...")
        return 'start'
    elif rx_byte == b'\xBB':
        print("\n收到停止指令，停止识别和发送")
        return 'stop'
    return None

"""
裁剪图像中间区域并适配LCD显示
:param img: 原始320×240图像
:return: 中间128×160的裁剪图像
"""
def crop_middle_for_lcd(img):
    # 裁剪中间128×160区域（ROI：x, y, w, h）
    middle_img = img.copy(roi=LCD_ROI)
    return middle_img

# ---------------------- 主循环----------------------
if __name__ == "__main__":
    init_camera()
    last_payload = None
    recognizing = False  # 识别状态标志

    while True:
        # 1. 采集320×240高分辨率图像（保证二维码识别精度）
        img_original = sensor.snapshot()

        # 2. 检测控制指令（0x01开始，0xBB停止）
        cmd = check_control_cmd()
        if cmd == 'start':
            recognizing = True
            last_payload = None
        elif cmd == 'stop':
            recognizing = False
            last_payload = None

        # 3. 如果处于识别状态，持续识别并发送数据
        if recognizing:
            qr_codes = img_original.find_qrcodes()
            if qr_codes:
                qr = qr_codes[0]
                # 在原始高分辨率图像上绘制矩形框（位置精准）
                img_original.draw_rectangle(qr.rect(), color=0xF800, thickness=3)
                current_payload = qr.payload().strip()

                if current_payload.isdigit() and len(current_payload) == 3:
                    payload_int = int(current_payload)
                    # 只在识别到新的二维码时发送
                    if current_payload != last_payload:
                        last_payload = current_payload
                        send_qr_data(payload_int)
            else:
                # 没有识别到二维码时清空last_payload
                last_payload = None

        # 4. 裁剪中间128×160区域给LCD显示
        img_lcd = crop_middle_for_lcd(img_original)
        lcd.write(img_lcd)

        # OpenMV IDE仍显示完整320×240画面，方便调试
