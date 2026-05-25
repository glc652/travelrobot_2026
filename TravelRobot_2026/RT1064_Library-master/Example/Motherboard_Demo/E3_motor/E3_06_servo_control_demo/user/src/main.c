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
// 接入舵机 主板上对应有舵机的接口 务必注意不要插反 <红色电源> <黑色接地> <黄色/橙色/棕色/白色...其它彩色的那根是信号>
//      模块管脚            单片机管脚
//      PWM                 C30
//      GND                 舵机电源 GND 连通 核心板电源地 GND
//      VCC                 舵机电源输出

// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程
//
// 2.在断电情况下 完成核心板插到主板的核心板插座上 确认核心板与主板插座没有明显缝隙
//
// 3.然后将舵机与主板正确连接 请务必注意不要插反 然后使用电池给主板供电打开开关
//
// 4.正常情况下舵机会来回转动 最好在舵机没有装在车上固定连接转向连杆时测试 防止安装位置不对造成堵转烧舵机
// 
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

// **************************** 代码区域 ****************************
#define SERVO_MOTOR1_PWM             (PWM4_MODULE2_CHA_C30)                          // 定义主板上舵机对应引脚
#define SERVO_MOTOR2_PWM             (PWM1_MODULE3_CHA_D0)                          // 定义主板上舵机对应引脚
#define SERVO_MOTOR3_PWM             (PWM1_MODULE3_CHB_D1)                          // 定义主板上舵机对应引脚
#define SERVO_MOTOR_FREQ            (50 )                                           // 定义主板上舵机频率  请务必注意范围 50-300

#define SERVO_MOTOR_L_MAX           (75 )                                           // 定义主板上舵机活动范围 角度
#define SERVO_MOTOR_R_MAX           (105)                                           // 定义主板上舵机活动范围 角度

// ------------------ 舵机占空比计算方式 ------------------
// 
// 舵机对应的 0-180 活动角度对应 控制脉冲的 0.5ms-2.5ms 高电平
// 
// 那么不同频率下的占空比计算方式就是
// PWM_DUTY_MAX/(1000/freq)*(1+Angle/180) 在 50hz 时就是 PWM_DUTY_MAX/(1000/50)*(1+Angle/180)
// 
// 那么 100hz 下 90度的打角 即高电平时间1.5ms 计算套用为
// PWM_DUTY_MAX/(1000/100)*(1+90/180) = PWM_DUTY_MAX/10*1.5
// 
// ------------------ 舵机占空比计算方式 ------------------
#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))

#if (SERVO_MOTOR_FREQ<50 || SERVO_MOTOR_FREQ>300)
    #error "SERVO_MOTOR_FREQ ERROE!"
#endif

float servo_motor_duty = 90.0;                                                  // 舵机动作角度
float servo_motor_dir = 1;                                                      // 舵机动作状态

int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
        
    system_delay_ms(300);           //等待主板其他外设上电完成
    
    pwm_init(SERVO_MOTOR1_PWM, SERVO_MOTOR_FREQ, 0);
    pwm_init(SERVO_MOTOR2_PWM, SERVO_MOTOR_FREQ, 0);
    pwm_init(SERVO_MOTOR3_PWM, SERVO_MOTOR_FREQ, 0);
    
    interrupt_global_enable(0);
    
    while(1)
    {
        pwm_set_duty(SERVO_MOTOR1_PWM, (uint32)SERVO_MOTOR_DUTY(servo_motor_duty));
        pwm_set_duty(SERVO_MOTOR2_PWM, (uint32)SERVO_MOTOR_DUTY(servo_motor_duty));
        pwm_set_duty(SERVO_MOTOR3_PWM, (uint32)SERVO_MOTOR_DUTY(servo_motor_duty));

        if(servo_motor_dir)
        {
            servo_motor_duty ++;
            if(servo_motor_duty >= SERVO_MOTOR_R_MAX)
            {
                servo_motor_dir = 0x00;
            }
        }
        else
        {
            servo_motor_duty --;
            if(servo_motor_duty <= SERVO_MOTOR_L_MAX)
            {
                servo_motor_dir = 0x01;
            }
        }
        system_delay_ms(50);         
    }
}

// **************************** 代码区域 ****************************

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 
// 问题1：舵机不动
//      检查舵机供电是否正常 至少5V供电 不可以用杜邦线供电
//      检查PWM信号是否正常 是否连通



