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
//      将核心板插在主板上 确保插到底核心板与主板插座间没有缝隙即可
//      将核心板插在主板上 确保插到底核心板与主板插座间没有缝隙即可
//      将核心板插在主板上 确保插到底核心板与主板插座间没有缝隙即可
// 
//      主板按键            单片机管脚
//      S1                  查看 zf_device_key.h 文件中 KEY1_PIN 宏定义的引脚 默认 C15
//      S2                  查看 zf_device_key.h 文件中 KEY2_PIN 宏定义的引脚 默认 C14
//      S3                  查看 zf_device_key.h 文件中 KEY3_PIN 宏定义的引脚 默认 C13
//      S4                  查看 zf_device_key.h 文件中 KEY4_PIN 宏定义的引脚 默认 C12
// 
//      主板拨码开关        单片机管脚
//      S5-1                C26
//      S5-2                C27
// 
//      主板蜂鸣器          单片机管脚
//      BEEP                B11


// *************************** 例程测试说明 ***************************
// 1.核心板插在主板上 主板使用电池供电 下载本例程
// 
// 2.复位核心板 LED会先闪烁两次 蜂鸣器也会响两次
// 
// 3.短按一下 S1-S4 中任意按键 BEEP 会响一下
// 
// 4.长按 S1-S4 中任意按键 BEEP 会一直响
// 
// 5.拨动 SWITCH1/SWITCH2 拨码开关 LED1 会切换闪烁或者熄灭状态
// 
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

// **************************** 代码区域 ****************************
#define SWITCH1             (C27)
#define SWITCH2             (C26)

#define LED1                (B9)

#define BEEP                (B11)

int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
    
    system_delay_ms(300);           //等待主板其他外设上电完成
    
    uint8 count_beep = 0;
    uint8 count_led = 0;

    // key_index_enum key_index_array[KEY_NUMBER] = {KEY_1,KEY_2,KEY_3,KEY_4};

    key_init(5);

    gpio_init(SWITCH1, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(SWITCH2, GPI, GPIO_HIGH, GPI_PULL_UP);

    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);

    gpio_init(BEEP, GPO, GPIO_LOW, GPO_PUSH_PULL);

    gpio_set_level(LED1, GPIO_LOW);                                             // LED1 亮
    gpio_set_level(BEEP, GPIO_HIGH);                                            // BEEP 响
    system_delay_ms(100);
    gpio_set_level(LED1, GPIO_HIGH);                                            // LED1 灭
    gpio_set_level(BEEP, GPIO_LOW);                                             // BEEP 停
    system_delay_ms(100);
    gpio_set_level(LED1, GPIO_LOW);                                             // LED1 亮
    gpio_set_level(BEEP, GPIO_HIGH);                                            // BEEP 响
    system_delay_ms(100);
    gpio_set_level(LED1, GPIO_HIGH);                                            // LED1 灭
    gpio_set_level(BEEP, GPIO_LOW);                                             // BEEP 停
    system_delay_ms(100);
    interrupt_global_enable(0);
    
    while(1)
    {
        if(!gpio_get_level(SWITCH1) || !gpio_get_level(SWITCH2))                // SWITCH1/SWITCH2 ON
        {
            if(25 > count_led)
            {
                gpio_set_level(LED1, GPIO_LOW);                                 // LED1 亮
            }
            else
            {
                gpio_set_level(LED1, GPIO_HIGH);                                // LED1 灭
            }
        }
        else
        {
            gpio_set_level(LED1, GPIO_HIGH);                                    // LED1 灭
        }

        count_led = ((count_led != 100) ? (count_led + 1) : (1));
        
        key_scanner();

        if( KEY_SHORT_PRESS == key_get_state(KEY_1) ||
            KEY_SHORT_PRESS == key_get_state(KEY_2) ||
            KEY_SHORT_PRESS == key_get_state(KEY_3) ||
            KEY_SHORT_PRESS == key_get_state(KEY_4))                            // 任意按键短按
        {
            // 短按的按键在松开时 状态才会被 key_scanner 置位为 KEY_SHORT_PRESS
            count_beep = 40;
            // 可以单独清除按键状态
            key_clear_state(KEY_1);
            key_clear_state(KEY_2);
            key_clear_state(KEY_3);
            key_clear_state(KEY_4);
        }
        else if(KEY_LONG_PRESS == key_get_state(KEY_1) ||
                KEY_LONG_PRESS == key_get_state(KEY_2) ||
                KEY_LONG_PRESS == key_get_state(KEY_3) ||
                KEY_LONG_PRESS == key_get_state(KEY_4))                         // 任意按键长按
        {
            // 长按的按键在按下期间会被 key_scanner 不断置位为 KEY_LONG_PRESS
            // 所以即使清除了本次的状态 在下次扫描时依旧会判定为 KEY_LONG_PRESS
            count_beep = 40;
            // 也可以清除所有按键状态
            key_clear_all_state();
        }

        if(count_beep)
        {
            gpio_set_level(BEEP, GPIO_HIGH);
            count_beep --;
        }
        else
        {
            gpio_set_level(BEEP, GPIO_LOW);
        }
        
        system_delay_ms(5);
    }
}

// **************************** 代码区域 ****************************

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 
// 问题1：S1-S4 按下 BEEP 不响
//      如果使用主板测试，主板必须要用电池供电
//      查看程序是否正常烧录，是否下载报错，确认正常按下复位按键
//      万用表测量对应 BEEP 引脚电压是否变化，如果不变化证明程序未运行，如果变化证明 BEEP 电路或者器件损坏
//      万用表检查对应 S1-S4 引脚电压是否正常变化，是否跟接入信号不符，引脚是否接错，是否断路短路
// 
// 问题2：SWITCH1 / SWITCH2 拨动无 LED 反应
//      如果使用主板测试，主板必须要用电池供电
//      查看程序是否正常烧录，是否下载报错，确认正常按下复位按键
//      万用表测量对应 LED 引脚电压是否变化，如果不变化证明程序未运行，如果变化证明 LED 灯珠损坏
//      万用表检查对应 SWITCH1 / SWITCH2 引脚电压是否正常变化，是否跟接入信号不符，引脚是否接错，是否断路短路



