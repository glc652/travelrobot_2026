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
// 接入无线转串口模块
//      模块管脚            单片机管脚
//      RX                  查看 zf_device_wrieless_uart.h 中 WRIELESS_UART_RX_PINx 宏定义 默认 D16
//      TX                  查看 zf_device_wrieless_uart.h 中 WRIELESS_UART_TX_PINx 宏定义 默认 D17
//      RTS                 查看 zf_device_wrieless_uart.h 中 WRIELESS_UART_RTS_PINx 宏定义 默认 D26
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源



// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程
// 
// 2.如果使用主板测试 在断电情况下 将核心板插入主板 无线转串口模块插入主板无线模块接口
// 
// 3.如果使用核心板和模块测试 在断电情况下 将核心板与无线模块连接
// 
// 4.通过逐飞助手上位机打开无线转串口 USB端的串口  串口波特率为 zf_device_wireless_uart.h 文件中 WRIELESS_UART_BUAD_RATE 宏定义 默认 115200，核心板按下复位按键
// 
// 5.单片机上电复位后 可以在逐飞助手上位机上看到如下串口信息：
//      SEEKFREE wireless uart demo.
// 
// 6.逐飞助手上位机发送数据会收到回复消息
// 
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查


// **************************** 代码区域 ****************************
#define LED1                    (B9 )

uint8 data_buffer[32];
uint8 data_len;

int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
        
    system_delay_ms(300);           //等待主板其他外设上电完成
    
    uint8 count = 0;

    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED1 输出 默认高电平 推挽输出模式
    if(wireless_uart_init())                                                    // 判断是否通过初始化
    {
        while(1)                                                                // 初始化失败就在这进入死循环
        {
            gpio_toggle_level(LED1);                                            // 翻转 LED 引脚输出电平 控制 LED 亮灭
            system_delay_ms(100);                                               // 短延时快速闪灯表示异常
        }
    }
    wireless_uart_send_byte('\r');
    wireless_uart_send_byte('\n');
    wireless_uart_send_string("SEEKFREE wireless uart demo.\r\n");              // 初始化正常 输出测试信息
    
    interrupt_global_enable(0);
    
    // 此处编写用户代码 例如外设初始化代码等

    while(1)
    {
        // 此处编写需要循环执行的代码
        data_len = wireless_uart_read_buffer(data_buffer, 32);                    // 查看是否有消息 默认缓冲区是 WIRELESS_UART_BUFFER_SIZE 总共 64 字节
        if(data_len != 0)                                                       // 收到了消息 读取函数会返回实际读取到的数据个数
        {
            wireless_uart_send_buffer(data_buffer, data_len);                     // 将收到的消息发送回去
            memset(data_buffer, 0, 32);
            func_uint_to_str((char *)data_buffer, data_len);
            wireless_uart_send_string("\r\ndata len:");                                 // 显示实际收到的数据信息
            wireless_uart_send_buffer(data_buffer, strlen((const char *)data_buffer));    // 显示收到的数据个数
            wireless_uart_send_string(".\r\n");
        }
        if(count ++ > 10)
        {
            count = 0;
            gpio_toggle_level(LED1);                                            // 翻转 LED 引脚输出电平 控制 LED 亮灭
        }
        system_delay_ms(50);
    }
}

// **************************** 代码区域 ****************************

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 
// 问题1：串口没有数据
//      查看逐飞助手上位机打开的是否是正确的串口，检查打开的 COM 口是否对应的是 无线转串口 的 COM 口
//      查看核心板LED灯状态 如果快速闪烁证明初始化失败 检查接线 或者按照说明书检查模块是否正常
// 
// 问题2：串口数据乱码
//      查看是否模块波特率被修改过 恢复出厂设置
// 
// 问题3：发送数据没有收到回复
//      调试看看是否读取到数据 data_len 是否一直都是 如果是 那么按照说明书检查模块是否正常


