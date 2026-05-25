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
// 接入凌瞳摄像头 对应主板摄像头接口 请注意线序
//      模块管脚            单片机管脚
//      TXD                 查看 zf_device_scc8660.h 中 SCC8660_COF_UART_TX 宏定义
//      RXD                 查看 zf_device_scc8660.h 中 SCC8660_COF_UART_RX 宏定义
//      PCLK                查看 zf_device_scc8660.h 中 SCC8660_PCLK_PIN 宏定义
//      VSY                 查看 zf_device_scc8660.h 中 SCC8660_VSYNC_PIN 宏定义
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源

// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程 将核心板插在主板上 插到底 摄像头接在主板的摄像头接口 注意线序
// 
// 2.主板上电 或者核心板链接完毕后上电 用TypeC数据线连接电脑和核心板上的TypeC接口
// 
// 3.电脑上使用逐飞助手上位机，选择图像传输。
// 
// 4.打开1064的虚拟串口
// 
// 5.上位机将会显示图像


// **************************** 代码区域 ****************************
//0：不包含边界信息
//1：包含三条边线信息，边线信息只包含横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点
//2：包含三条边线信息，边界信息只含有纵轴坐标，横轴坐标由图像宽度得到，意味着每个边界在一列中只会有一个点，一般来说很少有这样的使用需求
//3：包含三条边线信息，边界信息含有横纵轴坐标，意味着你可以指定每个点的横纵坐标，边线的数量也可以大于或者小于图像的高度，通常来说边线数量大于图像的高度，一般是搜线算法能找出回弯的情况
//4：没有图像信息，仅包含三条边线信息，边线信息只包含横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点，这样的方式可以极大的降低传输的数据量
#define INCLUDE_BOUNDARY_TYPE   3

// 边界的点数量远大于图像高度，便于保存回弯的情况
#define BOUNDARY_NUM            (SCC8660_H * 3 / 2)

uint8 xy_x1_boundary[BOUNDARY_NUM], xy_x2_boundary[BOUNDARY_NUM], xy_x3_boundary[BOUNDARY_NUM];
uint8 xy_y1_boundary[BOUNDARY_NUM], xy_y2_boundary[BOUNDARY_NUM], xy_y3_boundary[BOUNDARY_NUM];

uint8 x1_boundary[SCC8660_H], x2_boundary[SCC8660_H], x3_boundary[SCC8660_H];
uint8 y1_boundary[SCC8660_W], y2_boundary[SCC8660_W], y3_boundary[SCC8660_W];

// 图像备份数组，在发送前将图像备份再进行发送，这样可以避免图像出现撕裂的问题
uint16 image_copy[SCC8660_H][SCC8660_W];
#define LED1                    (B9 )

int main (void)
{
    clock_init(SYSTEM_CLOCK_600M);                                              // 不可删除
    debug_init();                                                               // 调试端口初始化
    system_delay_ms(300);
    usb_cdc_init();
    // 设置逐飞助手使用DEBUG串口进行收发
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_CUSTOM);
#if(0 != INCLUDE_BOUNDARY_TYPE)
    int32 i=0;
#endif

#if(3 <= INCLUDE_BOUNDARY_TYPE)
    int32 j=0;
#endif

    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED1 输出 默认高电平 推挽输出模式

    // 此处编写用户代码 例如外设初始化代码等
    while(1)
    {
        if(scc8660_init())
            gpio_toggle_level(LED1);                                            // 翻转 LED 引脚输出电平 控制 LED 亮灭 初始化出错这个灯会闪的很慢
        else
            break;
        system_delay_ms(500);                                                  // 闪灯表示异常
    }

#if(0 == INCLUDE_BOUNDARY_TYPE)
    // 发送总钻风图像信息(仅包含原始图像信息)
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_SCC8660, image_copy[0], SCC8660_W, SCC8660_H);

#elif(1 == INCLUDE_BOUNDARY_TYPE)
    // 发送总钻风图像信息(并且包含三条边界信息，边界信息只含有横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点)
    // 对边界数组写入数据
    for(i = 0; i < SCC8660_H; i++)
    {
        x1_boundary[i] = 70 - (70 - 20) * i / SCC8660_H;
        x2_boundary[i] = SCC8660_W / 2;
        x3_boundary[i] = 118 + (168 - 118) * i / SCC8660_H;
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_SCC8660, image_copy[0], SCC8660_W, SCC8660_H);
    seekfree_assistant_camera_boundary_config(X_BOUNDARY, SCC8660_H, x1_boundary, x2_boundary, x3_boundary, NULL, NULL ,NULL);


