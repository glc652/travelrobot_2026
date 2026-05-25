import os
import sensor, image, time, mjpeg

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA) # we run out of memory if the resolution is much bigger...
sensor.set_brightness(2000) # 设置图像亮度 越大越亮
sensor.skip_frames(time = 20)
sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
sensor.set_auto_whitebal(True)  # must turn this off to prevent image washout...
clock = time.clock()

#视频文件地址
m = mjpeg.Mjpeg("/sd/example.mjpeg")
#记录视频有多少帧
fps_count = 0;
while(True):
    #拍摄图片
    img = sensor.snapshot()
    clock.tick()
    #如果帧数没达到1000
    if fps_count < 1000:
        #保存当前图片为1帧
        m.add_frame(img)
        print(clock.fps())
        fps_count += 1
    else:
        #关闭文件才保存成功，需要传入保存视频的帧率，可以自己设定，参数填24表示保存的视频就是1秒钟播放24帧
        m.close(clock.fps())
        break


