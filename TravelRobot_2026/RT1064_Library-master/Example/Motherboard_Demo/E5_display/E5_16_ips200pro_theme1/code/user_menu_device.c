#include "zf_common_headfile.h"
#include "math.h"
#include "user_menu.h"
#include "user_menu_device.h"

uint16  widgets_page_id[2];                 // 分页组件ID
uint16  widgets_label_id[4];                // 标签组件ID
uint16  widgets_table_id[2];                // 表格组件ID
uint16  widgets_image_id;                   // 图像组件ID
uint16  widgets_progress_bar_id[2];         // 进度条组件ID
uint16  widgets_container_id;               // 容器组件ID

uint8   menu_switch_page;                   // 当前选择分页
uint8   action_switch_flag;                 // 动画切换flag
int     action_switch_step;                 // 动画切换步骤
int     action_switch_count;                // 动画切换计数
uint8   table_action_switch_flag;           // 表格动画切换flag
int16   table_action_offset;                // 表格控件动画偏移值
int16   table_action_offset_filter;         // 表格控件动画偏移值（滤波）
int16   value_switch_num;                   // 当前选择参数
int16   pid_switch_num;                     // 当前 PID 选择修改参数
int16   value_refresh_flag;                 // 参数刷新标志位

float   battery_voltage;                    // 电池电压
float   battery_percentage;                 // 电池电压百分比
float   battery_percentage_filter;          // 电池电压百分比（滤波）

int32   cpu_usage_check_count;              // CPU占有率计数
float   cpu_usage_percentage;               // CPU占有率百分比
float   cpu_usage_percentage_filter;        // CPU占有率百分比（滤波）

// 进度条不同状态颜色数组
const uint16 progress_bar_status_color[100] = {
        0xF800, 0xF000, 0xF020, 0xF020, 0xE840, 0xE860, 0xE860, 0xE080, 0xE0A0, 0xE0A0,
        0xD8C0, 0xD8C0, 0xD8E0, 0xD100, 0xD100, 0xD120, 0xD140, 0xC940, 0xC960, 0xC960,
        0xC180, 0xC1A0, 0xC1A0, 0xB9C0, 0xB9E0, 0xB9E0, 0xB200, 0xB220, 0xB220, 0xB240,
        0xAA40, 0xAA60, 0xAA80, 0xA280, 0xA2A0, 0xA2C0, 0x9AC0, 0x9AE0, 0x9AE0, 0x9300,
        0x9320, 0x9320, 0x8B40, 0x8B60, 0x8B60, 0x8B80, 0x8380, 0x83A0, 0x83C0, 0x7BC0,
        0x7BE0, 0x7C00, 0x7400, 0x7420, 0x7440, 0x6C40, 0x6C60, 0x6C60, 0x6C80, 0x64A0,
        0x64A0, 0x64C0, 0x5CE0, 0x5CE0, 0x5D00, 0x5500, 0x5520, 0x5540, 0x4D40, 0x4D60,
        0x4D80, 0x4580, 0x45A0, 0x45A0, 0x45C0, 0x3DE0, 0x3DE0, 0x3E00, 0x3620, 0x3620,
        0x3640, 0x2E60, 0x2E60, 0x2E80, 0x2680, 0x26A0, 0x26C0, 0x26C0, 0x1EE0, 0x1F00,
        0x1F00, 0x1720, 0x1720, 0x1740, 0x0F60, 0x0F60, 0x0F80, 0x07A0, 0x07A0, 0x07C0,
};

