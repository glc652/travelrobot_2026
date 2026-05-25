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
// *************************** 例程硬件连接说明 ***************************
// 使用逐飞科技调试下载器连接
//      直接将下载器正确连接在核心板的调试下载接口即可
//
// 接入总钻风灰度数字摄像头 对应主板摄像头接口 请注意线序
//      模块管脚            单片机管脚
//      TXD                 查看 zf_device_mt9v03x.h 中 MT9V03X_COF_UART_TX 宏定义
//      RXD                 查看 zf_device_mt9v03x.h 中 MT9V03X_COF_UART_RX 宏定义
//      PCLK                查看 zf_device_mt9v03x.h 中 MT9V03X_PCLK_PIN 宏定义
//      VSY                 查看 zf_device_mt9v03x.h 中 MT9V03X_VSYNC_PIN 宏定义
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源
// 
// 接入无线转串口模块
//      模块管脚            单片机管脚
//      RX                  查看 zf_device_wrieless_uart.h 中 WRIELESS_UART_RX_PINx 宏定义 默认 D16
//      TX                  查看 zf_device_wrieless_uart.h 中 WRIELESS_UART_TX_PINx 宏定义 默认 D17
//      RTS                 查看 zf_device_wrieless_uart.h 中 WRIELESS_UART_RTS_PINx 宏定义 默认 D26
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源
// *************************** 例程使用步骤说明 ***************************
// 1.根据硬件连接说明连接好模块，使用电源供电(下载器供电会导致模块电压不足)
//
// 2.下载例程到单片机中，打开逐飞助手上位机。
//
// 3.在逐飞助手上位机中，选择图像传输。
//
// 4.选择无线串口对应的串口号，波特率(默认115200)，点击连接
//
// 5.等待几秒钟，图像就会显示在逐飞串口助手中。


// *************************** 例程测试说明 ***************************
// 1.连接好模块和核心板后，无线串口的USB端插到电脑上（尽量使用配套主板测试以避免供电不足的问题） 烧录本例程 按下复位后程序开始运行
//
// 2.如果模块未能正常初始化 核心板上的LED1灯会闪烁 一般情况下重试会成功
//
// 3.打开逐飞助手上位机，选择图像传输，打开无线串口的串口号，连接可以看到图像显示
//
// 4.使用115200波特率。逐飞助手的图像大概两三秒一帧，这是由于串口传输慢导致的，并非摄像头采集问题。
//
// 5.115200波特率一秒钟大约传输11.25KB数据，一副图像188x120的分辨率大约有22KB的数据。
//

// **************************** 代码区域 ****************************
//0：不包含边界信息
//1：包含三条边线信息，边线信息只包含横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点
//2：包含三条边线信息，边界信息只含有纵轴坐标，横轴坐标由图像宽度得到，意味着每个边界在一列中只会有一个点，一般来说很少有这样的使用需求
//3：包含三条边线信息，边界信息含有横纵轴坐标，意味着你可以指定每个点的横纵坐标，边线的数量也可以大于或者小于图像的高度，通常来说边线数量大于图像的高度，一般是搜线算法能找出回弯的情况
//4：没有图像信息，仅包含三条边线信息，边线信息只包含横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点，这样的方式可以极大的降低传输的数据量
#define INCLUDE_BOUNDARY_TYPE   3

// 边界的点数量远大于图像高度，便于保存回弯的情况
#define BOUNDARY_NUM            (MT9V03X_H * 3 / 2)

uint8 xy_x1_boundary[BOUNDARY_NUM], xy_x2_boundary[BOUNDARY_NUM], xy_x3_boundary[BOUNDARY_NUM];
uint8 xy_y1_boundary[BOUNDARY_NUM], xy_y2_boundary[BOUNDARY_NUM], xy_y3_boundary[BOUNDARY_NUM];

uint8 x1_boundary[MT9V03X_H], x2_boundary[MT9V03X_H], x3_boundary[MT9V03X_H];
uint8 y1_boundary[MT9V03X_W], y2_boundary[MT9V03X_W], y3_boundary[MT9V03X_W];

// 图像备份数组，在发送前将图像备份再进行发送，这样可以避免图像出现撕裂的问题
uint8 image_copy[MT9V03X_H][MT9V03X_W];

#define LED1                    (B9 )

int main (void)
{
    clock_init(SYSTEM_CLOCK_600M);                                              // 不可删除
    debug_init();                                                               // 调试端口初始化
    system_delay_ms(300);
    gpio_init(LED1, GPO, 0, FAST_GPO_PUSH_PULL);                                // 初始化 LED1 输出 默认高电平 推挽输出模式
    if(wireless_uart_init())                                                    // 判断是否通过初始化
    {
        while(1)                                                                // 初始化失败就在这进入死循环
        {
            gpio_toggle_level(LED1);                                            // 翻转 LED 引脚输出电平 控制 LED 亮灭
            system_delay_ms(100);                                               // 短延时快速闪灯表示异常
        }
    }
    
    // 设置逐飞助手使用DEBUG串口进行收发
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIRELESS_UART);
    
#if(0 != INCLUDE_BOUNDARY_TYPE)
    int32 i=0;
#endif

#if(3 <= INCLUDE_BOUNDARY_TYPE)
    int32 j=0;
#endif

    // 此处编写用户代码 例如外设初始化代码等
    while(1)
    {
        if(mt9v03x_init())
            gpio_toggle_level(LED1);                                            // 翻转 LED 引脚输出电平 控制 LED 亮灭 初始化出错这个灯会闪的很慢
        else
            break;
        system_delay_ms(500);                                                   // 闪灯表示异常
    }

