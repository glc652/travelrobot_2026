
#include "zf_common_font.h"
#include "zf_driver_adc.h"
#include "zf_device_key.h"
#include "zf_device_mt9v03x.h"
#include "zf_driver_timer.h"
#include "zf_device_ips200pro.h"
#include "user_menu_device.h"
#include "user_menu.h"

float   test_user_value[9] = {              // 示例参数
        100,
        200,
        3.5,
        40,
        0,
        5,
        5,
        200,
        70,
};

char    test_pid_loop[3][10] = {            // 示例PID环
        "角速度环",
        "角度环",
        "速度环",
};

float   test_pid_value[3][3] = {            // 示例PID参数
        {0.1, 0.2, 0.3},
        {1.1, 1.2, 1.3},
        {2.1, 2.2, 2.3},
};



//-------------------------------------------------------------------------------------------------------------------
// 函数简介          按键 操作处理函数
// 返回类型          void
// 使用示例          user_menu_key_dispose();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void user_menu_key_dispose(void)
{
    if(key_get_state(KEY_1) == KEY_SHORT_PRESS)     // 按键1 短按判断 当前功能：选择更改参数目标
    {
        key_clear_state(KEY_1);                     // 按键1 消除按键状态

        if(menu_switch_page == 0)                   // 若当前界面为 实时参数界面 则切换到 设置参数界面
        {
            menu_switch_page = 1;                   // 菜单界面 更改为 设置参数界面

            action_switch_flag = 1;                 // 界面切换动画标志位 置位

            value_switch_num = 1;                   // 设置参数选择 更改为 选择第一个参数
        }
        else if(menu_switch_page == 1)              // 若已经在设置参数界面 则按键功能更改为 切换参数选择
        {
            if(++ value_switch_num > USER_MENU_VALUE_TABLE_NUM)     // 选择自增 超过最大值则回到第一个位置
            {
                value_switch_num = 1;
            }
        }

        table_action_switch_flag = 1;               // 设置参数表格动画标志位 置位

    }

    if(key_get_state(KEY_2) == KEY_SHORT_PRESS)     // 按键2 短按判断 当前功能：当前选择参数 数值增加
    {
        key_clear_state(KEY_2);                     // 按键2 消除按键状态

        if(menu_switch_page == 1)
        {
            // 选择不同的目标参数 执行不同的加减数量 用户可替换为自己的参数
            switch(value_switch_num)
            {
                case 1: test_user_value[0] += 1;    break;
                case 2: test_user_value[1] += 1;    break;
                case 3: test_user_value[2] += 0.1;  break;
                case 4: test_user_value[3] += 3;    break;
                case 5: test_user_value[4] += 3;    break;
                case 6: test_user_value[5] += 3;    break;
                case 7: test_user_value[6] += 3;    break;
                case 8: test_user_value[7] += 3;    break;
                case 9: test_user_value[8] += 3;    break;
                case 10:
                {
                    if(++ pid_switch_num >= 3)
                    {
                        pid_switch_num = 0;
                    }
                }break;
                case 11:test_pid_value[pid_switch_num][0] += 0.1; break;
                case 12:test_pid_value[pid_switch_num][1] += 0.1; break;
                case 13:test_pid_value[pid_switch_num][2] += 0.1; break;
                default:break;
            }
        }

        value_refresh_flag = 1;

    }

    if(key_get_state(KEY_3) == KEY_SHORT_PRESS)     // 按键3 短按判断 当前功能：当前选择参数 数值减少
    {
        key_clear_state(KEY_3);                     // 按键3 消除按键状态

        if(menu_switch_page == 1)
        {
            // 选择不同的目标参数 执行不同的加减数量 用户可替换为自己的参数
            switch(value_switch_num)
            {
                case 1: test_user_value[0] -= 1;    break;
                case 2: test_user_value[1] -= 1;    break;
                case 3: test_user_value[2] -= 0.1;  break;
                case 4: test_user_value[3] -= 2;    break;
                case 5: test_user_value[4] -= 2;    break;
                case 6: test_user_value[5] -= 2;    break;
                case 7: test_user_value[6] -= 2;    break;
                case 8: test_user_value[7] -= 2;    break;
                case 9: test_user_value[8] -= 2;    break;
                case 10:
                {
                    if(-- pid_switch_num < 0)
                     {
                        pid_switch_num = 2;
                     }
                }break;
                case 11:test_pid_value[pid_switch_num][0] -= 0.1; break;
                case 12:test_pid_value[pid_switch_num][1] -= 0.1; break;
                case 13:test_pid_value[pid_switch_num][2] -= 0.1; break;
                default:break;
            }
        }

        value_refresh_flag = 1;

    }

    if(key_get_state(KEY_4) == KEY_SHORT_PRESS)     // 按键4 短按判断
    {
        key_clear_state(KEY_4);                     // 按键4 消除按键状态

        if(menu_switch_page == 0)                   // 若当前在实时参数界面 则直接发车
        {

            // 发车！！！！
            // 实现程序由用户自行编写

        }
        else if(menu_switch_page == 1)              // 若当前在设置参数界面 则返回实时数据界面
        {
            menu_switch_page = 0;                   // 菜单界面 更改为 实时数据界面

            action_switch_flag = 1;                 // 界面切换动画标志位 置位
        }

    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介          用户菜单 设置参数名称显示 函数
// 返回类型          void
// 使用示例          user_menu_display_set_value();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void user_menu_display_set_value_name(void)
{
    // 以下为示例内容 模拟用户需要设置的参数名称

    ips200pro_table_cell_printf(widgets_table_id[0], 1,  1, "目标速度");     // 第一行 第一列 显示内容为 "目标速度"

    ips200pro_table_cell_printf(widgets_table_id[0], 2,  1, "目标高度");     // 第二行 第一列 显示内容为 "目标高度"

    ips200pro_table_cell_printf(widgets_table_id[0], 3,  1, "目标倾斜度");

    ips200pro_table_cell_printf(widgets_table_id[0], 4,  1, "转向P1量");

    ips200pro_table_cell_printf(widgets_table_id[0], 5,  1, "转向P2量");

    ips200pro_table_cell_printf(widgets_table_id[0], 6,  1, "转向D量");

    ips200pro_table_cell_printf(widgets_table_id[0], 7,  1, "陀螺仪D量");

    ips200pro_table_cell_printf(widgets_table_id[0], 8,  1, "俯仰零点");

    ips200pro_table_cell_printf(widgets_table_id[0], 9,  1, "横滚零点");

    ips200pro_table_cell_printf(widgets_table_id[0], 10, 1, "PID环选择");

    ips200pro_table_cell_printf(widgets_table_id[0], 11, 1, "当前P量");

    ips200pro_table_cell_printf(widgets_table_id[0], 12, 1, "当前I量");

    ips200pro_table_cell_printf(widgets_table_id[0], 13, 1, "当前D量");
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介          用户菜单 设置参数显示 函数
// 返回类型          void
// 使用示例          user_menu_display_set_value();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void user_menu_display_set_value(void)
{
    // 以下为示例内容 模拟用户需要设置的参数名称
    ips200pro_table_cell_printf(widgets_table_id[0], 1,  2, "%.1f", test_user_value[0]);     // 第一行 第二列 输出 目标速度

    ips200pro_table_cell_printf(widgets_table_id[0], 2,  2, "%.1f", test_user_value[1]);     // 第二行 第二列 输出 目标高度

    ips200pro_table_cell_printf(widgets_table_id[0], 3,  2, "%.1f", test_user_value[2]);     // 输出 目标倾斜度

    ips200pro_table_cell_printf(widgets_table_id[0], 4,  2, "%.1f", test_user_value[3]);     // 输出 转向P1量

    ips200pro_table_cell_printf(widgets_table_id[0], 5,  2, "%.1f", test_user_value[4]);     // 输出 转向P2量

    ips200pro_table_cell_printf(widgets_table_id[0], 6,  2, "%.1f", test_user_value[5]);     // 输出 转向D量

    ips200pro_table_cell_printf(widgets_table_id[0], 7,  2, "%.1f", test_user_value[6]);     // 输出 陀螺仪D量

    ips200pro_table_cell_printf(widgets_table_id[0], 8,  2, "%.1f", test_user_value[7]);     // 输出 俯仰零点

    ips200pro_table_cell_printf(widgets_table_id[0], 9,  2, "%.1f", test_user_value[8]);     // 输出 横滚零点

    ips200pro_table_cell_printf(widgets_table_id[0], 10, 2, "%s"  , test_pid_loop[pid_switch_num]);          // 输出 PID环选择

    ips200pro_table_cell_printf(widgets_table_id[0], 11, 2, "%.1f", test_pid_value[pid_switch_num][0]);      // 输出 当前P量

    ips200pro_table_cell_printf(widgets_table_id[0], 12, 2, "%.1f", test_pid_value[pid_switch_num][1]);      // 输出 当前I量

    ips200pro_table_cell_printf(widgets_table_id[0], 13, 2, "%.1f", test_pid_value[pid_switch_num][2]);      // 输出 当前D量
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介          用户菜单 实时数据名称显示 函数
// 返回类型          void
// 使用示例          user_menu_display_realtime_value_name();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void user_menu_display_realtime_value_name(void)
{
    // 以下为示例内容 模拟用户需要查看的实时参数名称

    ips200pro_table_cell_printf(widgets_table_id[1], 1,  1, "白点参考值");    // 第一行 第一列 显示内容为 "白点参考值"

    ips200pro_table_cell_printf(widgets_table_id[1], 2,  1, "图像远端距离");   // 第二行 第一列 显示内容为 "图像远端距离"

    ips200pro_table_cell_printf(widgets_table_id[1], 3,  1, "整体图像偏差");

    ips200pro_table_cell_printf(widgets_table_id[1], 4,  1, "图像偏差微分");

    ips200pro_table_cell_printf(widgets_table_id[1], 5,  1, "转角值输出");

    ips200pro_table_cell_printf(widgets_table_id[1], 6,  1, "图像处理耗时");
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介          用户菜单 实时数据显示 函数
// 返回类型          void
// 使用示例          user_menu_display_realtime_value();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void user_menu_display_realtime_value(void)
{
    // 以下为示例内容 模拟用户需要查看的实时参数数据

    ips200pro_table_cell_printf(widgets_table_id[1], 1,  2, "123");    // 第一行 第一列 显示内容为 "白点参考值"

    ips200pro_table_cell_printf(widgets_table_id[1], 2,  2, "456");    // 第二行 第二列 显示内容为 "图像远端距离"

    ips200pro_table_cell_printf(widgets_table_id[1], 3,  2, "789");

    ips200pro_table_cell_printf(widgets_table_id[1], 4,  2, "3.14");

    ips200pro_table_cell_printf(widgets_table_id[1], 5,  2, "%.2f", 4.56);

    ips200pro_table_cell_printf(widgets_table_id[1], 6,  2, "0x%X", 0xABCD);
}




//-------------------------------------------------------------------------------------------------------------------
// 函数简介          用户菜单循环函数
// 返回类型          void
// 使用示例          user_menu_loop();
// 备注信息          如果增加了图像显示，建议将菜单循环函数放在图像标志位判断语句内，可实现稳定50帧刷新率
//-------------------------------------------------------------------------------------------------------------------
void user_menu_loop(void)
{
    key_scanner();                  // 按键扫描

    user_menu_key_dispose();        // 按键处理

    user_menu_action_loop();

    user_menu_table_action_loop();

    if(menu_switch_page == 0)
    {
        user_menu_display_realtime_value();

        ips200pro_set_color(widgets_progress_bar_id[0], COLOR_FOREGROUND, progress_bar_status_color[(uint8)battery_percentage_filter]);  // 设置电量条颜色
        ips200pro_progress_bar_set_value(widgets_progress_bar_id[0], 0, (uint8)battery_percentage_filter);                               // 设置电量百分比条
        ips200pro_label_printf(widgets_label_id[0], "%d", (uint8)battery_percentage_filter);                                             // 输出电量百分比数值

        ips200pro_set_color(widgets_progress_bar_id[1], COLOR_FOREGROUND, progress_bar_status_color[(100 - (uint8)cpu_usage_percentage_filter)]);    // 设置CPU占有率颜色
        ips200pro_progress_bar_set_value(widgets_progress_bar_id[1], 0, (uint8)cpu_usage_percentage_filter);                             // 设置CPU占有率百分比条
        ips200pro_label_printf(widgets_label_id[2], "%d", (uint8)cpu_usage_percentage_filter);                                           // 输出CPU占有率百分比数值
    }

    // 最后输出图像（如果先输出图像 屏幕会去绘制图像 导致后续的响应滞后 从而增加耗时）
    ips200pro_image_display(widgets_image_id, mt9v03x_image[0], 188, 120, IMAGE_GRAYSCALE, 0);                                       // 输出图像
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介          用户菜单初始化
// 返回类型          void
// 使用示例          user_menu_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void user_menu_init(void)
{
    widgets_page_id[0]          = ips200pro_init("用户菜单", IPS200PRO_TITLE_TOP, 0);   // 创建界面 用户菜单  title_size传参为0 代表不显示title


    widgets_table_id[0]         = ips200pro_table_create(0, 133, USER_MENU_VALUE_TABLE_NUM, 2);        // 创建第一个 表格控件 用于显示各种参数
    ips200pro_set_hidden(widgets_table_id[0], 1);                                    // 第一个表格控件隐藏 后续将通过动画切换显示
    ips200pro_set_color(widgets_table_id[0], COLOR_BORDER,     TABLE_DEFAULT_BORDER_COLOR);            // 配置第一个表格控件 边线颜色 为 默认边线颜色
    ips200pro_set_color(widgets_table_id[0], COLOR_FOREGROUND, TABLE_DEFAULT_FOREGROUND_COLOR);        // 配置第一个表格控件 前景(字体)颜色 为 默认前景颜色
    ips200pro_set_color(widgets_table_id[0], COLOR_BACKGROUND, TABLE_DEFAULT_BACKGROUND_COLOR);        // 配置第一个表格控件 背景颜色为 默认背景颜色
    user_menu_display_set_value_name();                                             // 第一个表格控件填充 参数名称 数据
    user_menu_display_set_value();                                                  // 第一个表格控件填充 参数 数据


    widgets_table_id[1]         = ips200pro_table_create(0, 173, USER_MENU_REALTIME_VALUE_TABLE_NUM, 2);// 创建第二个 表格控件 用于显示各种实时数据
    ips200pro_set_color(widgets_table_id[1], COLOR_BORDER,     TABLE_DEFAULT_BORDER_COLOR);            // 配置第二个表格控件 边线颜色 为 默认边线颜色
    ips200pro_set_color(widgets_table_id[1], COLOR_FOREGROUND, TABLE_DEFAULT_FOREGROUND_COLOR);        // 配置第二个表格控件 前景(字体)颜色 为 默认前景颜色
    ips200pro_set_color(widgets_table_id[1], COLOR_BACKGROUND, TABLE_DEFAULT_BACKGROUND_COLOR);        // 配置第二个表格控件 背景颜色为 默认背景颜色
    user_menu_display_realtime_value_name();                                        // 第二个表格控件填充 实时数据名称 数据
    user_menu_display_realtime_value();                                             // 第二个表格控件填充 实时数据 数据


    widgets_container_id = ips200pro_container_create(0, 0, 240, 173);               // 创建容器控件 用于掩盖表格
    ips200pro_set_color(widgets_container_id, COLOR_BORDER, RGB565_WHITE);           // 配置容器控件 边线颜色 为 白色


    widgets_progress_bar_id[0]  = ips200pro_progress_bar_create(0, 0, 240, 20);      // 创建第一个进度条控件 用于显示 电池电量 条
    widgets_label_id[0]         = ips200pro_label_create(2, 2, 20, 20);              // 创建第一个标签控件  用于显示 电池电量 数值
    ips200pro_label_printf(widgets_label_id[0], "0");                                // 第一个标签控件显示初始内容
    widgets_label_id[1]         = ips200pro_label_create(176, 2, 64, 20);            // 创建第二个标签控件  用于显示 电量 字符
    ips200pro_label_printf(widgets_label_id[1], "电池电量");                            // 第二个标签控件显示初始内容


    widgets_progress_bar_id[1]  = ips200pro_progress_bar_create(0, 22, 240, 20);     // 创建第二个进度条控件 用于显示 CPU占用率 条
    widgets_label_id[2]         = ips200pro_label_create(2, 22, 20, 20);             // 创建第三个标签控件  用于显示 CPU占用率 数值
    ips200pro_label_printf(widgets_label_id[2], "0");                                // 第三个标签控件显示初始内容
    widgets_label_id[3]         = ips200pro_label_create(176, 22, 64, 20);           // 创建第四个标签控件  用于显示 CPU 字符
    ips200pro_label_printf(widgets_label_id[3], "CPU占用");                            // 第四个标签控件显示初始内容


    widgets_image_id            = ips200pro_image_create(26, 44, 188, 120);          // 创建图像控件 用于显示总钻风灰度图像
}



