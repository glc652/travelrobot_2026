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
#include "fsl_ocotp.h"

// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完

// *************************** 例程硬件连接说明 ***************************
// 使用逐飞科技 CMSIS-DAP | ARM 调试下载器连接
//      直接将下载器正确连接在核心板的调试下载接口即可
// 
// 使用 USB-TTL 模块连接
//      模块管脚            单片机管脚
//      USB-TTL-RX          查看 zf_common_debug.h 文件中 DEBUG_UART_TX_PIN 宏定义的引脚 默认 A9
//      USB-TTL-TX          查看 zf_common_debug.h 文件中 DEBUG_UART_RX_PIN 宏定义的引脚 默认 A10
//      USB-TTL-GND         核心板电源地 GND
//      USB-TTL-3V3         核心板 3V3 电源



// *************************** 例程测试说明 ***************************
// 注意这是不可恢复的操作，请谨慎决定
// 注意这是不可恢复的操作，请谨慎决定
// 注意这是不可恢复的操作，请谨慎决定
// 
// 使用本例程可以直接烧写RT1064的片内保险丝
// 通过烧写保险丝之后，核心板启动时不需要使用外部的boot_cfg引脚，直接使用片内的保险丝状态来进行配置
// 通过这样的方法，避免出现因为使用了boot_cfg引脚，导致出现内核启动时配置了错误的参数导致不启动的问题。
//
// 1.核心板烧录完成本例程，单独使用核心板与调试下载器或者 USB-TTL 模块，在断电情况下完成连接
// 
// 2.将调试下载器或者 USB-TTL 模块连接电脑，完成上电
// 
// 3.电脑上使用串口助手打开对应的串口，串口波特率为 zf_common_debug.h 文件中 DEBUG_UART_BAUDRATE 宏定义 默认 115200，核心板按下复位按键
// 
// 4.可以在串口助手上看到如下串口信息：
//    6000000
//    OCOTP Write operation success!



#define EXAMPLE_OCOTP_FREQ_HZ (CLOCK_GetFreq(kCLOCK_IpgClk))
#define EXAMPLE_OCOTP_FUSE_MAP_ADDRESS 0x06
#define EXAMPLE_OCOTP_FUSE_WRITE_VALUE 0x18

uint8  burn_success;
uint32 fuse_value;

int main(void)
{
    uint32 version;
    
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
    
    // 此处编写用户代码 例如外设初始化代码等

    // 初始化GPIO B9 为输出 默认输出低电平
    OCOTP_Init(OCOTP, EXAMPLE_OCOTP_FREQ_HZ);
	
	version = OCOTP_GetVersion(OCOTP);
	
	printf("%x\r\n",version);
	
	fuse_value = OCOTP_ReadFuseShadowRegister(OCOTP, 0x00); // 读取当前保险丝的状态

    // 此处编写用户代码 例如外设初始化代码等
    
    while(1)
    {
        //此处编写需要循环执行的代码
		if (0x00U == (OCOTP_LOCK_BOOT_CFG_MASK & OCOTP_ReadFuseShadowRegister(OCOTP, 0x00)))
		{
			if (kStatus_Success == OCOTP_WriteFuseShadowRegister(OCOTP, EXAMPLE_OCOTP_FUSE_MAP_ADDRESS, EXAMPLE_OCOTP_FUSE_WRITE_VALUE))
			{
				printf("OCOTP Write operation success!\r\n");
				burn_success = 1; // 烧写成功
			}
			else
			{
				printf("OCOTP write operation failed. Access deny!\r\n");
				burn_success = 0; // 烧写失败
			}
		}
		while(1);
    }
}


