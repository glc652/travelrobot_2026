from pyb import LED
import os
import sensor, image, time

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA) # we run out of memory if the resolution is much bigger...
sensor.set_brightness(2000) # 设置图像亮度 越大越亮
sensor.skip_frames(time = 20)
sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
sensor.set_auto_whitebal(True)  # must turn this off to prevent image washout...
clock = time.clock()

#SAVE_FLAG为真进行图像保存
#SAVE_FLAG为假进行图像读取
SAVE_FLAG = False #True #False

while(True):
    img = sensor.snapshot()

    #测试时先保存再读取
    if SAVE_FLAG:
        #设置保存路径，注意需要加/sd/
        image_pat = "/sd/test_save" + ".bmp"
        #保存图片，如果路径存在则不会保存成功
        img.save(image_pat,quality=100)
    else:
        #读取图片，只能读取bmp格式
        img1 = image.Image("/sd/test_save.bmp")
        print(img1)
        #将图片放到图像的右下角
        #参数1：图片变量
        #参数2：起始x位置
        #参数3：起始y位置
        #参数4：x方向缩放
        #参数5：y方向缩放
        img.draw_image(img1, int(img.width()/2), int(img.height()/2), x_scale=0.5, y_scale=0.5)

