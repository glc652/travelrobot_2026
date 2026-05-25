#include "ipspro_page_ctrl.h"

extern int angle, far_value, image_err, run;
extern uint32 rand_num;

// 初始化页面的变量
uint16  widgets_page_id[3];

// 初始化页面1内的控件变量
uint16  page1_widgets_container_id[4];
uint16  page1_widgets_label_id[5];
uint16  page1_widgets_image_id[1];
uint16  page1_widgets_meter_id[1];
uint16  page1_widgets_param_id[2];

// 初始化页面2内的控件变量
uint16  page2_widgets_label_id[4];
uint16  page2_widgets_table_id[3];

// 初始化页面3内的控件变量
uint16  page3_widgets_container_id[1];
uint16  page3_widgets_table_id[1];

// 初始化当前页面记录
uint8 now_page_num = 1;
int8 now_table_num = 0;
uint8 now_table_num_switch = 1;

// 初始化调参的数据
float road_para[3] = {0};
float speed_para[3] = {0};
float pos_para[3] = {0};

// 初始化调参每次调整的步长
float road_para_step[3] = {0.001, 0.01, 0.001};
float speed_para_step[3] = {10, 0.1, 0.1};
float pos_para_step[3] = {0};

float* param[3] = {road_para, speed_para, pos_para};
float* param_step[3] = {road_para_step, speed_para_step, pos_para_step};

void ipspro_page_ctrl_flash_init()
{
    flash_init();
// 首次需要先解除下面的注释，初始化flash数据，后续再继续注释
//    flash_union_buffer[0].float_type = param[0][0];
//    flash_union_buffer[1].float_type = param[0][1];
//    flash_union_buffer[2].float_type = param[0][2];
//    flash_union_buffer[3].float_type = param[1][0];
//    flash_union_buffer[4].float_type = param[1][1];
//    flash_union_buffer[5].float_type = param[1][2];
//    flash_union_buffer[6].float_type = param[2][0];
//    flash_union_buffer[7].float_type = param[2][1];
//    flash_union_buffer[8].float_type = param[2][2];
//    flash_erase_page(127, 0);
//    flash_write_page_from_buffer(127, 0);
    
// 每次上电读取flash数据
    flash_read_page_to_buffer(127, (flash_page_enum)0);
    SCB_InvalidateDCache();
    param[0][0] = flash_union_buffer[0].float_type;
    param[0][1] = flash_union_buffer[1].float_type;
    param[0][2] = flash_union_buffer[2].float_type;
    param[1][0] = flash_union_buffer[3].float_type;
    param[1][1] = flash_union_buffer[4].float_type;
    param[1][2] = flash_union_buffer[5].float_type;
    param[2][0] = flash_union_buffer[6].float_type;
    param[2][1] = flash_union_buffer[7].float_type;
    param[2][2] = flash_union_buffer[8].float_type;
}

// 初始化按键控制，需要在pit的通道一中断中添加按键回调
void ipspro_page_ctrl_key_init()
{
    key_init(10);
    key_clear_all_state();
    pit_ms_init(PIT_CH1, 10);
}

// 显示数据到页面1上，包括摄像头，角度，图像信息
void ipspro_page_ctrl_show_page1(int angle, int far_value, int image_err, bool run)
{
    if(now_page_num == 1)
    {
        if(mt9v03x_finish_flag && IS_USE_CAMERA)
        {
// 设置边线
//          ips200pro_image_draw_line(page1_widgets_image_id[0], 1, mid_line, 120, IMAGE_LINE_TYPE_UINT8, RGB565_GREEN);
//          ips200pro_image_draw_line(page1_widgets_image_id[0], 2, left_line, 120, IMAGE_LINE_TYPE_UINT8, RGB565_RED);
//          ips200pro_image_draw_line(page1_widgets_image_id[0], 3, right_line, 120, IMAGE_LINE_TYPE_UINT8, RGB565_BLUE);
            
            ips200pro_image_display(page1_widgets_image_id[0], mt9v03x_image[0], 188, 120, IMAGE_GRAYSCALE, 0);     
        }
        ips200pro_label_printf(page1_widgets_param_id[0], "%3d", far_value);
        ips200pro_label_printf(page1_widgets_param_id[1], "%3d", image_err);
        if(run)
        {
            ips200pro_label_printf(page1_widgets_label_id[2], "运行");
            ips200pro_set_color(page1_widgets_container_id[3], COLOR_BACKGROUND, RGB565_GREEN);
        }
        else
        {
            ips200pro_label_printf(page1_widgets_label_id[2], "停止");
            ips200pro_set_color(page1_widgets_container_id[3], COLOR_BACKGROUND, RGB565_RED);
        }
//        // 设置方向角度值
        ips200pro_meter_set_value(page1_widgets_meter_id[0], (int)angle);
    }
}