#if(0 == INCLUDE_BOUNDARY_TYPE)
    // 发送总钻风图像信息(仅包含原始图像信息)
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, image_copy[0], MT9V03X_W, MT9V03X_H);

#elif(1 == INCLUDE_BOUNDARY_TYPE)
    // 发送总钻风图像信息(并且包含三条边界信息，边界信息只含有横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点)
    // 对边界数组写入数据
    for(i = 0; i < MT9V03X_H; i++)
    {
        x1_boundary[i] = 70 - (70 - 20) * i / MT9V03X_H;
        x2_boundary[i] = MT9V03X_W / 2;
        x3_boundary[i] = 118 + (168 - 118) * i / MT9V03X_H;
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, image_copy[0], MT9V03X_W, MT9V03X_H);
    seekfree_assistant_camera_boundary_config(X_BOUNDARY, MT9V03X_H, x1_boundary, x2_boundary, x3_boundary, NULL, NULL ,NULL);


#elif(2 == INCLUDE_BOUNDARY_TYPE)
    // 发送总钻风图像信息(并且包含三条边界信息，边界信息只含有纵轴坐标，横轴坐标由图像宽度得到，意味着每个边界在一列中只会有一个点)
    // 通常很少有这样的使用需求
    // 对边界数组写入数据
    for(i = 0; i < MT9V03X_W; i++)
    {
        y1_boundary[i] = i * MT9V03X_H / MT9V03X_W;
        y2_boundary[i] = MT9V03X_H / 2;
        y3_boundary[i] = (MT9V03X_W - i) * MT9V03X_H / MT9V03X_W;
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, image_copy[0], MT9V03X_W, MT9V03X_H);
    seekfree_assistant_camera_boundary_config(Y_BOUNDARY, MT9V03X_W, NULL, NULL ,NULL, y1_boundary, y2_boundary, y3_boundary);


#elif(3 == INCLUDE_BOUNDARY_TYPE)
    // 发送总钻风图像信息(并且包含三条边界信息，边界信息含有横纵轴坐标)
    // 这样的方式可以实现对于有回弯的边界显示
    j = 0;
    for(i = MT9V03X_H - 1; i >= MT9V03X_H / 2; i--)
    {
        // 直线部分
        xy_x1_boundary[j] = 34;
        xy_y1_boundary[j] = i;

        xy_x2_boundary[j] = 47;
        xy_y2_boundary[j] = i;

        xy_x3_boundary[j] = 60;
        xy_y3_boundary[j] = i;
        j++;
    }

    for(i = MT9V03X_H / 2 - 1; i >= 0; i--)
    {
        // 直线连接弯道部分
        xy_x1_boundary[j] = 34 + (MT9V03X_H / 2 - i) * (MT9V03X_W / 2 - 34) / (MT9V03X_H / 2);
        xy_y1_boundary[j] = i;

        xy_x2_boundary[j] = 47 + (MT9V03X_H / 2 - i) * (MT9V03X_W / 2 - 47) / (MT9V03X_H / 2);
        xy_y2_boundary[j] = 15 + i * 3 / 4;

        xy_x3_boundary[j] = 60 + (MT9V03X_H / 2 - i) * (MT9V03X_W / 2 - 60) / (MT9V03X_H / 2);
        xy_y3_boundary[j] = 30 + i / 2;
        j++;
    }

    for(i = 0; i < MT9V03X_H / 2; i++)
    {
        // 回弯部分
        xy_x1_boundary[j] = MT9V03X_W / 2 + i * (138 - MT9V03X_W / 2) / (MT9V03X_H / 2);
        xy_y1_boundary[j] = i;

        xy_x2_boundary[j] = MT9V03X_W / 2 + i * (133 - MT9V03X_W / 2) / (MT9V03X_H / 2);
        xy_y2_boundary[j] = 15 + i * 3 / 4;

        xy_x3_boundary[j] = MT9V03X_W / 2 + i * (128 - MT9V03X_W / 2) / (MT9V03X_H / 2);
        xy_y3_boundary[j] = 30 + i / 2;
        j++;
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, image_copy[0], MT9V03X_W, MT9V03X_H);
    seekfree_assistant_camera_boundary_config(XY_BOUNDARY, BOUNDARY_NUM, xy_x1_boundary, xy_x2_boundary, xy_x3_boundary, xy_y1_boundary, xy_y2_boundary, xy_y3_boundary);


#elif(4 == INCLUDE_BOUNDARY_TYPE)
    // 发送总钻风图像信息(并且包含三条边界信息，边界信息只含有横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点)
    // 对边界数组写入数据
    for(i = 0; i < MT9V03X_H; i++)
    {
        x1_boundary[i] = 70 - (70 - 20) * i / MT9V03X_H;
        x2_boundary[i] = MT9V03X_W / 2;
        x3_boundary[i] = 118 + (168 - 118) * i / MT9V03X_H;
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, NULL, MT9V03X_W, MT9V03X_H);
    seekfree_assistant_camera_boundary_config(X_BOUNDARY, MT9V03X_H, x1_boundary, x2_boundary, x3_boundary, NULL, NULL ,NULL);


#endif


    // 此处编写用户代码 例如外设初始化代码等

    while(1)
    {
        // 此处编写需要循环执行的代码

        if(mt9v03x_finish_flag)
        {
            mt9v03x_finish_flag = 0;

            // 在发送前将图像备份再进行发送，这样可以避免图像出现撕裂的问题
            memcpy(image_copy[0], mt9v03x_image[0], MT9V03X_IMAGE_SIZE);

            // 发送图像
            seekfree_assistant_camera_send();
        }

        // 此处编写需要循环执行的代码
    }
}

