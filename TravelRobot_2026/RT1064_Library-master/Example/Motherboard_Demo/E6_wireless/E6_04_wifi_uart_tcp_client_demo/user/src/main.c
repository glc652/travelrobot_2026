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
// 接入 Wi-Fi 转串口模块
//      模块管脚            单片机管脚
//      RX                  查看 zf_device_wifi_uart.h 中 WRIELESS_UART_RX_PINx 宏定义    默认 D16
//      TX                  查看 zf_device_wifi_uart.h 中 WRIELESS_UART_TX_PINx 宏定义    默认 D17
//      RTS                 查看 zf_device_wifi_uart.h 中 WRIELESS_UART_RTS_PINx 宏定义   默认 D26
//      RST                 查看 zf_device_wifi_uart.h 中 WRIELESS_UART_RST_PINx 宏定义   默认 B9
//      VCC                 5V 电源
//      GND                 电源地

// *************************** 例程使用步骤说明 ***************************
//1.根据硬件连接说明连接好模块，使用电源供电(下载器供电会导致模块电压不足)
//
//2.查看电脑所连接的wifi，记录wifi名称，密码，IP地址
//
//3.在下方的代码区域中修改宏定义，WIFI_SSID_TEST为wifi名称，WIFI_PASSWORD_TEST为wifi密码
//
//4.打开zf_device_wifi_uart.h，修改WIFI_UART_TARGET_IP宏定义，设置为wifi的IP地址
//
//5.下载例程到单片机中，打开逐飞助手上位机，打开下载器的串口
//
//6.打开逐飞助手上位机，设置为TCP Sever服务，并远程连接到模块的IP地址


// *************************** 例程测试说明 ***************************
// 1.本例程会通过 Debug 串口输出测试信息 请务必接好调试串口以便获取测试信息
// 
// 2.电脑上使用逐飞助手上位机打开对应的串口，串口波特率为 zf_common_debug.h 文件中 DEBUG_UART_BAUDRATE 宏定义 默认 115200
//
// 3.连接好模块和核心板后（尽量使用配套主板测试以避免供电不足的问题） 烧录本例程 按下复位后程序开始运行
// 
// 4.如果模块未能正常初始化 会通过 DEBUG 串口输出未能成功初始化的原因 随后程序会尝试重新初始化 一般情况下重试会成功
// 
// 5.如果一直在 Debug 串口输出报错 就需要检查报错内容 并查看本文件下方的常见问题列表进行排查
// 
// 6.程序默认不开启 WIFI_UART_AUTO_CONNECT 宏定义 通过 main 函数中的接口建立网络链接 如果需要固定自行建立链接 可以开启该宏定义
// 
// 7.当模块初始化完成后会通过 DEBUG 串口输出当前模块的主要信息：固件版本、IP信息、MAC信息、PORT信息
// 
// 8.本例程是 TCP Client 例程 模块会被配置为 TCP Client 需要连接到局域网内的 TCP Server 才能进行通信
//   目标连接的 TCP Server 的 IP 与端口默认使用 zf_device_wifi_uart.h 中 WIFI_UART_TARGET_IP 与 WIFI_UART_TARGET_PORT 定义
//   实际测试需要根据现场 TCP Server 的实际 IP 地址与端口设置
// 
// 9.当本机设备主动连接到 TCP Server （例如电脑使打开另一个逐飞助手上位机进入 TCP Server 模式 然后本机连接到电脑的 IP 与端口）
//   本例程会先行向 Server 发送一段测试数据 随后在收到数据后会在 Debug 串口输出信息以及当前链接信息 并将数据主动发送回去
// 
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

// **************************** 代码区域 ****************************
#define WIFI_SSID_TEST          "SEEKFREE"
#define WIFI_PASSWORD_TEST      "12345678"

uint8 wifi_uart_test_buffer[] = "this is wifi uart test buffer";
uint8 wifi_uart_get_data_buffer[256];
uint16 data_length;

