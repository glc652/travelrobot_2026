# 导入需要的库
import seekfree, pyb
import sensor, image, time, math
import os, tf
from pyb import LED
from machine import UART

# 初始化屏幕
lcd = seekfree.LCD180(3)

# 初始化摄像头
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_brightness(2000)
sensor.skip_frames(time = 20)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(True,(0,0,0))

# 打开LED灯
LED(4).on()

while(True):
    # 截取当前图像
    img = sensor.snapshot()
    # 在图像中搜索矩形
    for r in img.find_rects(threshold = 3000):
        # 判断矩形是否满足要求
        if r.w() > 120 and r.w() < 180 and r.h() > 70 and r.h() < 130:
            # 绘制矩形外框，便于在IDE上查看识别到的矩形位置
            img.draw_rectangle(r.rect(), color = (255, 0, 0))
            # 将矩形属性打印出来
            print("w:" + str(r.w()) + "  h:" + str(r.h()))

            #在矩形中搜索圆形
            for c in img.find_circles(roi = r.rect(), threshold = 1800, x_margin = 5, y_margin = 5, r_margin = 5,r_min = 1, r_max = 5, r_step = 1):
                # 绘制圆形外框
                img.draw_circle(c.x(), c.y(), c.r(), color = (255, 0, 0))
                # 计算实际X坐标点
                posx = (float(c.x()) - float(r.x())) / float(r.w()) * 35 + 1
                # 计算实际Y坐标点
                posy = 25 - ((float(c.y()) - float(r.y())) / float(r.h()) * 25) + 1
                # 将坐标点绘制到图像上
                img.draw_string(c.x() - 5, c.y() - 10, "(" + str(int(posx)) + "," + str(int(posy)) + ")", color = (0, 0, 255), scale = 1, mono_space = False)
        # 判断矩形是否符合显示区域
        if r.x() > 2 and r.x() < 120 and r.y() > 4 and r.y() < 50:
            print("1w:" + str(r.w()) + "  h:" + str(r.h()))
            # 复制矩形的图像
            img2 = img.copy(roi = [r.x()-1, r.y()-3, 160, 120])
            # 将图像显示到显示屏上
            lcd.show_image(img2, 160, 120, 0, 0, zoom=0)
