#ifndef IPSPRO_PAGE_CTRL_H
#define IPSPRO_PAGE_CTRL_H

#include "zf_common_headfile.h"

// 设置主题色
#define MAIN_COLOR      IPS200PRO_RGB888_TO_RGB565(221,187,153)
#define BACK_COLOR      IPS200PRO_RGB888_TO_RGB565(83,81,100)

// 设置是否启用摄像头
#define IS_USE_CAMERA   true
    
void ipspro_page_ctrl_show_page3(uint8 list_num, char* label);
void ipspro_page_ctrl_show_page1(int angle, int far_value, int image_err, bool run);
void ipspro_page_ctrl_key_loop();
void ipspro_page_ctrl_init(void);
    
#endif // IPSPRO_PAGE_CTRL_H