int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
        
    system_delay_ms(300);           //等待主板其他外设上电完成
    
    printf("\r\n Wi-Fi UART init.");

    // 当前使用的是 WIFI 串口模块的 station 模式 可以用于连接路由器 WIFI
    // 
    // 发送数据方式可以使用命令发送(默认) 也可以使用透传发送 可以在 wifi_uart_init 函数内修改 <当模块作为服务器时 只能使用命令方式发送数据>
    while(wifi_uart_init(WIFI_SSID_TEST, WIFI_PASSWORD_TEST, WIFI_UART_STATION))
    {
        system_delay_ms(500);                                                   // 初始化失败 等待 500ms
    }

    // zf_device_wifi_uart.h 文件内的宏定义可以更改模块连接(建立) WIFI 之后，是否自动连接 TCP 服务器、创建 UDP 连接、创建 TCP 服务器等操作
    if(1 != WIFI_UART_AUTO_CONNECT)                                             // 如果没有开启自动连接 就需要手动连接目标 IP
    {
        while(wifi_uart_connect_tcp_servers(                                    // 向指定目标 IP 的 TCP Server 端口建立 TCP 连接
            WIFI_UART_TARGET_IP,                                                // 这里使用与自动连接时一样的目标 IP 实际使用时也可以直接填写目标 IP 字符串
            WIFI_UART_TARGET_PORT,                                              // 这里使用与自动连接时一样的目标端口 实际使用时也可以直接填写目标端口字符串
            WIFI_UART_COMMAND))                                                 // 采用命令传输模式 当然你可以改成透传模式 实际上差别并不是很大
        {
            // 如果一直建立失败 考虑一下是不是没有接硬件复位
            printf("\r\n Connect TCP Servers error, try again.");
            system_delay_ms(500);                                               // 建立连接失败 等待 500ms
        }
    }

    printf("\r\nmodule version:%s", wifi_uart_information.wifi_uart_version);   // 模块固件版本
    printf("\r\nmodule mac    :%s", wifi_uart_information.wifi_uart_mac);       // 模块 MAC 信息
    printf("\r\nmodule ip     :%s", wifi_uart_information.wifi_uart_local_ip);  // 模块 IP 地址
    printf("\r\nmodule port   :%s", wifi_uart_information.wifi_uart_local_port);// 模块 PORT 信息

    // 发送测试数据至服务器
    data_length = wifi_uart_send_buffer(wifi_uart_test_buffer, sizeof(wifi_uart_test_buffer));
    if(!data_length)
    {
        printf("\r\n send success.");
    }
    else
    {
        printf("\r\n %d bytes data send failed.", data_length);
    }
    interrupt_global_enable(0);
    
    while(1)
    {
        data_length = wifi_uart_read_buffer(wifi_uart_get_data_buffer, sizeof(wifi_uart_get_data_buffer));

        if(data_length) 												        // 如果接收到数据 则进行数据类型判断
        {
            if(strstr((char *)wifi_uart_get_data_buffer, "+IPD"))  		        // 判断数据格式是否是通过网络发送过来的数据
            {
                printf("\r\n Get data: <%s>.", wifi_uart_get_data_buffer);
                if(!wifi_uart_send_buffer(wifi_uart_get_data_buffer, data_length))
                {
                    printf("\r\n send success.");
                    memset(wifi_uart_get_data_buffer, 0, data_length);		    // 数据发送完成 清空数据
                }
                else
                {
                    printf("\r\n %d bytes data send failed.", data_length);
                }
            }
        }
        system_delay_ms(100);
    }
}
// **************************** 代码区域 ****************************

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 
// 问题1：串口没有数据
//      查看逐飞助手上位机打开的是否是正确的串口 检查打开的 COM 口是否对应的是 Debug UART(USB 转 TTL 或者 调试下载器) 的 COM 口
// 
// 问题2：串口数据乱码
//      查看逐飞助手上位机使用的波特率是否是 zf_common_debug.h 中 DEBUG_UART_BAUDRATE 宏定义的一致 默认是 115200
// 
// 问题3：一直显示 reset failed 信息
//      默认使用硬件引脚复位 如果没有接 WIFI_UART_RST_PIN 对应的复位引脚 就需要接上复位引脚 或者修改 WIFI_UART_HARDWARE_RST 为禁用
// 
// 问题4：一直显示 Connect TCP Servers error, try again. 信息
//      检查默认使用的 TCP Servers IP 地址（zf_device_wifi_uart.h 中 WIFI_UART_TARGET_IP）是否与测试使用的 TCP Servers IP 一致
//      检查默认使用的 TCP Servers 通信端口（zf_device_wifi_uart.h 中 WIFI_UART_TARGET_PORT）是否与测试使用的 TCP Servers 通信端口一致
// 
// 问题5：一直显示 wifi set failed 或者 get module information failed 信息
//      将 WIFI_SSID_TEST 宏定义修改为你测试使用的 Wi-Fi 名称 
//      将 WIFI_PASSWORD_TEST 宏定义修改为你测试使用的 Wi-Fi 密码 


