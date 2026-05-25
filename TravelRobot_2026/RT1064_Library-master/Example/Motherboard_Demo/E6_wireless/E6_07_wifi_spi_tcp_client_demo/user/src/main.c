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
// 使用逐飞科技 CMSIS-DAP | ARM 调试下载器连接
//      直接将下载器正确连接在核心板的调试下载接口即可
// 
// 接入 高速Wifi SPI 模块
//      模块管脚            单片机管脚
//      RST                 查看 zf_device_wifi_spi.h 中 WIFI_SPI_RST_PIN 宏定义
//      INT                 查看 zf_device_wifi_spi.h 中 WIFI_SPI_INT_PIN 宏定义
//      CS                  查看 zf_device_wifi_spi.h 中 WIFI_SPI_CS_PIN 宏定义
//      MISO                查看 zf_device_wifi_spi.h 中 WIFI_SPI_MISO_PIN 宏定义
//      SCK                 查看 zf_device_wifi_spi.h 中 WIFI_SPI_SCK_PIN 宏定义
//      MOSI                查看 zf_device_wifi_spi.h 中 WIFI_SPI_MOSI_PIN 宏定义
//      5V                  5V 电源
//      GND                 电源地


// *************************** 例程使用步骤说明 ***************************
// 1.根据硬件连接说明连接好模块，使用电源供电(下载器供电会导致模块电压不足)
//
// 2.查看电脑所连接的wifi，记录wifi名称，密码，IP地址
//
// 3.在下方的代码区域中修改宏定义，WIFI_SSID_TEST为wifi名称，WIFI_PASSWORD_TEST为wifi密码
//
// 4.打开zf_device_wifi_spi.h，修改WIFI_SPI_TARGET_IP宏定义，设置为wifi的IP地址
//
// 5.下载例程到单片机中，打开串口助手，打开下载器的串口
//
// 6.打开逐飞助手，设置为TCP Server，选择合适的本机地址后点击连接


// *************************** 例程测试说明 ***************************
// 1.本例程会通过 Debug 串口输出测试信息 请务必接好调试串口以便获取测试信息
// 
// 2.连接好模块和核心板后（尽量使用配套主板测试以避免供电不足的问题） 烧录本例程 按下复位后程序开始运行
// 
// 3.如果模块未能正常初始化 会通过 DEBUG 串口输出未能成功初始化的原因 随后程序会尝试重新初始化 一般情况下重试会成功
// 
// 4.如果一直在 Debug 串口输出报错 就需要检查报错内容 并查看本文件下方的常见问题列表进行排查
// 
// 5.程序默认不开启 WIFI_SPI_AUTO_CONNECT 宏定义 通过 main 函数中的接口建立网络链接 如果需要固定自行建立链接 可以开启该宏定义
// 
// 6.当模块初始化完成后会通过 DEBUG 串口输出当前模块的主要信息：固件版本、IP信息、MAC信息、PORT信息
// 
// 7.本例程是 TCP Client 例程 模块会被配置为 TCP Client 需要连接到局域网内的 TCP Server 才能进行通信
//   目标连接的 TCP Server 的 IP 与端口默认使用 zf_device_wifi_spi.h 中 WIFI_SPI_TARGET_IP 与 WIFI_SPI_TARGET_PORT 定义
//   实际测试需要根据现场 TCP Server 的实际 IP 地址与端口设置
// 
// 8.当本机设备成功连接到 TCP Server （例如电脑使用逐飞助手上位机进入 TCP Server 模式 然后本机连接到电脑的 IP 与端口）
//   本例程会先通过调试串口发送链接信息，然后向 Server 发送一段测试数据， 之后可以使用上位机发送数据给设备，设备收到之后会将数据发回给上位机
// 
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

// **************************** 代码区域 ****************************

#define WIFI_SSID_TEST          "SEEKFREE"
#define WIFI_PASSWORD_TEST      "12345678"  // 如果需要连接的WIFI 没有密码则需要将 "12345678" 替换为 NULL


uint8 wifi_spi_test_buffer[] = "this is wifi spi test buffer";
uint8 wifi_spi_get_data_buffer[256];
uint16 data_length;