// 显示识别内容到页面3上
void ipspro_page_ctrl_show_page3(uint8 list_num, char* label)
{
    ips200pro_table_cell_printf(page3_widgets_table_id[0], list_num, 2, "\t\t%s", label);
}

// 按键控制
void ipspro_page_ctrl_key_loop()
{
    if(key_get_state(KEY_4) == KEY_SHORT_PRESS) // 短按按键4切换页面1和页面2
    {
        if(now_page_num == 1)
        {
            now_page_num = 2;
            ips200pro_set_color(page2_widgets_table_id[now_table_num/3], COLOR_TABLE_SELECTED_BG, MAIN_COLOR);
            ips200pro_table_select(page2_widgets_table_id[now_table_num/3], now_table_num%3 + 1, now_table_num_switch);
        }
        else
        {
            now_page_num = 1;
            
        }
        ips200pro_page_switch(now_page_num, PAGE_ANIM_ON);  
        key_clear_state(KEY_4);
    }
    else if(key_get_state(KEY_4) == KEY_LONG_PRESS) // 长按按键4切换页面3
    {
        ips200pro_page_switch(3, PAGE_ANIM_ON);  
        key_clear_state(KEY_4);
    }
    
    if(key_get_state(KEY_2) == KEY_SHORT_PRESS) // 短按按键2
    {
        if(now_page_num == 1)   // 如果在页面1，则随机显示角度，图像等参数
        {
            angle = rand_num % 360;
            far_value = rand_num % 120;
            image_err = rand_num % 100 - 50;
            run = rand_num % 2;
        }
        else if(now_page_num == 2)  //如果在页面2，则切换参数名称或者参数的加减
        {
            if(now_table_num_switch == 1)
            {
                now_table_num_switch = 2;
            }
            else
            {
                now_table_num_switch = 1;
            }
            ips200pro_table_select(page2_widgets_table_id[now_table_num/3], now_table_num%3 + 1, now_table_num_switch);
        }
        key_clear_state(KEY_2);
    }
    
    if(key_get_state(KEY_1) == KEY_SHORT_PRESS) // 短按按键1
    {
        if(now_page_num == 2)   //如果在页面2，则控制参数名称或者参数的加
        {
            if(now_table_num_switch == 1)
            {
                now_table_num--;
                if(now_table_num < 0)
                {
                    now_table_num = 8;
                }
                ips200pro_table_select(page2_widgets_table_id[now_table_num/3], now_table_num%3 + 1, now_table_num_switch);
            }
            else if(now_table_num_switch == 2)
            {
                param[now_table_num/3][now_table_num%3] += param_step[now_table_num/3][now_table_num%3];
                
                ips200pro_table_cell_printf(page2_widgets_table_id[now_table_num/3], (now_table_num%3) + 1, 2, "  %.4f", param[now_table_num/3][now_table_num%3]);
                
                flash_union_buffer[now_table_num].float_type = param[now_table_num/3][now_table_num%3];
                flash_erase_page(127, (flash_page_enum)0);
                flash_write_page_from_buffer(127, (flash_page_enum)0);
            }
        }
        key_clear_state(KEY_1);
    }
    
    if(key_get_state(KEY_3) == KEY_SHORT_PRESS) // 短按按键3
    {
        if(now_page_num == 2)   //如果在页面2，则控制参数名称或者参数的减
        {
            if(now_table_num_switch == 1)
            {
                now_table_num++;
                if(now_table_num > 8)
                {
                    now_table_num = 0;
                }
                ips200pro_table_select(page2_widgets_table_id[now_table_num/3], now_table_num%3 + 1, now_table_num_switch);
            }
            else if(now_table_num_switch == 2)
            {
                param[now_table_num/3][now_table_num%3] -= param_step[now_table_num/3][now_table_num%3];
                
                ips200pro_table_cell_printf(page2_widgets_table_id[now_table_num/3], (now_table_num%3) + 1, 2, "  %.4f", param[now_table_num/3][now_table_num%3]);
                
                flash_union_buffer[now_table_num].float_type = param[now_table_num/3][now_table_num%3];
                flash_erase_page(127, (flash_page_enum)0);
                flash_write_page_from_buffer(127, (flash_page_enum)0);
            }
        }
        key_clear_state(KEY_3);
    }
    
}

