#include "my_lvgl_led.h"
#include "main.h" // ȷ����������ʵ���STM32ͷ�ļ�

#define LED_COUNT 8 // LED������
#define LED_WIDTH  25
#define LED_HEIGHT  25
#define LED_SPACING 5

 //���ڿ���LED��GPIO����
GPIO_TypeDef* LED_GPIO_PORT[LED_COUNT] = {LED1_GPIO_Port, LED2_GPIO_Port, LED3_GPIO_Port, LED4_GPIO_Port, LED5_GPIO_Port, LED6_GPIO_Port, LED7_GPIO_Port, LED8_GPIO_Port};
uint16_t LED_GPIO_PIN[LED_COUNT] = {LED1_Pin, LED2_Pin, LED3_Pin, LED4_Pin, LED5_Pin, LED6_Pin, LED7_Pin, LED8_Pin};

 //����¼����������л�LED�Ƶ�״̬
static void led_button_event_handler(lv_event_t * e) {
    lv_obj_t * led_button = lv_event_get_target(e); // ��ȡ�����LED��ť
    uint8_t index = (uintptr_t)lv_obj_get_user_data(led_button); // ��ȡ��ť����

    // ��ȡ��ǰ״̬
		lv_color_t bg_color = lv_obj_get_style_bg_color(led_button,NULL);
    
    // �л�LED��״̬
    if (0xFF == bg_color.blue) {
        HAL_GPIO_WritePin(LED_GPIO_PORT[index], LED_GPIO_PIN[index], GPIO_PIN_SET); // �ر�LED
        lv_obj_set_style_bg_color(led_button, lv_color_hex(0x808080), 0); // ����Ϊ��ɫ���ر�״̬��
    } else {
        HAL_GPIO_WritePin(LED_GPIO_PORT[index], LED_GPIO_PIN[index], GPIO_PIN_RESET); // ��LED
        lv_obj_set_style_bg_color(led_button, lv_color_hex(0x0000FF), 0); // ����Ϊ��ɫ����״̬��
    }
}

void create_leds_8(lv_obj_t * parent) {
    // ����8��LED��
    for (int i = 0; i < LED_COUNT; i++) {
        lv_obj_t * led_button = lv_btn_create(parent);
        lv_obj_set_size(led_button, LED_WIDTH, LED_HEIGHT);
        lv_obj_align(led_button, LV_ALIGN_BOTTOM_MID, (LED_WIDTH + LED_SPACING) * (i - (LED_COUNT / 2)) + (LED_WIDTH / 2), -60);

        // ���ó�ʼ״̬Ϊ�ر�
        lv_obj_set_style_bg_color(led_button, lv_color_hex(0x808080), 0); // ��ɫ
        lv_obj_set_style_radius(led_button, LED_WIDTH / 2, 0); // ʹ���Բ��
        
        // ���ð�ť�û�����ΪLED����
        lv_obj_set_user_data(led_button, (void*)(uintptr_t)i); // ����LED����

        // ��ӵ���¼�
        lv_obj_add_event_cb(led_button, led_button_event_handler, LV_EVENT_CLICKED, NULL);
    }
}
