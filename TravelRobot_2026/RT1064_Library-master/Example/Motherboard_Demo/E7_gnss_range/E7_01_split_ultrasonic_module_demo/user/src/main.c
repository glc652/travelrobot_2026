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
// 接入有来有去模块
//      模块管脚            单片机管脚
//      SPLIT_ULTRASONIC_RX C16
//      SPLIT_ULTRASONIC_TX C17
//      SPLIT_ULTRASONIC_EN B9
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源



// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程，单独使用核心板与调试下载器或者 USB-TTL 模块，并连接好有来有去模块，在断电情况下完成连接
// 
// 2.将调试下载器或者 USB-TTL 模块连接电脑，完成上电
// 
// 3.电脑上使用逐飞助手上位机打开对应的串口，串口波特率为 zf_common_debug.h 文件中 DEBUG_UART_BAUDRATE 宏定义 默认 115200，核心板按下复位按键
// 
// 4.可以在逐飞助手上位机上看到如下串口信息：
//      Ranging start.
//      Ranging counter us is x.
// 
// 5.模块对着改变两个模块间距离就会看到测距数值变化
// 
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查


// **************************** 代码区域 ****************************
#define SPLIT_ULTRASONIC_UART       (UART_4)
#define SPLIT_ULTRASONIC_BAUD       (115200)
#define SPLIT_ULTRASONIC_TX         (UART4_RX_C17)
#define SPLIT_ULTRASONIC_RX         (UART4_TX_C16)

#define SPLIT_ULTRASONIC_EN         (B9 )                                       // 设置对应端口 有去模块发送 这里其实可以不用控制

uint16 ranging_counter = 0;                                                     // 测距数据
uint8 ranging_flage = 0x00;                                                     // 测距状态

uint8 uart_data;
uint8 dat[3];
uint8 num;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     超声波 uart_handler 的中断处理函数 这个函数将在 UART_4 对应的串口中断调用 详见 isr.c
// 参数说明     void
// 返回参数     void
// 使用示例                     uart_handler();
//-------------------------------------------------------------------------------------------------------------------
void uart_handler (void)
{
    uart_query_byte(SPLIT_ULTRASONIC_UART, &uart_data);
    dat[num] = uart_data;
    if(dat[0] != 0xa5)  num = 0;                                                // 检查第一个数据是否为0xa5
    else                num ++;

    if(num == 3)                                                                // 接收完成 开始处理数据
    {
        num = 0;
        ranging_counter = dat[1] << 8 | dat[2];                                 // 得到超声波模块测出的距离
        ranging_flage = 0x01;
    }
}

int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
            
    system_delay_ms(300);           //等待主板其他外设上电完成
    
    gpio_init(SPLIT_ULTRASONIC_EN, GPO, GPIO_LOW, GPO_PUSH_PULL);               // 初始化有去模块使能引脚
    uart_init(SPLIT_ULTRASONIC_UART, SPLIT_ULTRASONIC_BAUD, SPLIT_ULTRASONIC_RX, SPLIT_ULTRASONIC_TX);
    uart_rx_interrupt(SPLIT_ULTRASONIC_UART, 1);
    // 此处编写用户代码 例如外设初始化代码等

    printf("\r\n Ranging start.");
    gpio_set_level(SPLIT_ULTRASONIC_EN, GPIO_HIGH);                                   // 拉高使能 有去模块发送 实际接上电上拉就开始发送 程序可以不用控制

    interrupt_global_enable(0);
    
    while(1)
    {
        if(ranging_flage)                                                       // 等待测距完毕
        {
            printf("\r\n Ranging counter us is %d.", ranging_counter);          // 输出测距信息
            ranging_flage = 0x00;
        }

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
// 问题3：串口只输出了“Ranging start.”
//      检查模块连接是否正常，是否按照说明书一样闪灯
// 
// 问题4：数值异常
//      请确保在说明书有效距离内
//      请确保在说明书有效角度内
//      请确保模块连接正常

