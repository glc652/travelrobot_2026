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
// 使用 IPS200PRO屏幕插到主板上
//      CLK                 查看 zf_device_ips200pro.h 中 IPS200PRO_CLK_PIN 宏定义  默认  B0
//      MOSI                查看 zf_device_ips200pro.h 中 IPS200PRO_MOSI_PIN 宏定义 默认  B1
//      MISO                查看 zf_device_ips200pro.h 中 IPS200PRO_MISO_PIN 宏定义 默认  NULL
//      RST                 查看 zf_device_ips200pro.h 中 IPS200PRO_RST_PIN 宏定义  默认  B2
//      INT                 查看 zf_device_ips200pro.h 中 IPS200PRO_INT_PIN 宏定义  默认  C19
//      CS                  查看 zf_device_ips200pro.h 中 IPS200PRO_CS_PIN 宏定义   默认  B3
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源


// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程 主板电池供电
// 
// 2，接上屏幕，按一下复位，屏幕上会显示亮起并显示页面
// 
// 3.通过按键控制页面 

// **************************** 代码区域 ****************************

// 保存屏幕的栈大小，
uint32 ips200pro_stack_size;

// 保存创建的页面ID，当需要对页面进行操作的时候则需要填写此ID
uint16 page_id[3];

uint16 label_id[4];

uint8  power = 66;
float  speed = 66.666;

char   test_str[] = "长文本语句测试，长文本语句测试\n长文本语句测试，长文本语句测试\n长文本语句测试，长文本语句测试\n长文本语句测试，";

int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
    system_delay_ms(300);
    
    // 初始化屏幕并创建一个页面
    page_id[0] = ips200pro_init("Label测试", IPS200PRO_TITLE_BOTTOM, 30);

    // 获取屏幕信息，根据个人需求调用 1064主板屏幕接口没有连接芯片的miso，所以无法使用获取的功能
    ips200pro_get_information(&ips200pro_information);
    ips200pro_get_free_stack_size(&ips200pro_stack_size);
    ips200pro_get_time(&ips200pro_time);

    // 设置全局字体为16号
    ips200pro_set_default_font(FONT_SIZE_16);

    // 屏幕设置为竖屏
    ips200pro_set_direction(IPS200PRO_PORTRAIT);

    // 设置编码格式
    ips200pro_set_format(IPS200PRO_FORMAT_GBK);

    // 设置选中页面的标题背景颜色，填写任意一个创建完成的页面ID即可
    ips200pro_set_color(page_id[0], COLOR_PAGE_SELECTED_BG, IPS200PRO_RGB888_TO_RGB565(0xFF, 0xE3, 0xE3));

    // 设置第一个页面的背景色
    ips200pro_set_color(page_id[0], COLOR_BACKGROUND, IPS200PRO_RGB888_TO_RGB565(0x43, 0x4A, 0x61));

    // 设置页面标题文字颜色，填写任意一个创建完成的页面ID即可
    ips200pro_set_color(page_id[0], COLOR_FOREGROUND, RGB565_BLACK);

    // 设置页面选中后的标题文字颜色，填写任意一个创建完成的页面ID即可
    ips200pro_set_color(page_id[0], COLOR_PAGE_SELECTED_TEXT, RGB565_WHITE);

    // 切换页面，切换到第一个页面
    ips200pro_page_switch(page_id[0] ,PAGE_ANIM_ON);

    // 创建标签，指定XY的起点以及宽度高度，显示的内容不会超多设置的宽度与高度，如果文本内容过长则可以采用滚动、截断等方式进行显示
    label_id[0] = ips200pro_label_create(0, 0, 120, 20);

    // 显示字符串，文本由于超过label的宽度，默认采用循环滚动显示,如果需要修改则可以使用ips200pro_label_mode函数进行修改
    ips200pro_label_printf(label_id[0], "This is a test");

    // 修改文字颜色
    ips200pro_set_color(label_id[0], COLOR_FOREGROUND, IPS200PRO_RGB888_TO_RGB565(0x43, 0x4A, 0x61));
    // 设置Label背景颜色 如果不设置则默认与背景颜色一致
    ips200pro_set_color(label_id[0], COLOR_BACKGROUND, RGB565_WHITE);
    // 设置Label边线颜色 如果不设置则默认与背景颜色一致
    ips200pro_set_color(label_id[0], COLOR_BORDER, IPS200PRO_RGB888_TO_RGB565(0x43, 0x4A, 0x61));

    label_id[1] = ips200pro_label_create(0, 30, 200, 60);
    ips200pro_set_color(label_id[1], COLOR_FOREGROUND, RGB565_WHITE);
    // 中文、字符串、变量显示，使用方法与printf几乎一致，因此可以自行学习C语言的printf函数的使用，如果需要换行显示则可以使用 \n进行换行
    ips200pro_label_printf(label_id[1], "这是一个测试\nPower=%d\nSpeed=%f", power, speed);

    label_id[2] = ips200pro_label_create(0, 90, 200, 100);
    ips200pro_set_color(label_id[2], COLOR_FOREGROUND, RGB565_WHITE);
    ips200pro_label_printf(label_id[2], "中文大字体测试，\n仅16、20、24号\n字体支持中文显示");
    // 特别注意、特别注意、特别注意
    // 设置Label的字体大小，需要特别注意只有16、20、24才支持中文显示
    // 特别注意、特别注意、特别注意
    ips200pro_set_font(label_id[2], FONT_SIZE_24);

    label_id[3] = ips200pro_label_create(0, 180, 240, 100);
    ips200pro_set_color(label_id[3], COLOR_FOREGROUND, RGB565_WHITE);
    // 如果文本长度超过50字节，则只能使用ips200pro_label_show_string函数进行显示，无法使用printf格式化并输出的方式显示
    ips200pro_label_show_string(label_id[3], test_str);

    // 设置屏幕背光亮度，可以设置范围0-255
    ips200pro_set_backlight(255);
    while (1)
    {
    }
}



