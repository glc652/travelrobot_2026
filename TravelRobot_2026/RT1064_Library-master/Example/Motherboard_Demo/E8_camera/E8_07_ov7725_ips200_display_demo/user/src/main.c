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
// 接入小钻风硬件二值化摄像头 对应主板摄像头接口 请注意线序
//      模块管脚            单片机管脚
//      TXD/SCL             查看 zf_device_ov7725.h 中 OV7725_COF_UART_TX 或 OV7725_COF_IIC_SCL 宏定义
//      RXD/SDA             查看 zf_device_ov7725.h 中 OV7725_COF_UART_RX 或 OV7725_COF_IIC_SDA 宏定义
//      PCLK                查看 zf_device_ov7725.h 中 OV7725_PCLK_PIN 宏定义
//      VSY                 查看 zf_device_ov7725.h 中 OV7725_VSYNC_PIN 宏定义
//      D0-D7               查看 zf_device_ov7725.h 中 OV7725_DATA_PIN 宏定义 从该定义开始的连续八个引脚
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源
// 
// 接入2寸IPS模块
//      并口两寸屏 硬件引脚
//      RD                  查看 zf_device_ips200.h 中 IPS200_RD_PIN_PARALLEL8     宏定义 B0 
//      WR                  查看 zf_device_ips200.h 中 IPS200_WR_PIN_PARALLEL8     宏定义 B1 
//      RS                  查看 zf_device_ips200.h 中 IPS200_RS_PIN_PARALLEL8     宏定义 B2 
//      RST                 查看 zf_device_ips200.h 中 IPS200_RST_PIN_PARALLEL8    宏定义 C19
//      CS                  查看 zf_device_ips200.h 中 IPS200_CS_PIN_PARALLEL8     宏定义 B3 
//      BL                  查看 zf_device_ips200.h 中 IPS200_BL_PIN_PARALLEL8     宏定义 C18
//      D0-D7               查看 zf_device_ips200.h 中 IPS200_Dx_PIN_PARALLEL8     宏定义 B16/B17/B18/B19/D12/D13/D14/D15
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源
//      SPI 两寸屏 硬件引脚
//      SCL                 查看 zf_device_ips200.h 中 IPS200_SCL_PIN_SPI  宏定义  B0
//      SDA                 查看 zf_device_ips200.h 中 IPS200_SDA_PIN_SPI  宏定义  B1
//      RST                 查看 zf_device_ips200.h 中 IPS200_RST_PIN_SPI  宏定义  B2
//      DC                  查看 zf_device_ips200.h 中 IPS200_DC_PIN_SPI   宏定义  C19
//      CS                  查看 zf_device_ips200.h 中 IPS200_CS_PIN_SPI   宏定义  B3 
//      BL                  查看 zf_device_ips200.h 中 IPS200_BLk_PIN_SPI  宏定义  C18
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源


// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程 将核心板插在主板上 插到底
// 
// 2.摄像头接在主板的摄像头接口 注意线序2寸IPS模块插入主板屏幕接口
// 
// 3.主板上电 或者核心板链接完毕后上电 核心板按下复位按键
// 
// 4.屏幕会显示初始化信息然后显示摄像头图像
// 
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

// **************************** 代码区域 ****************************
#define IPS200_TYPE     (IPS200_TYPE_PARALLEL8)                                 // 并口两寸屏 这里宏定义填写 IPS200_TYPE_PARALLEL8
                                                                                // SPI 两寸屏 这里宏定义填写 IPS200_TYPE_SPI

int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
    	        
    system_delay_ms(300);           //等待主板其他外设上电完成
    
    ips200_init(IPS200_TYPE);
    ips200_show_string(0, 0, "ov7725 init.");
    while(1)
    {
        if(ov7725_init())
            ips200_show_string(0, 16, "ov7725 reinit.");
        else
            break;
        system_delay_ms(500);                                                   // 短延时快速闪灯表示异常
    }
    ips200_show_string(0, 16, "init success.");
    interrupt_global_enable(0);
    
    while(1)
    {
        if(ov7725_finish_flag)
        {
            ips200_displayimage7725((const uint8 *)ov7725_image_binary, 240, 180);
//            ips200_show_binary_image(0, 0, (const uint8 *)ov7725_image_binary, OV7725_W, OV7725_H, 240, 180);
            ov7725_finish_flag = 0;
        }
    }
}

// **************************** 代码区域 ****************************

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 
// 问题1：屏幕不显示
//      如果使用主板测试，主板必须要用电池供电 检查屏幕供电引脚电压
//      检查屏幕是不是插错位置了 检查引脚对应关系
//      如果对应引脚都正确 检查一下是否有引脚波形不对 需要有示波器
//      无法完成波形测试则复制一个GPIO例程将屏幕所有IO初始化为GPIO翻转电平 看看是否受控
// 
// 问题2：显示 reinit 字样
//      检查接线是否正常
//      主板供电是否使用电量充足的电池供电
// 
// 问题2：显示图像杂乱 错位
//      检查摄像头信号线是否有松动

