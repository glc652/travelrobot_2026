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
//      模块管脚            单片机管脚
//      SCL                 查看 zf_device_tft180.h 中 TFT180_SCL_PIN 宏定义 默认 B0
//      SDA                 查看 zf_device_tft180.h 中 TFT180_SDA_PIN 宏定义 默认 B1
//      RES                 查看 zf_device_tft180.h 中 TFT180_RES_PIN 宏定义 默认 B2 
//      DC                  查看 zf_device_tft180.h 中 TFT180_DC_PIN 宏定义 默认 C19
//      CS                  查看 zf_device_tft180.h 中 TFT180_CS_PIN 宏定义 默认 B3 
//      BL                  查看 zf_device_tft180.h 中 TFT180_BL_PIN 宏定义 默认 C18
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源



// *************************** 例程测试说明 ***************************
// 1.核心板烧录本例程 插在主板上 1.8寸TFT 显示模块插在主板的屏幕接口排座上 请注意引脚对应 不要插错
// 
// 2.电池供电 上电后 1.8寸TFT 屏幕亮起 显示字符数字浮点数和波形图
// 
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查


// **************************** 代码区域 ****************************
int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
        
    system_delay_ms(300);           //等待主板其他外设上电完成
    
    uint16_t data[128];
    int16_t data_index = 0;
    for( ; data_index < 64; data_index ++)
        data[data_index] = data_index;
    for(data_index = 64; data_index < 128; data_index ++)
        data[data_index] = 128 - data_index;

    tft180_set_dir(TFT180_PORTAIT);
    tft180_set_font(TFT180_8X16_FONT);
    tft180_set_color(RGB565_RED, RGB565_BLACK);
    tft180_init();
    interrupt_global_enable(0);
    
    while(1)
    {
        tft180_clear();
        tft180_show_rgb565_image(4, 60, (const uint16 *)gImage_seekfree_logo, 240, 80, 120, 40, 0); // 显示一个RGB565色彩图片 原图240*80 显示240*80 低位在前
        system_delay_ms(1500);

        tft180_full(RGB565_GRAY);
        tft180_show_string( 0,  16*0,   "SEEKFREE");                            // 显示字符串
        tft180_show_chinese(0,  16*1,   16, (const uint8 *)chinese_test, 4, RGB565_BLUE);           // 显示汉字

        // 显示的 flaot 数据 最多显示 8bit 位整数 最多显示 6bit 位小数
        tft180_show_float(  0,  16*2,   -13.141592,     1, 6);                  // 显示 float 数据 1bit 整数 6bit 小数 应当显示 -3.141592 总共会有 9 个字符的显示占位
        tft180_show_float(  0,  16*3,   13.141592,      8, 4);                  // 显示 float 数据 8bit 整数 4bit 小数 应当显示 13.1415 总共会有 14 个字符的显示占位 后面会有 5 个字符的空白占位

        tft180_show_int(    0,  16*4,   -127,           2);                     // 显示 int8 数据
        tft180_show_uint(   0,  16*5,   255,            4);                     // 显示 uint8 数据

        tft180_show_int(    0,  16*6,  -32768,          4);                     // 显示 int16 数据
        tft180_show_uint(   0,  16*7,  65535,           6);                     // 显示 uint16 数据

        tft180_show_int(    0,  16*8,  -2147483648,     8);                     // 显示 int32 数据 8bit 整数 应当显示 -47483648
        tft180_show_uint(   0,  16*9,  4294967295,      8);                     // 显示 uint32 数据 10bit 整数 应当显示 4294967295

        system_delay_ms(1000);

        tft180_full(RGB565_GRAY);
        tft180_show_wave(32, 64, data, 128, 64,  64, 32);                       // 显示一个三角波形 波形宽度 128 波形最大值 64 显示宽度 64 显示最大值 32
        system_delay_ms(1000);
        tft180_full(RGB565_GRAY);
        tft180_show_wave( 0, 48, data, 128, 64, 128, 64);                       // 显示一个三角波形 波形宽度 128 波形最大值 64 显示宽度 128 显示最大值 64
        system_delay_ms(1000);

        // 使用画线函数 从顶上两个角画射线
        tft180_clear();
        for(data_index = 0; data_index < 128; data_index += 10)
        {
            tft180_draw_line(0, 0, data_index, 160 - 1, RGB565_66CCFF);
            system_delay_ms(20);
        }
        tft180_draw_line(0, 0, 128 - 1, 160 - 1, RGB565_66CCFF);
        for(data_index = 150; data_index >= 0; data_index -= 10)
        {
            tft180_draw_line(0, 0, 128 - 1, data_index, RGB565_66CCFF);
            system_delay_ms(20);
        }

        tft180_draw_line(128 - 1, 0, 128 - 1, 160 - 1, RGB565_66CCFF);
        for(data_index = 120; data_index > 0; data_index -= 10)
        {
            tft180_draw_line(128 - 1, 0, data_index, 160 - 1, RGB565_66CCFF);
            system_delay_ms(20);
        }
        tft180_draw_line(128 - 1, 0, 0, 160 - 1, RGB565_66CCFF);
        for(data_index = 150; data_index >= 0; data_index -= 10)
        {
            tft180_draw_line(128 - 1, 0, 0, data_index, RGB565_66CCFF);
            system_delay_ms(20);
        }
        system_delay_ms(1000);

        tft180_full(RGB565_RED);
        system_delay_ms(500);
        tft180_full(RGB565_GREEN);
        system_delay_ms(500);
        tft180_full(RGB565_BLUE);
        system_delay_ms(500);
        tft180_full(RGB565_WHITE);
        system_delay_ms(500);
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