// 初始化所有页面
void ipspro_page_ctrl_init(void)
{
    // 可宏定义控制是否开启摄像头
    if(IS_USE_CAMERA)
    {
        mt9v03x_init();
    }
    ipspro_page_ctrl_flash_init();
    ips200pro_init(NULL, IPS200PRO_TITLE_BOTTOM, 30);
    // 设置全局字体为16号
    ips200pro_set_default_font(FONT_SIZE_16);
    ips200pro_set_direction(IPS200PRO_PORTRAIT);
    
    widgets_page_id[0] = ips200pro_page_create("实时状态");
    widgets_page_id[1] = ips200pro_page_create("修改参数");
    widgets_page_id[2] = ips200pro_page_create("识别内容");
    ips200pro_set_backlight(0);
//    ips200pro_set_hidden(SCREEN_WIDGETS_PAGE, 1, 1);
    
    ips200pro_page_switch(3, PAGE_ANIM_ON);
    
    ips200pro_set_color(widgets_page_id[2], COLOR_PAGE_SELECTED_TEXT, MAIN_COLOR);
    ips200pro_set_color(widgets_page_id[2], COLOR_PAGE_SELECTED_BG, MAIN_COLOR);
    ips200pro_set_color(widgets_page_id[2], COLOR_BACKGROUND, BACK_COLOR);
    
    page3_widgets_container_id[0] = ips200pro_container_create(10, 10, 220, 270);
    ips200pro_container_radius(page3_widgets_container_id[0], 0, 10);
    ips200pro_set_color( page3_widgets_container_id[0], COLOR_BACKGROUND, MAIN_COLOR);
    

    page3_widgets_table_id[0] = ips200pro_table_create(20, 26, 8, 2);
    ips200pro_set_color(page3_widgets_table_id[0], COLOR_FOREGROUND, BACK_COLOR);
    ips200pro_set_color(page3_widgets_table_id[0], COLOR_BORDER, BACK_COLOR);
    ips200pro_table_set_col_width(page3_widgets_table_id[0], 1, 60);
    ips200pro_table_set_col_width(page3_widgets_table_id[0], 2, 140);
    ips200pro_table_cell_printf(page3_widgets_table_id[0], 1, 1, "  1 ");
    ips200pro_table_cell_printf(page3_widgets_table_id[0], 2, 1, "  2 ");
    ips200pro_table_cell_printf(page3_widgets_table_id[0], 3, 1, "  3 ");
    ips200pro_table_cell_printf(page3_widgets_table_id[0], 4, 1, "  4 ");
    ips200pro_table_cell_printf(page3_widgets_table_id[0], 5, 1, "  5 ");
    ips200pro_table_cell_printf(page3_widgets_table_id[0], 6, 1, "  6 ");
    ips200pro_table_cell_printf(page3_widgets_table_id[0], 7, 1, "  7 ");
    ips200pro_table_cell_printf(page3_widgets_table_id[0], 8, 1, "  8 ");
    
    
    ips200pro_page_switch(2, PAGE_ANIM_ON);    
    ips200pro_set_color(widgets_page_id[1], COLOR_PAGE_SELECTED_TEXT, MAIN_COLOR);
    ips200pro_set_color(widgets_page_id[1], COLOR_PAGE_SELECTED_BG, MAIN_COLOR);
    ips200pro_set_color(widgets_page_id[1], COLOR_BACKGROUND, BACK_COLOR);

    page2_widgets_label_id[0] = ips200pro_label_create(85, 3, 80, 22);
    ips200pro_label_printf(page2_widgets_label_id[0], "循迹参数");
    ips200pro_set_color(page2_widgets_label_id[0], COLOR_FOREGROUND, MAIN_COLOR);
    
    page2_widgets_table_id[0] = ips200pro_table_create(20, 26, 3, 2);
    ips200pro_set_color(page2_widgets_table_id[0], COLOR_FOREGROUND, MAIN_COLOR);
    ips200pro_set_color(page2_widgets_table_id[0], COLOR_BORDER, MAIN_COLOR);
    ips200pro_table_set_col_width(page2_widgets_table_id[0], 1, 60);
    ips200pro_table_set_col_width(page2_widgets_table_id[0], 2, 140);
    ips200pro_table_cell_printf(page2_widgets_table_id[0], 1, 1, "  P2");
    ips200pro_table_cell_printf(page2_widgets_table_id[0], 2, 1, "  P ");
    ips200pro_table_cell_printf(page2_widgets_table_id[0], 3, 1, "  D ");
    
    ips200pro_table_cell_printf(page2_widgets_table_id[0], 1, 2, "  %.4f", param[0][0]);
    ips200pro_table_cell_printf(page2_widgets_table_id[0], 2, 2, "  %.4f", param[0][1]);
    ips200pro_table_cell_printf(page2_widgets_table_id[0], 3, 2, "  %.4f", param[0][2]);
    
    page2_widgets_label_id[1] = ips200pro_label_create(85, 99, 80, 22);
    ips200pro_label_printf(page2_widgets_label_id[1], "速度参数");
    ips200pro_set_color(page2_widgets_label_id[1], COLOR_FOREGROUND, MAIN_COLOR);
    
    page2_widgets_table_id[1] = ips200pro_table_create(20, 122, 3, 2);
    ips200pro_set_color(page2_widgets_table_id[1], COLOR_FOREGROUND, MAIN_COLOR);
    ips200pro_set_color(page2_widgets_table_id[1], COLOR_BORDER, MAIN_COLOR);
    ips200pro_table_set_col_width(page2_widgets_table_id[1], 1, 60);
    ips200pro_table_set_col_width(page2_widgets_table_id[1], 2, 140);
    ips200pro_table_cell_printf(page2_widgets_table_id[1], 1, 1, "  S ");
    ips200pro_table_cell_printf(page2_widgets_table_id[1], 2, 1, "  P ");
    ips200pro_table_cell_printf(page2_widgets_table_id[1], 3, 1, "  I ");
    ips200pro_table_cell_printf(page2_widgets_table_id[1], 1, 2, "  %.4f", param[1][0]);
    ips200pro_table_cell_printf(page2_widgets_table_id[1], 2, 2, "  %.4f", param[1][1]);
    ips200pro_table_cell_printf(page2_widgets_table_id[1], 3, 2, "  %.4f", param[1][2]);
      
    page2_widgets_label_id[2] = ips200pro_label_create(85, 195, 80, 22);
    ips200pro_label_printf(page2_widgets_label_id[2], "位置参数");
    ips200pro_set_color(page2_widgets_label_id[2], COLOR_FOREGROUND, MAIN_COLOR);
    
    page2_widgets_table_id[2] = ips200pro_table_create(20, 217, 3, 2);
    ips200pro_set_color(page2_widgets_table_id[2], COLOR_FOREGROUND, MAIN_COLOR);
    ips200pro_set_color(page2_widgets_table_id[2], COLOR_BORDER, MAIN_COLOR);
    ips200pro_table_set_col_width(page2_widgets_table_id[2], 1, 60);
    ips200pro_table_set_col_width(page2_widgets_table_id[2], 2, 140);
    ips200pro_table_cell_printf(page2_widgets_table_id[2], 1, 1, "  P ");
    ips200pro_table_cell_printf(page2_widgets_table_id[2], 2, 1, "  I ");
    ips200pro_table_cell_printf(page2_widgets_table_id[2], 3, 1, "  D "); 
    ips200pro_table_cell_printf(page2_widgets_table_id[2], 1, 2, "  %.4f", param[2][0]);
    ips200pro_table_cell_printf(page2_widgets_table_id[2], 2, 2, "  %.4f", param[2][1]);
    ips200pro_table_cell_printf(page2_widgets_table_id[2], 3, 2, "  %.4f", param[2][2]); 
    
    ips200pro_page_switch(1, PAGE_ANIM_ON);
    
    //图像
    ips200pro_set_color(widgets_page_id[0], COLOR_PAGE_SELECTED_TEXT, MAIN_COLOR);
    ips200pro_set_color(widgets_page_id[0], COLOR_PAGE_SELECTED_BG, MAIN_COLOR);
    ips200pro_set_color(widgets_page_id[0], COLOR_BACKGROUND, BACK_COLOR);
    
    page1_widgets_container_id[0] = ips200pro_container_create(5, 5, 230, 137);
    ips200pro_container_radius(page1_widgets_container_id[0], 0, 10);
    ips200pro_set_color(page1_widgets_container_id[0], COLOR_BACKGROUND, MAIN_COLOR);
    
    page1_widgets_label_id[0] = ips200pro_label_create(12, 46, 20, 100);
    ips200pro_label_printf(page1_widgets_label_id[0], "图\n\n像");
    ips200pro_set_color(page1_widgets_label_id[0], COLOR_FOREGROUND, BACK_COLOR);
    page1_widgets_image_id[0] = ips200pro_image_create(35, 14, 188, 120);
    
    //方向
    page1_widgets_container_id[1] = ips200pro_container_create(5, 150, 160, 130);
    ips200pro_container_radius(page1_widgets_container_id[1], 0, 10);
    ips200pro_set_color(page1_widgets_container_id[1], COLOR_BACKGROUND, MAIN_COLOR);
    
    page1_widgets_label_id[1] = ips200pro_label_create(12, 186, 20, 120);
    ips200pro_label_printf(page1_widgets_label_id[1], "方\n\n向");
    ips200pro_set_color(page1_widgets_label_id[1], COLOR_FOREGROUND, BACK_COLOR);
    page1_widgets_meter_id[0] = ips200pro_meter_create(35, 154, 120, METER_ANGLE);
    ips200pro_meter_set_value(page1_widgets_meter_id[0], 0);
    ips200pro_set_color(page1_widgets_meter_id[0], COLOR_MRTER_TICKS, MAIN_COLOR);
    ips200pro_set_color(page1_widgets_meter_id[0], COLOR_FOREGROUND, MAIN_COLOR);

    //速度
    page1_widgets_container_id[2] = ips200pro_container_create(172, 150, 63, 130);
    ips200pro_container_radius(page1_widgets_container_id[2], 0, 10);
    ips200pro_set_color(page1_widgets_container_id[2], COLOR_BACKGROUND, MAIN_COLOR);
        
    page1_widgets_container_id[3] = ips200pro_container_create(178, 158, 50, 30);
    ips200pro_container_radius(page1_widgets_container_id[3], 0, 10);
    ips200pro_set_color(page1_widgets_container_id[3], COLOR_BACKGROUND, RGB565_RED);
    page1_widgets_label_id[2] = ips200pro_label_create(187, 164, 100, 20);
    ips200pro_label_printf(page1_widgets_label_id[2], "运行");
    ips200pro_set_color(page1_widgets_label_id[2], COLOR_FOREGROUND, BACK_COLOR);
    
    page1_widgets_label_id[3] = ips200pro_label_create(179, 195, 100, 20);
    ips200pro_label_printf(page1_widgets_label_id[3], "远端值");
    ips200pro_set_color(page1_widgets_label_id[3], COLOR_FOREGROUND, BACK_COLOR);
    page1_widgets_param_id[0] = ips200pro_label_create(189, 215, 100, 20);
    ips200pro_label_printf(page1_widgets_param_id[0], "000");
    ips200pro_set_color(page1_widgets_param_id[0], COLOR_FOREGROUND, BACK_COLOR);
    
    page1_widgets_label_id[4] = ips200pro_label_create(179, 236, 100, 20);
    ips200pro_label_printf(page1_widgets_label_id[4], "偏差值");
    ips200pro_set_color(page1_widgets_label_id[4], COLOR_FOREGROUND, BACK_COLOR);
    page1_widgets_param_id[1] = ips200pro_label_create(189, 256, 100, 20);
    ips200pro_label_printf(page1_widgets_param_id[1], "000");
    ips200pro_set_color(page1_widgets_param_id[1], COLOR_FOREGROUND, BACK_COLOR);
 
    ipspro_page_ctrl_key_init();
    ips200pro_page_switch(1, PAGE_ANIM_ON);
    ips200pro_set_backlight(255);
}

