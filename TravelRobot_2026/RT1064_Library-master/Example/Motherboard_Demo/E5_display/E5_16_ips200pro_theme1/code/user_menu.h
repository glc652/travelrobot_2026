#ifndef _USER_MENU_H_
#define _USER_MENU_H_

#include "zf_common_typedef.h"


#define USER_MENU_VALUE_TABLE_NUM           (13)        // 用户菜单 设置参数数量  示例为13个可调参数
#define USER_MENU_REALTIME_VALUE_TABLE_NUM  (6)         // 用户菜单 实时参数数量  示例为6 个实时参数


void user_menu_key_dispose                  (void);     // 用户菜单 按键处理函数

void user_menu_display_set_value_name       (void);     // 用户菜单 显示 设置参数名称

void user_menu_display_set_value            (void);     // 用户菜单 显示 设置参数内容

void user_menu_display_realtime_value_name  (void);     // 用户菜单 显示 实时参数名称

void user_menu_display_realtime_value       (void);     // 用户菜单 显示 实时参数内容

void user_menu_loop                         (void);     // 用户菜单 循环语句 放主函数内容

void user_menu_init                         (void);     // 用户菜单 初始化语句


#endif
