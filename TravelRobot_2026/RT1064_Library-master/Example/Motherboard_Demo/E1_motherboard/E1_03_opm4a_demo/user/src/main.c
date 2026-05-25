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
// 电容电感安装板连接运放输入 主板上电容电感安装板连接对应接口
//      模块管脚            单片机管脚
//      L1                  电磁接口1-B14
//      L2                  电磁接口1-B15
//      L3                  电磁接口1-B21
//      L4                  电磁接口1-B23
//      GND                 核心板电源地 GND
// 
// 运放输出接单片机引脚 主板上运放模块直接插在对应接口
//      模块管脚            单片机管脚
//      O1                  电磁运放模块-B14
//      O2                  电磁运放模块-B15
//      O3                  电磁运放模块-B21
//      O4                  电磁运放模块-B23
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源

// *************************** 例程测试说明 ***************************
// 1.这个例程需要连接电磁电容检测部分 并且要接好运放模块
// 
// 2.接好后下载这个模块还需要一个赛道信号源接上电磁线测试
// 
// 3.接好硬件信号源接好后 本例程会通过串口输出各个电容电感对的检测值
// 
// 4.电脑上使用逐飞助手上位机打开对应的串口，串口波特率为 zf_common_debug.h 文件中 DEBUG_UART_BAUDRATE 宏定义 默认 115200
//
// 5.根据实际值调整对应的运放的滑变电阻调整放大倍数
// 
// 6.输出信息应当如下：
//      Lx data is xxx.
// 
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

// **************************** 代码区域 ****************************
#define CHANNEL_NUMBER          (4)

#define ADC_CHANNEL1            (ADC1_CH3_B14)
#define ADC_CHANNEL2            (ADC1_CH4_B15)
#define ADC_CHANNEL3            (ADC1_CH10_B21)
#define ADC_CHANNEL4            (ADC1_CH12_B23)

uint8 channel_index = 0;
adc_channel_enum channel_list[CHANNEL_NUMBER] = 
{
    ADC_CHANNEL1, ADC_CHANNEL2, ADC_CHANNEL3, ADC_CHANNEL4,
};

int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
        
    system_delay_ms(300);           //等待主板其他外设上电完成
    
    adc_init(ADC_CHANNEL1, ADC_12BIT);                                          // 初始化对应 ADC 通道为对应精度
    adc_init(ADC_CHANNEL2, ADC_12BIT);                                          // 初始化对应 ADC 通道为对应精度
    adc_init(ADC_CHANNEL3, ADC_12BIT);                                          // 初始化对应 ADC 通道为对应精度
    adc_init(ADC_CHANNEL4, ADC_12BIT);                                          // 初始化对应 ADC 通道为对应精度

    interrupt_global_enable(0);
    
    while(1)
    {
        for(channel_index = 0; channel_index < CHANNEL_NUMBER; channel_index ++)
        {
            printf(
                "L%d data is %d.\r\n",
                channel_index + 1,
                adc_convert(channel_list[channel_index]));                      // 循环输出单次转换结果
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
// 问题3：对应电感在电磁线上晃动串口输出数据无变化
//      检查信号引脚是否接对，信号线是否松动
//      使用万用表测量实际引脚上电压是多少 然后按照 voltage/3V3*4096 计算


