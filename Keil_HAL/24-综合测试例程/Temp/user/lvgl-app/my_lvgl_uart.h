#ifndef __MY_LVGL_UART_H__
#define __MY_LVGL_UART_H__

#include "lvgl.h"


//static char receive_buffer[100]; // ���ݽ��ջ�����
void my_lvgl_set_receive_box(void);
lv_obj_t *my_lvgl_uart(void);

#endif

