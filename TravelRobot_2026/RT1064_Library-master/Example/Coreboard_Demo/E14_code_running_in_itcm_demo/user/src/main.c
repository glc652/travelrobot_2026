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

//通过使用AT_ITCM_SECTION_ALIGN_INIT(函数定义) 将函数放入ITCM内运行
//具体使用方法参考以下示例
//放在ITCM内的代码不能调用没有放在ITCM内的代码
AT_ITCM_SECTION_INIT(void test(long t))
{
    while(t--);
}

//使用AT_DTCM_SECTION_INIT可以将函数放到DTCM内运行
//放在DTCM内的代码不能调用没有放在DTCM内的代码
//AT_DTCM_SECTION_INIT(void test(long t))
//{
//    while(t--);
//}

//使用AT_OCRAM_SECTION_INIT可以将函数放到OCRAM内运行
//放在OCRAM内的代码不能调用没有放在OCRAM内的代码
//AT_OCRAM_SECTION_INIT(void test(long t))
//{
//    while(t--);
//}

//通过以上方法可以将test函数放到ITCM内运行
//当程序较小的时候可能感受不到放到ITCM的好处，因为有cache加速，当代码较大的时候就会发现有明显的速度优势
//本示例程序仅仅演示如何使用，如果去测试时间可能时间基本是相同的

uint8 gpio_status;
int main(void)
{
    //建议一般选择放入ITCM区域  库设置的DTCM有448KB大小  ITCM 64KB  OCRAM 512KB
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
    
    // 此处编写用户代码 例如外设初始化代码等

    // 初始化GPIO B9 为输出 默认输出低电平
    gpio_init(B9, GPO, 0, GPO_PUSH_PULL);

    // 此处编写用户代码 例如外设初始化代码等
    
    while(1)
    {
        gpio_toggle_level(B9);              // 翻转引脚电平
        test(9999999);              

    }
}


