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
#include "ipspro_page_ctrl.h"

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

// 短按按键1，如果在页面2，则控制参数名称或者参数的加

// 短按按键2，如果在页面1，则随机显示角度，图像等参数
// 短按按键2，如果在页面2，则控制参数名称或者参数的加

// 短按按键3，如果在页面2，则控制参数名称或者参数的减

// 短按按键4切换页面1和页面2
// 长按按键4切换页面3
// 


uint32 rand_num = 0;
int angle, far_value, image_err, run;

int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
    system_delay_ms(300);
    
    // ips200pro初始化，如果flash的数据全是-0.0000，先看看是否完成flash数据初始化，查看ipspro_page_ctrl_flash_init函数即可
    ipspro_page_ctrl_init();
    
    // 设置页面3的识别结果
    ipspro_page_ctrl_show_page3(1, "显示器");
    ipspro_page_ctrl_show_page3(2, "23");
    ipspro_page_ctrl_show_page3(3, "手机");
    ipspro_page_ctrl_show_page3(4, "万用表");
    ipspro_page_ctrl_show_page3(5, "卷尺");
    ipspro_page_ctrl_show_page3(6, "76");
    ipspro_page_ctrl_show_page3(7, "音响");
    ipspro_page_ctrl_show_page3(8, "电烙铁");
    
    while(1)
    {
        // 累计数据充当随机数
        rand_num++;
        rand_num %= 10000;
        
        // 显示页面1的数据
        ipspro_page_ctrl_show_page1(angle, far_value, image_err, run);
        
        // 页面的按键控制，需要在pit的通道一中断中添加按键回调函数
        ipspro_page_ctrl_key_loop();
    }
}



