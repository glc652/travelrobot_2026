#ifndef _USER_MENU_DEVICE_H_
#define _USER_MENU_DEVICE_H_

#include "zf_common_typedef.h"
#include "zf_driver_adc.h"


#define TABLE_DEFAULT_BORDER_COLOR          (0x05FF)    // 表格默认边线颜色  天蓝色(RGB565)
#define TABLE_DEFAULT_FOREGROUND_COLOR      (0x0000)    // 表格默认字体颜色  黑色(RGB565)
#define TABLE_DEFAULT_BACKGROUND_COLOR      (0xFFFF)    // 表格默认背景颜色  白色(RGB565)


extern uint16 widgets_page_id[2];                       // 分页组件ID
extern uint16 widgets_label_id[4];                      // 标签组件ID
extern uint16 widgets_table_id[2];                      // 表格组件ID
extern uint16 widgets_image_id;                         // 图像组件ID
extern uint16 widgets_progress_bar_id[2];               // 进度条组件ID
extern uint16 widgets_container_id;                     // 容器组件ID

extern uint8  menu_switch_page;                         // 当前选择分页
extern uint8  action_switch_flag;                       // 动画切换flag
extern int    action_switch_step;                       // 动画切换步骤
extern int    action_switch_count;                      // 动画切换计数
extern uint8  table_action_switch_flag;                 // 表格动画切换flag
extern int16  table_action_offset;                      // 表格控件动画偏移值
extern int16  table_action_offset_filter;               // 表格控件动画偏移值（滤波）
extern int16  value_switch_num;                         // 当前选择参数
extern int16  pid_switch_num;                           // 当前 PID 选择修改参数
extern int16  value_refresh_flag;                       // 参数刷新标志位

extern float battery_voltage;                           // 电池电压
extern float battery_percentage;                        // 电池电压百分比
extern float battery_percentage_filter;                 // 电池电压百分比（滤波）

extern int32 cpu_usage_check_count;                     // CPU占有率计数
extern float cpu_usage_percentage;                      // CPU占有率百分比
extern float cpu_usage_percentage_filter;               // CPU占有率百分比（滤波）

extern const uint16 progress_bar_status_color[100];
void time_callback();
void user_menu_action_loop(void);
void user_menu_table_action_loop(void);
void battery_voltage_gather(uint16 adc_get_data);
void cpu_usage_check(void);


#endif