//-------------------------------------------------------------------------------------------------------------------
// 函数简介          组件分步退出
// 返回类型          void
// 使用示例          widgets_edged_out(action_switch_count, 10, TABLE_DEFAULT_BORDER_COLOR);
// 备注信息          内部动画调用 原理：从 固定颜色 分步骤 逐渐变化到 白色
//-------------------------------------------------------------------------------------------------------------------
static uint16 widgets_edged_out(int now_step, int max_step, uint16 color)
{
    uint16 output_color = 0;
    uint16 output_color_r = 0;
    uint16 output_color_g = 0;
    uint16 output_color_b = 0;

    output_color_r = (((color >> 11) + (31 - (color >> 11)) * now_step / max_step));
    output_color_g = ((((color & 0x07E0) >> 5) + (63 - ((color & 0x07E0) >> 5)) * now_step / max_step));
    output_color_b = ((color & 0x001F) + (31 - (color & 0x001F)) * now_step / max_step);
    output_color =  (output_color_r << 11) | (output_color_g << 5) | output_color_b;

    return output_color;
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介          组件分步显示
// 返回类型          void
// 使用示例          widgets_edged_in(action_switch_count, 10, TABLE_DEFAULT_BORDER_COLOR);
// 备注信息          内部动画调用 原理：从 白色 分步骤 逐渐变化到 固定颜色
//-------------------------------------------------------------------------------------------------------------------
static uint16 widgets_edged_in(int now_step, int max_step, uint16 color)
{
    uint16 output_color = 0;
    uint16 output_color_r = 0;
    uint16 output_color_g = 0;
    uint16 output_color_b = 0;

    output_color_r = 31 - (31 - (color >> 11)) * now_step / max_step;
    output_color_g = 63 - (63 - ((color & 0x07E0) >> 5)) * now_step / max_step;
    output_color_b = 31 - (31 - (color & 0x001F)) * now_step / max_step;

    output_color =  (output_color_r << 11) | (output_color_g << 5) | output_color_b;

    return output_color;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介          用户菜单 界面切换 动画函数
// 返回类型          void
// 使用示例          user_menu_action_loop();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void user_menu_action_loop(void)
{
    if(action_switch_flag == 1)             // 判断界面切换动画 标志位
    {
        if(menu_switch_page == 0)           // 判断需要切换到哪个界面
        {
            if(action_switch_step == 0)     // 按照动画步骤 播放切换动画
            {
                // 参数设置表格 逐渐消失
                ips200pro_set_color(widgets_table_id[0], COLOR_BORDER,     widgets_edged_out(action_switch_count, 10, TABLE_DEFAULT_BORDER_COLOR));
                ips200pro_set_color(widgets_table_id[0], COLOR_FOREGROUND, widgets_edged_out(action_switch_count, 10, TABLE_DEFAULT_FOREGROUND_COLOR));
                ips200pro_set_color(widgets_table_id[0], COLOR_BACKGROUND, widgets_edged_out(action_switch_count, 10, TABLE_DEFAULT_BACKGROUND_COLOR));

                // 电池电量 进度条逐渐下移
                ips200pro_set_position(widgets_progress_bar_id[0], 0, -22 + action_switch_count * 2);
                ips200pro_set_position(widgets_label_id[0], 2, -22 + action_switch_count * 2);
                ips200pro_set_position(widgets_label_id[1], 176, -22 + action_switch_count * 2);

                // CPU占用率 进度条逐渐下移
                ips200pro_set_position(widgets_progress_bar_id[1], 0, -22 + action_switch_count * 4);
                ips200pro_set_position(widgets_label_id[2], 2, -22 + action_switch_count * 4);
                ips200pro_set_position(widgets_label_id[3], 176, -22 + action_switch_count * 4);

                // 图像 以及 掩盖容器 逐渐下移
                ips200pro_set_position(widgets_image_id, 26, 0 + action_switch_count * 4);
                ips200pro_set_position(widgets_container_id, 0, -44 + action_switch_count * 4);

                if(++ action_switch_count >= 11)       // 第一步 动画结束
                {
                    action_switch_step ++;
                    action_switch_count = 0;
                    ips200pro_set_hidden(widgets_table_id[1], 0);
                    ips200pro_set_hidden(widgets_table_id[0], 1);
                    ips200pro_table_select(widgets_table_id[0], 1, 0);
                    ips200pro_set_position(widgets_table_id[0], 0, 133);
                }
            }
            else if(action_switch_step == 1)
            {
                // 实时参数表格 逐渐浮现
                ips200pro_set_color(widgets_table_id[1], COLOR_BORDER,     widgets_edged_in(action_switch_count, 10, TABLE_DEFAULT_BORDER_COLOR));
                ips200pro_set_color(widgets_table_id[1], COLOR_FOREGROUND, widgets_edged_in(action_switch_count, 10, TABLE_DEFAULT_FOREGROUND_COLOR));
                ips200pro_set_color(widgets_table_id[1], COLOR_BACKGROUND, widgets_edged_in(action_switch_count, 10, TABLE_DEFAULT_BACKGROUND_COLOR));
                if(++ action_switch_count >= 11)
                {
                    action_switch_step ++;
                    action_switch_count = 0;
                }
            }
            else
            {
                action_switch_step  = 0;
                action_switch_count = 0;
                action_switch_flag = 0;
            }
        }
        else
        {
            if(action_switch_step == 0)
            {
                ips200pro_set_color(widgets_table_id[1], COLOR_BORDER,     widgets_edged_out(action_switch_count, 10, TABLE_DEFAULT_BORDER_COLOR));
                ips200pro_set_color(widgets_table_id[1], COLOR_FOREGROUND, widgets_edged_out(action_switch_count, 10, TABLE_DEFAULT_FOREGROUND_COLOR));
                ips200pro_set_color(widgets_table_id[1], COLOR_BACKGROUND, widgets_edged_out(action_switch_count, 10, TABLE_DEFAULT_BACKGROUND_COLOR));

                ips200pro_set_position(widgets_progress_bar_id[0], 0, 0 - action_switch_count * 2);
                ips200pro_set_position(widgets_label_id[0], 2, 0 - action_switch_count * 2);
                ips200pro_set_position(widgets_label_id[1], 176, 0 - action_switch_count * 2);

                ips200pro_set_position(widgets_progress_bar_id[1], 0, 22 - action_switch_count * 4);
                ips200pro_set_position(widgets_label_id[2], 2, 22 - action_switch_count * 4);
                ips200pro_set_position(widgets_label_id[3], 176, 22 - action_switch_count * 4);

                ips200pro_set_position(widgets_image_id, 26, 44 - action_switch_count * 4);
                ips200pro_set_position(widgets_container_id, 0, 0 - action_switch_count * 4);

                if(++ action_switch_count >= 11)
                {
                    action_switch_step ++;
                    action_switch_count = 0;
                    ips200pro_set_hidden(widgets_table_id[1], 1);
                    ips200pro_table_select(widgets_table_id[0], (uint8)value_switch_num, 0);
                }
            }
            else if(action_switch_step == 1)
            {
                ips200pro_set_color(widgets_table_id[0], COLOR_BORDER,     widgets_edged_in(action_switch_count, 10, TABLE_DEFAULT_BORDER_COLOR));
                ips200pro_set_color(widgets_table_id[0], COLOR_FOREGROUND, widgets_edged_in(action_switch_count, 10, TABLE_DEFAULT_FOREGROUND_COLOR));
                ips200pro_set_color(widgets_table_id[0], COLOR_BACKGROUND, widgets_edged_in(action_switch_count, 10, TABLE_DEFAULT_BACKGROUND_COLOR));

                if(action_switch_count == 0)
                {
                    ips200pro_set_hidden(widgets_table_id[0], 0);
                }
                if(++ action_switch_count >= 11)
                {
                    action_switch_step ++;
                    action_switch_count = 0;
                }
            }
            else
            {
                action_switch_step  = 0;
                action_switch_count = 0;
                action_switch_flag = 0;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介          用户菜单 表格移动 动画函数
// 返回类型          void
// 使用示例          user_menu_table_action_loop();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void user_menu_table_action_loop(void)
{
    if(menu_switch_page == 1 && action_switch_flag == 0)
    {
        if(table_action_switch_flag)
        {
            table_action_offset = (int16)func_limit_ab(((float)(value_switch_num - 8) * 23), 0, (23 * (USER_MENU_VALUE_TABLE_NUM - 8)));

            int16 offset_speed = func_limit_ab(func_abs(table_action_offset - table_action_offset_filter) / 5, 3, 99);

            table_action_offset_filter = func_limit_ab(table_action_offset, table_action_offset_filter - offset_speed, table_action_offset_filter + offset_speed);

            ips200pro_set_position(widgets_table_id[0], 0, 133 - table_action_offset_filter);

            ips200pro_table_select(widgets_table_id[0], (uint8)value_switch_num, 0);

            if(table_action_offset_filter == table_action_offset)
            {
                table_action_switch_flag = 0;
            }
        }

        if(value_refresh_flag)
        {
            value_refresh_flag = 0;

            user_menu_display_set_value();
        }

    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介          CPU占有率检测
// 返回类型          void
// 使用示例          cpu_usage_check();
// 备注信息          该函数需放在主循环运行 默认该函数内的1ms延时作为空闲状态 统计1秒内执行该延时的次数 即可计算出CPU占有率
//-------------------------------------------------------------------------------------------------------------------
void cpu_usage_check(void)
{
    static int32 system_count   = 0;               // 定时器计数值获取

    static float run_time_count = 0;               // 定时器计数值获取

    static int32 first_run_flag = 0;               // 初次运行标志位
    
    int32 run_time_get;                            // 间隔时长临时变量
    
    if(first_run_flag == 0)
    {
        first_run_flag = 1;
        
        timer_init(GPT_TIM_1, TIMER_US); 
        
        timer_clear(GPT_TIM_1);

        timer_start(GPT_TIM_1);
    }

    system_count = timer_get(GPT_TIM_1);      // 获取定时器计数值

    if(system_count >= 100000)                   // 如果大于100ms 则进行占用率计算
    {
        timer_clear(GPT_TIM_1);               // 重置统计周期
      
        cpu_usage_percentage = (func_limit_ab(100000 - run_time_count, 0, 100000)) / ((float)system_count / 100000.0f) / 1000.0f;       // 计算100ms内 运行此函数的总耗时

        run_time_count = 0;
        
        system_count = 0;

        cpu_usage_percentage_filter = (cpu_usage_percentage_filter * 9 + cpu_usage_percentage) / 10.0f;                 // 占用率数据 低通滤波
    }

    run_time_get = timer_get(GPT_TIM_1);                      // 统计运算该函数的耗时

    // 获取方法：主循环仅cpu_usage_check时，通过调节 偏差矫正值 保证占用率计算判断内的run_time_count在 100ms ± 1ms 即可
    run_time_count = run_time_count + ((float)run_time_get - (float)system_count + 0.155);
}











