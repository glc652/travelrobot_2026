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
// 如果使用的并不是逐飞科技 RT1064 主板 请按照下述方式进行接线
// 以下按照1064主板加上1064无刷转接板进行定义
//      模块引脚    单片机引脚
//      无刷电机1
//      PWM         查看 main.c 中 BLDC_MOTOR1_PWM  宏定义 默认 D0
//      DIR         查看 main.c 中 BLDC_MOTOR1_DIR  宏定义 默认 D2
//      SPDOUT      查看 main.c 中 BLDC_MOTOR1_ENCODER_A 宏定义 默认 C0
//      DIROUT      查看 main.c 中 BLDC_MOTOR1_ENCODER_B  宏定义 默认 C1
//      GND         电源地

//      无刷电机2
//      PWM         查看 main.c 中 BLDC_MOTOR2_PWM  宏定义 默认 D1
//      DIR         查看 main.c 中 BLDC_MOTOR2_DIR  宏定义 默认 D3
//      SPDOUT      查看 main.c 中 BLDC_MOTOR2_ENCODER_A 宏定义 默认 C2
//      DIROUT      查看 main.c 中 BLDC_MOTOR2_ENCODER_B  宏定义 默认 C24
//      GND         电源地

// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程 主板电池供电
// 
// 2.驱动接上电机 可以看到电机周期正反转
// 
// 3.调试串口会持续打印当前速度信息.输出信息应当如下：
//      bldc encoder counter: x
// 
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

// **************************** 代码区域 ****************************
#define MAX_DUTY            (30 )                                                               // 最大占空比输出限制
// 无刷电机1接口定义
#define BLDC_MOTOR1_PWM                 (PWM2_MODULE1_CHB_C9)                                   // PWM输出端口
#define BLDC_MOTOR1_DIR                 (C8 )                                                   // 电机方向输出端口     
#define BLDC_MOTOR1_ENCODER_TIM         (QTIMER1_ENCODER1)                                      // 编码器定时器
#define BLDC_MOTOR1_ENCODER_A           (QTIMER1_ENCODER1_CH1_C0)                               // 编码器计数端口
#define BLDC_MOTOR1_ENCODER_B           (QTIMER1_ENCODER1_CH2_C1)                               // 编码器方向采值端口
// 无刷电机2接口定义    
#define BLDC_MOTOR2_PWM                 (PWM2_MODULE0_CHB_C7)                                   // PWM输出端口
#define BLDC_MOTOR2_DIR                 (C6 )                                                   // 电机方向输出端口     
#define BLDC_MOTOR2_ENCODER_TIM         (QTIMER1_ENCODER2)                                      // 编码器定时器
#define BLDC_MOTOR2_ENCODER_A           (QTIMER1_ENCODER2_CH1_C2)                               // 编码器计数端口
#define BLDC_MOTOR2_ENCODER_B           (QTIMER1_ENCODER2_CH2_C24)                              // 编码器方向采值端口
    
#define PIT_CH                          (PIT_CH0 )                                              // 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
                    
int8 duty = 0;                                                                                  // 当前占空比
bool dir = true;                                                                                // 计数方向
int16 encoder1=0, encoder2=0;                                                                   // 编码器值
                
int main(void)              
{               
    clock_init(SYSTEM_CLOCK_600M);                                                              // 不可删除             
    debug_init();                                                                               // 调试端口初始化              
                                                                    
    system_delay_ms(300);                                                                       //等待主板其他外设上电完成
                    
    pwm_init(BLDC_MOTOR1_PWM, 1000, 0);                                                         // PWM 初始化频率1KHz 占空比初始为0
    gpio_init(BLDC_MOTOR1_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                                  // 初始化电机方向输出引脚           
    encoder_dir_init(BLDC_MOTOR1_ENCODER_TIM, BLDC_MOTOR1_ENCODER_A, BLDC_MOTOR1_ENCODER_B);    // 初始化编码器采值引脚及定时器
                    
    pwm_init(BLDC_MOTOR2_PWM, 1000, 0);                                                         // PWM 初始化频率1KHz 占空比初始为0
    gpio_init(BLDC_MOTOR2_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                                  // 初始化电机方向输出引脚           
    encoder_dir_init(BLDC_MOTOR2_ENCODER_TIM, BLDC_MOTOR2_ENCODER_A, BLDC_MOTOR2_ENCODER_B);    // 初始化编码器采值引脚及定时器

    pit_ms_init(PIT_CH, 100);
    
    interrupt_global_enable(0);
    
    while(1)
    {
        printf("bldc encoder counter: motor1 = %d,motor2 = %d \r\n", encoder1, encoder2);       // 串口输出采集的数据
        system_delay_ms(300);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 的中断处理函数 这个函数将在 PIT 对应的定时器中断调用 详见 isr.c
// 参数说明     void
// 返回参数     void
// 使用示例     pit_handler();
//-------------------------------------------------------------------------------------------------------------------
void pit_handler (void)
{
    if(duty >= 0)                                                                               // 正转
    {       
        pwm_set_duty(BLDC_MOTOR1_PWM, duty * (PWM_DUTY_MAX / 100));                             // 计算占空比
        gpio_set_level(BLDC_MOTOR1_DIR, 1);     
                
        pwm_set_duty(BLDC_MOTOR2_PWM, duty * (PWM_DUTY_MAX / 100));                             // 计算占空比
        gpio_set_level(BLDC_MOTOR2_DIR, 1);     
    }       
    else                                                                                        // 反转
    {
        pwm_set_duty(BLDC_MOTOR1_PWM, -duty * (PWM_DUTY_MAX / 100));     
        gpio_set_level(BLDC_MOTOR1_DIR, 0);
        
        pwm_set_duty(BLDC_MOTOR2_PWM, -duty * (PWM_DUTY_MAX / 100));     
        gpio_set_level(BLDC_MOTOR2_DIR, 0);
    }
    if(dir)                                                                                     // 根据方向判断计数方向 本例程仅作参考
    {               
        duty ++;                                                                                // 正向计数
        if(duty >= MAX_DUTY)                                                                    // 达到最大值
        dir = false;                                                                            // 变更计数方向
    }               
    else                
    {               
        duty --;                                                                                // 反向计数
        if(duty <= -MAX_DUTY)                                                                   // 达到最小值
        dir = true;                                                                             // 变更计数方向
    }   
    
    encoder1 = encoder_get_count(BLDC_MOTOR1_ENCODER_TIM);                                      // 采集对应编码器数据
    encoder_clear_count(BLDC_MOTOR1_ENCODER_TIM);                                               // 清除对应计数
    
    encoder2 = encoder_get_count(BLDC_MOTOR2_ENCODER_TIM);                                      // 采集对应编码器数据
    encoder_clear_count(BLDC_MOTOR2_ENCODER_TIM);                                               // 清除对应计数
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
// 问题3：无刷电机无反应
//      检查信号引脚是否接对，信号线是否松动
//      使用万用表测量实际PWM输出引脚是否有输出或者示波器查看波形
// 
// 问题4：无刷电机转动但转速显示无速度
//      检查信号引脚是否接对，信号线是否松动
//      使用万用表测量对应的无刷电机速度输出是否有信号
