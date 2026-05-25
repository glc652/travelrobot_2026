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
// 查看核心板说明书，将核心板的SD卡相关跳线连接起来
//
// 使用逐飞科技 CMSIS-DAP | ARM 调试下载器连接
//      直接将下载器正确连接在核心板的调试下载接口即可
// 
// 使用 USB-TTL 模块连接
//      模块管脚            单片机管脚
//      USB-TTL-RX          查看 zf_common_debug.h 文件中 DEBUG_UART_TX_PIN 宏定义的引脚 默认 B12
//      USB-TTL-TX          查看 zf_common_debug.h 文件中 DEBUG_UART_RX_PIN 宏定义的引脚 默认 B13
//      USB-TTL-GND         核心板电源地 GND
//      USB-TTL-3V3         核心板 3V3 电源

// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程，单独使用核心板与调试下载器或者 USB-TTL 模块，在断电情况下完成连接
// 
// 2.将调试下载器或者 USB-TTL 模块连接电脑，完成上电
// 
// 3.电脑上使用串口助手打开对应的串口，串口波特率为 zf_common_debug.h 文件中 DEBUG_UART_BAUDRATE 宏定义 默认 115200，核心板按下复位按键
// 
// 4.可以在串口助手上看到如下串口信息：
//    FATFS 文件系统示例程序

//    请插入SD卡

//    创建文件系统中。。。这可能需要一个比较长的时间
// 
// 
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查


#define BUFFER_SIZE (512U)


static FATFS g_fileSystem; // 文件系统结构体变量
static FIL g_fileObject;   // 文件结构体变量


// 写入缓冲区
AT_DTCM_SECTION_ALIGN(uint8_t g_bufferWrite[SDK_SIZEALIGN(BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_DESCRIPTOR_BUFFER_ALIGN_SIZE));

// 读取缓冲区
AT_DTCM_SECTION_ALIGN(uint8_t g_bufferRead[SDK_SIZEALIGN(BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)], MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_DESCRIPTOR_BUFFER_ALIGN_SIZE));



//SD卡需要使用的引脚有 B23 C30 D4 D12 D13 D14 D15 D16 D17
//如果核心板插在主板上的请确认这些端口是没有连接其他外设（确认拨码开关处于OFF 且没有插  舵机、运放模块、2寸IPS并口屏幕、无线转串口模块）
//否则有可能导致例程无法使用


//在使用前请将核心板的J1 J3跳线焊接起来。
uint32 temp;
int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
    
    // 此处编写用户代码 例如外设初始化代码等

    // 此处编写用户代码 例如外设初始化代码等
    
    FRESULT error;
    DIR directory;                  // 目录结构体变量
    FILINFO fileInformation;
    UINT bytesWritten;
    UINT bytesRead;
    const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
    volatile bool failedFlag = false;
    char ch = '0';
    BYTE work[FF_MAX_SS];
   
    printf("\r\nFATFS 文件系统示例程序\r\n");
    
    printf("\r\n请插入SD卡\r\n");
	sdio_init();                    // 初始化SDIO接口并等待SD卡插入

    if (f_mount(&g_fileSystem, driverNumberBuffer, 0U))
    {
        printf("挂载失败\r\n");
        return -1;
    }

#if (FF_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
    if (error)
    {
        printf("改变驱动器失败\r\n");
        return -1;
    }
#endif

#if FF_USE_MKFS
    printf("\r\n创建文件系统中。。。这可能需要一个比较长的时间\r\n");
	// 当SD卡文件系统创建完毕之后，就不需要在每次都创建了
	// 因为每次创建都是会把SD卡格式化一遍的，每次都格式化容易导致卡报废
    if (f_mkfs(driverNumberBuffer, 0U, work, sizeof(work)))
    {
        printf("创建文件系统失败\r\n");
        return -1;
    }
