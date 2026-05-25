/*********************************************************************************************************************
* RT1064DVL6A Opensourec Library 即（RT1064DVL6A 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 RT1064DVL6A 开源库的一部分
* 
* RT1064DVL6A 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 8.32.4 or MDK 5.33
* 适用平台          RT1064DVL6A
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2022-09-21        SeekFree            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"

// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完

// *************************** 例程硬件连接说明 ***************************
// 使用逐飞科技 CMSIS-DAP | ARM 调试下载器连接
//      直接将下载器正确连接在核心板的调试下载接口即可
// 
// 使用 USB-TTL 模块连接
//      模块管脚            单片机管脚
//      USB-TTL-RX          查看 zf_common_debug.h 文件中 DEBUG_UART_TX_PIN 宏定义的引脚 默认 B12
//      USB-TTL-TX          查看 zf_common_debug.h 文件中 DEBUG_UART_RX_PIN 宏定义的引脚 默认 B13
//      USB-TTL-GND         核心板电源地 GND
//      USB-TTL-3V3         核心板 3V3 电源
// 
// 接入 IMU660RC
//      模块管脚            单片机管脚
//      SCL/SPC             查看 zf_device_imu660rc.h 中 IMU660RC_SPC_PIN 	宏定义
//      SDA/DSI             查看 zf_device_imu660rc.h 中 IMU660RC_SDI_PIN 	宏定义
//      SA0/SDO             查看 zf_device_imu660rc.h 中 IMU660RC_SDO_PIN 	宏定义
//      CS                  查看 zf_device_imu660rc.h 中 IMU660RC_CS_PIN  	宏定义
//      INT2                查看 zf_device_imu660rc.h 中 IMU660RC_INT2_PIN  宏定义
//      GND                 电源地 GND
//      3V3                 电源 3V3



// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程，单独使用核心板与调试下载器或者 USB-TTL 模块，并连接好 IMU660RC 模块，在断电情况下完成连接
// 
// 2.将调试下载器或者 USB-TTL 模块连接电脑 完成上电 正常 B9 LED 会闪烁
// 
// 3.电脑上使用逐飞助手上位机打开对应的串口，串口波特率为 zf_common_debug.h 文件中 DEBUG_UART_BAUDRATE 宏定义 默认 115200，核心板按下复位按键
// 
// 4.可以在逐飞助手上位机上看到如下串口信息：
//      IMU660RC acc data: x-..., y-..., z-...
//      IMU660RC gyro data: x-..., y-..., z-...
//      IMU660RC  euler_angles data: roll-..., pitch-..., yaw-...
// 5.移动旋转 IMU660RC 就会看到数值变化
// 
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查


// **************************** 代码区域 ****************************
#define LED1                        (B9 )

//==========特别数注意==========
//==========特别数注意==========
//==========特别数注意==========

// 本程序默认通过中断引脚采集IMU660RC的四元数并解算欧拉角，需在硬件上做如下接线：
// 将IMU660RC的INT2引脚连接至单片机的外部中断引脚（具体哪个引脚请查看 zf_device_imu660rc.h 中 imu660rc_INT2_PIN  宏定义），否则无法使用欧拉角解算功能。
//
// 若仅需获取加速度计和角速度数据（无需欧拉角），则无需额外接线：
// 初始化函数改为调用 imu660rc_init(IMU660RC_QUARTERNION_DISABLE);
// 并在周期中断服务函数中调用 imu660rc_get_acc() 和 imu660rc_get_gyro()，
// 即可分别读取加速度计和角速度数据。

int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
        
    system_delay_ms(300);           //等待主板其他外设上电完成
    
    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED1 输出 默认高电平 推挽输出模式
		wireless_uart_init();
    while(1)
    {
        if(imu660rc_init(IMU660RC_QUARTERNION_120HZ))														// 设置 IMU660RC 以120HZ的速度产生中断触发信号
        {
            printf("\r\nIMU660RC init error.");                                 // IMU660RC 初始化失败
        }
        else
        {
            break;
        }
        gpio_toggle_level(LED1);                                                // 翻转 LED 引脚输出电平 控制 LED 亮灭 初始化出错这个灯会闪的很慢
    }

    while(1)
    {
				//在isr.c的外部中断服务中调用了 imu660rc_callback(); 完成了所有数据的采集。因此不用在单独调用 imu660rc_get_acc() 和 imu660rc_get_gyro()
//        printf("\r\nIMU660RC acc data: x=%5d, y=%5d, z=%5d\r\n", imu660rc_acc_x, imu660rc_acc_y, imu660rc_acc_z);
//        printf("\r\nIMU660RC gyro data:  x=%5d, y=%5d, z=%5d\r\n", imu660rc_gyro_x, imu660rc_gyro_y, imu660rc_gyro_z);
				printf("\r\nIMU660RC euler_angles data:  roll=%5f, pitch=%5f, yaw=%5f\r\n", imu660rc_roll, imu660rc_pitch, imu660rc_yaw);
        gpio_toggle_level(LED1);                                                // 翻转 LED 引脚输出电平 控制 LED 亮灭
        system_delay_ms(1000);
    }
}


// **************************** 代码区域 ****************************

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 
// 问题1：串口没有数据
//      查看逐飞助手上位机打开的是否是正确的串口，检查打开的 COM 口是否对应的是调试下载器或者 USB-TTL 模块的 COM 口
//      如果是使用逐飞科技 CMSIS-DAP | ARM 调试下载器连接，那么检查下载器线是否松动，检查核心板串口跳线是否已经焊接，串口跳线查看核心板原理图即可找到
//      如果是使用 USB-TTL 模块连接，那么检查连线是否正常是否松动，模块 TX 是否连接的核心板的 RX，模块 RX 是否连接的核心板的 TX
// 
// 问题2：串口数据乱码
//      查看逐飞助手上位机设置的波特率是否与程序设置一致，程序中 zf_common_debug.h 文件中 DEBUG_UART_BAUDRATE 宏定义为 debug uart 使用的串口波特率
// 
// 问题3：串口输出 IMU660RC init error.
//      检查 IMU660RC 的接线是否正确
//      检查 IMU660RC 的模块是不是坏了
//      给信号线加上拉看看
// 
// 问题4：IMU660RC 数值异常
//      看看是不是线松了 或者信号线被短路了
//      可能模块部分受损


