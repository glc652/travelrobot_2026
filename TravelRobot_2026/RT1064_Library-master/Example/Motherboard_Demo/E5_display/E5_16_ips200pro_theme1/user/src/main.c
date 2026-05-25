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
#include "user_menu.h"
#include "user_menu_device.h"

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
// 1.该例程为综合菜单例程 需结合总钻风摄像头以及主板按键使用
// 2.核心板烧录本例程 插在主板上
//   2寸IPS-lx 显示模块插在主板的屏幕接口排座上
//   总钻风摄像头通过摄像头转接板连接到摄像头接口
//   请注意主板按键是否与zf_device_key.h文件内的宏定义对应
//   请注意引脚对应 不要插错
// 2.电池供电 上电后 2寸IPS 屏幕亮起 显示电池电量、CPU占用率、总钻风图像、实时参数表格
// 3.单击KEY1按键，可切换到参数设置界面 此时电池电量、CPU占用率进度条会收起，浮现参数设置表格
// 4.再次单击KEY1按键，可切换参数选择 逐渐向下
// 5.单击KEY2按键，此时选中的参数数值会增加
// 6.单击KEY3按键，此时选中的参数数值会减少
// 7.单击KEY4按键，将会退出参数设置界面 此时电池电量、CPU占用率进度条会浮现，收起参数设置表格
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

// **************************** 代码区域 ****************************

int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
    system_delay_ms(300);
    
    mt9v03x_init();                         // 总钻风摄像头 初始化配置

    key_init(20);                           // 按键驱动库 初始化配置 按键扫描在用户菜单循环函数内 因此若摄像头是默认的50帧 则对应20ms周期

    user_menu_init();                       // 用户菜单  初始化配置

    while (1)
    {
        if(mt9v03x_finish_flag)             // 判断摄像头采集完成标志位
        {
            mt9v03x_finish_flag = 0;        // 标志位清零

            user_menu_loop();               // 用户菜单循环函数
        }
        
        cpu_usage_check();                  // 检测CPU占用率
        // 此处编写需要循环执行的代码
    }
}



