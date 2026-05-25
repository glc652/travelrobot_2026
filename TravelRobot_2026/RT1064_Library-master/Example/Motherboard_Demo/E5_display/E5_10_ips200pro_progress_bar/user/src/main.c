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

uint16 progress_bar_id[5];

int8 i;

int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
    system_delay_ms(300);
    
    // 初始化屏幕并创建一个页面
    page_id[0] = ips200pro_init("Progress Bar测试", IPS200PRO_TITLE_BOTTOM, 30);

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

    // 设置屏幕背光亮度，可以设置范围0-255
    ips200pro_set_backlight(255);

    // 切换页面，切换到第一个页面
    ips200pro_page_switch(page_id[0] ,PAGE_ANIM_ON);

    // 创建一个长条形进度条
    progress_bar_id[0] = ips200pro_progress_bar_create(20, 10, 200, 20);

    progress_bar_id[1] = ips200pro_progress_bar_create(20, 50, 200, 20);

    // 创建一个圆形进度条，当宽度与高度相等时进度条为圆形
    progress_bar_id[2] = ips200pro_progress_bar_create(30, 80, 60, 60);

    progress_bar_id[3] = ips200pro_progress_bar_create(160, 80, 60, 60);

    // 创建一个纵向进度条
    progress_bar_id[4] = ips200pro_progress_bar_create(110, 80, 20, 200);

    // 修改进度条颜色
    ips200pro_set_color(progress_bar_id[0], COLOR_FOREGROUND, IPS200PRO_RGB888_TO_RGB565(255, 150, 78));
    // 设置进度条背景颜色
    ips200pro_set_color(progress_bar_id[0], COLOR_BACKGROUND, RGB565_WHITE);

    // 修改进度条颜色
    ips200pro_set_color(progress_bar_id[1], COLOR_FOREGROUND, IPS200PRO_RGB888_TO_RGB565(255, 150, 78));
    // 设置进度条背景颜色
    ips200pro_set_color(progress_bar_id[1], COLOR_BACKGROUND, RGB565_WHITE);

    // 修改进度条颜色
    ips200pro_set_color(progress_bar_id[2], COLOR_FOREGROUND, IPS200PRO_RGB888_TO_RGB565(255, 150, 78));
    // 设置进度条背景颜色
    ips200pro_set_color(progress_bar_id[2], COLOR_BACKGROUND, RGB565_WHITE);

    // 修改进度条颜色
    ips200pro_set_color(progress_bar_id[3], COLOR_FOREGROUND, IPS200PRO_RGB888_TO_RGB565(255, 150, 78));
    // 设置进度条背景颜色
    ips200pro_set_color(progress_bar_id[3], COLOR_BACKGROUND, RGB565_WHITE);

    // 修改进度条颜色
    ips200pro_set_color(progress_bar_id[4], COLOR_FOREGROUND, IPS200PRO_RGB888_TO_RGB565(255, 150, 78));
    // 设置进度条背景颜色
    ips200pro_set_color(progress_bar_id[4], COLOR_BACKGROUND, RGB565_WHITE);
    while (1)
    {
        for(i=0; 90 > i; i++)
        {
            // 起始值固定为0仅修改结束值
            ips200pro_progress_bar_set_value(progress_bar_id[0], 0, i);
            ips200pro_progress_bar_set_value(progress_bar_id[2], 0, i);
            ips200pro_progress_bar_set_value(progress_bar_id[4], 0, i);
            // 同时修改起始值与结束值，从而实现类似水平仪的效果
            ips200pro_progress_bar_set_value(progress_bar_id[1], i, i + 10);
            ips200pro_progress_bar_set_value(progress_bar_id[3], i, i + 10);
            system_delay_ms(20);
        }

        // 反向
        for(i=100; 10 <= i; i--)
        {
            // 进度条数值设置
            ips200pro_progress_bar_set_value(progress_bar_id[0], i, 100);
            ips200pro_progress_bar_set_value(progress_bar_id[2], i, 100);
            ips200pro_progress_bar_set_value(progress_bar_id[4], i, 100);

            ips200pro_progress_bar_set_value(progress_bar_id[1], i - 10, i);
            ips200pro_progress_bar_set_value(progress_bar_id[3], i - 10, i);

            system_delay_ms(20);
        }
    }
}