#elif(2 == INCLUDE_BOUNDARY_TYPE)
    // 发送总钻风图像信息(并且包含三条边界信息，边界信息只含有纵轴坐标，横轴坐标由图像宽度得到，意味着每个边界在一列中只会有一个点)
    // 通常很少有这样的使用需求
    // 对边界数组写入数据
    for(i = 0; i < SCC8660_W; i++)
    {
        y1_boundary[i] = i * SCC8660_H / SCC8660_W;
        y2_boundary[i] = SCC8660_H / 2;
        y3_boundary[i] = (SCC8660_W - i) * SCC8660_H / SCC8660_W;
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_SCC8660, image_copy[0], SCC8660_W, SCC8660_H);
    seekfree_assistant_camera_boundary_config(Y_BOUNDARY, SCC8660_W, NULL, NULL ,NULL, y1_boundary, y2_boundary, y3_boundary);


#elif(3 == INCLUDE_BOUNDARY_TYPE)
    // 发送总钻风图像信息(并且包含三条边界信息，边界信息含有横纵轴坐标)
    // 这样的方式可以实现对于有回弯的边界显示
    j = 0;
    for(i = SCC8660_H - 1; i >= SCC8660_H / 2; i--)
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

    for(i = SCC8660_H / 2 - 1; i >= 0; i--)
    {
        // 直线连接弯道部分
        xy_x1_boundary[j] = 34 + (SCC8660_H / 2 - i) * (SCC8660_W / 2 - 34) / (SCC8660_H / 2);
        xy_y1_boundary[j] = i;

        xy_x2_boundary[j] = 47 + (SCC8660_H / 2 - i) * (SCC8660_W / 2 - 47) / (SCC8660_H / 2);
        xy_y2_boundary[j] = 15 + i * 3 / 4;

        xy_x3_boundary[j] = 60 + (SCC8660_H / 2 - i) * (SCC8660_W / 2 - 60) / (SCC8660_H / 2);
        xy_y3_boundary[j] = 30 + i / 2;
        j++;
    }

    for(i = 0; i < SCC8660_H / 2; i++)
    {
        // 回弯部分
        xy_x1_boundary[j] = SCC8660_W / 2 + i * (138 - SCC8660_W / 2) / (SCC8660_H / 2);
        xy_y1_boundary[j] = i;

        xy_x2_boundary[j] = SCC8660_W / 2 + i * (133 - SCC8660_W / 2) / (SCC8660_H / 2);
        xy_y2_boundary[j] = 15 + i * 3 / 4;

        xy_x3_boundary[j] = SCC8660_W / 2 + i * (128 - SCC8660_W / 2) / (SCC8660_H / 2);
        xy_y3_boundary[j] = 30 + i / 2;
        j++;
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_SCC8660, image_copy[0], SCC8660_W, SCC8660_H);
    seekfree_assistant_camera_boundary_config(XY_BOUNDARY, BOUNDARY_NUM, xy_x1_boundary, xy_x2_boundary, xy_x3_boundary, xy_y1_boundary, xy_y2_boundary, xy_y3_boundary);


#elif(4 == INCLUDE_BOUNDARY_TYPE)
    // 发送总钻风图像信息(并且包含三条边界信息，边界信息只含有横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点)
    // 对边界数组写入数据
    for(i = 0; i < SCC8660_H; i++)
    {
        x1_boundary[i] = 70 - (70 - 20) * i / SCC8660_H;
        x2_boundary[i] = SCC8660_W / 2;
        x3_boundary[i] = 118 + (168 - 118) * i / SCC8660_H;
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_SCC8660, NULL, SCC8660_W, SCC8660_H);
    seekfree_assistant_camera_boundary_config(X_BOUNDARY, SCC8660_H, x1_boundary, x2_boundary, x3_boundary, NULL, NULL ,NULL);


#endif


    // 此处编写用户代码 例如外设初始化代码等

    while(1)
    {
        // 此处编写需要循环执行的代码

        if(scc8660_finish_flag)
        {
            scc8660_finish_flag = 0;

            // 发送图像
            memcpy(image_copy[0], scc8660_image[0], SCC8660_IMAGE_SIZE);
            seekfree_assistant_camera_send();
        }

        // 此处编写需要循环执行的代码
    }
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     逐飞助手发送函数
// 参数说明     *buff           需要发送的数据地址
// 参数说明     length          需要发送的长度
// 返回参数     uint32          剩余未发送数据长度
// 使用示例
//-------------------------------------------------------------------------------------------------------------------
uint32 seekfree_assistant_transfer(const uint8 *buff, uint32 length)
{
    usb_cdc_write_buffer((uint8 *)buff, length);
    return 0;
}
