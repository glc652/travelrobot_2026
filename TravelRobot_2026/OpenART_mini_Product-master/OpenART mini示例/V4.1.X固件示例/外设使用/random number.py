import pyb,time

while(True):
    # 获取随机数
    num = pyb.rng()
    # 打印随机数
    print(num)
    # 延时200ms
    time.sleep_ms(200)
