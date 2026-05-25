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

// *************************** 例程测试说明 ***************************
//本例程将详细解释变量的存放位置以及如何指定变量存在我们想要的RAM中

uint8 a;        //未赋值的全局变量 存放在DTCM里面
uint8 b=1;      //赋值的全局变量   放在DTCM里面



AT_DTCM_SECTION(uint8 c);          		        //指定变量存放在DTCM区域
AT_DTCM_SECTION_ALIGN(uint8 d,4);  		        //指定变量存放在DTCM区域  并且4字节对齐

AT_OCRAM_SECTION(uint8 e);                      //指定变量存放在OCRAM区域
AT_OCRAM_SECTION_ALIGN(uint8 f,4);              //指定变量存放在OCRAM区域  并且4字节对齐

AT_SDRAM_SECTION(uint8 g);                      //指定变量存放在SDRAM区域
AT_SDRAM_SECTION_ALIGN(uint8 h,4);              //指定变量存放在SDRAM区域  并且4字节对齐

AT_SDRAM_NONCACHE_SECTION(uint8 i);             //指定变量存放在SDRAM noncache区域
AT_SDRAM_NONCACHE_SECTION_ALIGN(uint8 j,4);     //指定变量存放在SDRAM noncache区域  并且4字节对齐

int main(void)
{
    //在函数内部定义的变量以及 函数的参数都是存在栈区域的，栈是放在了DTCM里
    //因此这些变量都是放在DTCM的
    uint8 x;    //此变量存放在DTCM里面
    
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
    
    // 此处编写用户代码 例如外设初始化代码等


    // 此处编写用户代码 例如外设初始化代码等
    
    x=0;
    while(1)
    {
        c++;
        d++;
        e++;
        f++;
        g++;
        h++;
        i++;
        j++;
        x++;
    }
}