int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
    
    system_delay_ms(300);
    
    while(wifi_spi_init(WIFI_SSID_TEST, WIFI_PASSWORD_TEST))
    {
		printf("\r\n connect wifi failed. \r\n");
        system_delay_ms(100);                                                   // 初始化失败 等待 100ms
    }
    
    printf("\r\n module version:%s", wifi_spi_version);      					// 模块固件版本
    printf("\r\n module mac    :%s", wifi_spi_mac_addr);     					// 模块 MAC 信息
    printf("\r\n module ip     :%s", wifi_spi_ip_addr_port); 					// 模块 IP 地址

    // zf_device_wifi_spi.h 文件内的宏定义可以更改模块连接(建立) WIFI 之后，是否自动连接 TCP 服务器、创建 UDP 连接
    if(0 == WIFI_SPI_AUTO_CONNECT)                                              // 如果没有开启自动连接 就需要手动连接目标 IP
    {
        while(wifi_spi_socket_connect(                                          // 向指定目标 IP 的端口建立 TCP 连接
            "TCP",                                                              // 指定使用TCP方式通讯
            WIFI_SPI_TARGET_IP,                                                 // 指定远端的IP地址，填写上位机的IP地址
            WIFI_SPI_TARGET_PORT,                                               // 指定远端的端口号，填写上位机的端口号，通常上位机默认是8080
            WIFI_SPI_LOCAL_PORT))                                               // 指定本机的端口号
        {
            // 如果一直建立失败 考虑一下是不是没有接硬件复位
            printf("\r\n Connect TCP Servers error, try again.");
            system_delay_ms(100);                                               // 建立连接失败 等待 100ms
        }
    }
    
	// 使用高速 WIFI SPI模块时无法使用并口屏幕（因为引脚有共用），可以选择使用两寸串口屏幕
    // 使用高速 WIFI SPI模块时无法使用并口屏幕（因为引脚有共用），可以选择使用两寸串口屏幕
    // 使用高速 WIFI SPI模块时无法使用并口屏幕（因为引脚有共用），可以选择使用两寸串口屏幕
    //ips200_init(IPS200_TYPE_SPI);
    
    // 发送测试数据至服务器
    data_length = wifi_spi_send_buffer(wifi_spi_test_buffer, sizeof(wifi_spi_test_buffer));
    if(!data_length)
    {
        printf("\r\n send success.");
    }
    else
    {
        printf("\r\n %d bytes data send failed.", data_length);
    }
    
    while(1)
    {
        data_length = wifi_spi_read_buffer(wifi_spi_get_data_buffer, sizeof(wifi_spi_get_data_buffer));
        if(data_length)                                                  	// 如果接收到数据 则进行数据类型判断
        {
            printf("\r\n Get data: <%s>.", wifi_spi_get_data_buffer);
            if(!wifi_spi_send_buffer(wifi_spi_get_data_buffer, data_length))
            {
                printf("\r\n send success.");
                memset(wifi_spi_get_data_buffer, 0, data_length);          	// 数据发送完成 清空数据
            }
            else
            {
                printf("\r\n %d bytes data send failed.", data_length);
            }
        }
        system_delay_ms(100);
    }
}
// **************************** 代码区域 ****************************

// *************************** 例程常见问题说明 ***************************
// 问题1：串口没有数据
//      查看串口助手打开的是否是正确的串口 检查打开的 COM 口是否对应的是 Debug UART(USB 转 TTL 或者 调试下载器) 的 COM 口
// 
// 问题2：串口数据乱码
//      查看串口助手使用的波特率是否是 zf_common_debug.h 中 DEBUG_UART_BAUDRATE 宏定义的一致 默认是 115200
// 
// 问题3：上位机收不到数据或者一直显示Connect TCP Servers error, try again.
//      检查默认使用的IP 地址（zf_device_wifi_spi.h 中 WIFI_SPI_TARGET_IP）是否与上位机的本机地址一致
//      检查默认使用的通信端口（zf_device_wifi_spi.h 中 WIFI_SPI_TARGET_PORT）是否与上位机的端口号一致
// 
// 问题4：一直显示 connect wifi failed
//      将 WIFI_SSID_TEST 宏定义修改为你测试使用的 Wi-Fi 名称 
//      将 WIFI_PASSWORD_TEST 宏定义修改为你测试使用的 Wi-Fi 密码 