#endif /* FF_USE_MKFS */
    
    printf("\r\n创建目录中\r\n");
    error = f_mkdir(_T("/dir_1"));
    if (error)
    {
        if (error == FR_EXIST)
        {
            printf("目录已存在\r\n");
        }
        else
        {
            printf("目录创建失败\r\n");
            return -1;
        }
    }

    printf("\r\n创建一个文件在当前目录下\r\n");
    error = f_open(&g_fileObject, _T("/dir_1/f_1.dat"), (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
    if (error)
    {
        if (error == FR_EXIST)
        {
            printf("文件已存在\r\n");
        }
        else
        {
            printf("创建文件失败\r\n");
            return -1;
        }
    }

    printf("\r\n在当前目录中，再创建一个目录\r\n");
    error = f_mkdir(_T("/dir_1/dir_2"));
    if (error)
    {
        if (error == FR_EXIST)
        {
            printf("目录以存在\r\n");
        }
        else
        {
            printf("目录创建失败\r\n");
            return -1;
        }
    }

    printf("\r\n列出该目录中的文件\r\n");
    if (f_opendir(&directory, "/dir_1"))
    {
        printf("目录打开失败\r\n");
        return -1;
    }

    for (;;)
    {
        error = f_readdir(&directory, &fileInformation);

        /* To the end. */
        if ((error != FR_OK) || (fileInformation.fname[0U] == 0U))
        {
            break;
        }
        if (fileInformation.fname[0] == '.')
        {
            continue;
        }
        if (fileInformation.fattrib & AM_DIR)
        {
            printf("文件夹 : %s.\r\n", fileInformation.fname);
        }
        else
        {
            printf("普通文件 : %s.\r\n", fileInformation.fname);
        }
    }

    memset(g_bufferWrite, 'a', sizeof(g_bufferWrite));
    g_bufferWrite[BUFFER_SIZE - 2U] = '\r';
    g_bufferWrite[BUFFER_SIZE - 1U] = '\n';

    printf("\r\n写入/读取文件，直到遇到错误\r\n");
    while (true)
    {
        if (failedFlag || (ch == 'q'))
        {
            break;
        }

        printf("\r\n写入数据到上面创建的文件\r\n");
        error = f_write(&g_fileObject, g_bufferWrite, sizeof(g_bufferWrite), &bytesWritten);
        if ((error) || (bytesWritten != sizeof(g_bufferWrite)))
        {
            printf("写入文件失败 \r\n");
            failedFlag = true;
            continue;
        }

        /* Move the file pointer */
        if (f_lseek(&g_fileObject, 0U))
        {
            printf("设置文件指针位置失败\r\n");
            failedFlag = true;
            continue;
        }

        printf("读取上面创建的文件\r\n");
        memset(g_bufferRead, 0U, sizeof(g_bufferRead));
        error = f_read(&g_fileObject, g_bufferRead, sizeof(g_bufferRead), &bytesRead);
        if ((error) || (bytesRead != sizeof(g_bufferRead)))
        {
            printf("读取文件失败\r\n");
            failedFlag = true;
            continue;
        }

        printf("开始比较写入与读取数据是否一致\r\n");
        if (memcmp(g_bufferWrite, g_bufferRead, sizeof(g_bufferWrite)))
        {
            printf("写入与读取数据不一致\r\n");
            failedFlag = true;
            continue;
        }
        printf("写入与读取数据一致\r\n");

        printf("\r\n输入q退出程序 输入其他重复执行写入与读取\r\n");
        ch = getchar();
        putchar(ch);
    }
    printf("\r\n示例程序不会再进行读取操作\r\n");

    if (f_close(&g_fileObject))
    {
        printf("\r\n关闭文件失败\r\n");
        return -1;
    }

    while (true)
    {
    }
}

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 
// 问题1：一直在创建文件系统
//      这个过程可能比较久，建议等到30秒
//
// 问题2：一直显示请插入SD卡
//      可能SD卡没插好
