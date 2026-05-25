import sensor, image, time, math

sensor.reset()
sensor.set_pixformat(sensor.RGB565) # 设置图像色彩为RGB565
sensor.set_framesize(sensor.QVGA)   # 设置图像格式为QVGA （320*240）
sensor.set_framerate(60)            # 设置帧率为60帧  可以填写2、8、15、30、50、60
sensor.set_auto_whitebal(True)      # 设置自动白平衡
sensor.skip_frames(time = 200)      # 跳过200帧，摄像头设置参数后需要跳过一定帧率
clock = time.clock()

# 设置色块检测阈值，可以在工具的阈值编辑器调整阈值，默认为红色
thresholds = [(0, 52, 30, 127, -128, 127)]

while(True):
    # 帧率计算
    clock.tick()
    # 获取图像
    img = sensor.snapshot()
    # 根据阈值检测色块
    # thresholds 色彩阈值
    # pixels_threshold像素阈值 小于此值会过滤掉
    # area_threshold面积阈值 色块外接矩形的面积小于此值会被过滤
    # merge 将同一位置的阈值合并
    for blob in img.find_blobs(thresholds, pixels_threshold=100, area_threshold=100, merge=True):
        img.draw_rectangle(blob.rect())

    # 显示帧率
    print(clock.fps())
