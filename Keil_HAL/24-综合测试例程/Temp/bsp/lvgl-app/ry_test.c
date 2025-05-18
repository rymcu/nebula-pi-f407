
#include "lvgl.h"
#include "ry_test.h"
// ���庯��ָ������
void (*screen_func_ptr)(void);



typedef struct my_page
{
	uint32_t screen_addr;
} my_page_t;

lv_ll_t ll_test1;
my_page_t *page_temp1;
/*--------------------------------------------------------------------------------------------------------
*��ʼ��������ҳ��������ӵ�������ָ��ͷ�ڵ�
*
--------------------------------------------------------------------------------------------------------*/
void ry_list_init1(void)
{	
	_lv_ll_init(&ll_test1, sizeof(my_page_t));

	page_temp1 = _lv_ll_ins_head(&ll_test1);
	page_temp1->screen_addr = (uint32_t)create_screen2;//�������ƾ��Ǻ�����ַ
	page_temp1 = _lv_ll_ins_head(&ll_test1);
	page_temp1->screen_addr = (uint32_t)create_screen1;

	
	page_temp1 = _lv_ll_get_head(&ll_test1);
}

void ry_test(void)
{
	ry_list_init1();
	screen_func_ptr = (void(*)(void))(page_temp1->screen_addr);
	screen_func_ptr();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ȫ�ֱ���
static lv_obj_t * current_screen = NULL; // ��ǰ��Ļ����

void delete_current_screen(void) 
	{
    if (current_screen != NULL) {
        lv_obj_del(current_screen); // ɾ����ǰ��Ļ�������ж���
        current_screen = NULL; // ���ָ��
    }
}

void event_next_button(lv_event_t * e) {
    create_screen2(); // �л�����Ļ2
}

void event_back_button(lv_event_t * e) {
    create_screen1(); // �л�����Ļ1
}


// ������Ļ1
void create_screen1(void) {
    // ɾ������Ļ
    delete_current_screen();

    // ��������Ļ1��������ʽ
    lv_obj_t * screen1 = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen1, lv_color_hex(0xFFFFFF), 0); 

    // ���������ǩ
    lv_obj_t * label1 = lv_label_create(screen1);
    lv_label_set_text(label1, "This is Screen 1");
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, -20);

    // �����л�����Ļ2�İ�ť
    lv_obj_t * btn_next = lv_btn_create(screen1);
    lv_obj_align(btn_next, LV_ALIGN_CENTER, 0, 20);
    lv_obj_t * label_next = lv_label_create(btn_next);
    lv_label_set_text(label_next, "Go to Screen 2");
    lv_obj_add_event_cb(btn_next, event_next_button, LV_EVENT_CLICKED, NULL);

    current_screen = screen1; // ���µ�ǰ��Ļ
    lv_scr_load(screen1); // ��������Ļ
}

// ������Ļ2
void create_screen2(void) {
    // ɾ������Ļ
    delete_current_screen();

    // ��������Ļ2��������ʽ
    lv_obj_t * screen2 = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen2, lv_color_hex(0xFFFFFF), 0); 

    // ���������ǩ
    lv_obj_t * label2 = lv_label_create(screen2);
    lv_label_set_text(label2, "This is Screen 2");
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, -20);

    // �����л�����Ļ1�İ�ť
    lv_obj_t * btn_back = lv_btn_create(screen2);
    lv_obj_align(btn_back, LV_ALIGN_CENTER, 0, 20);
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Back to Screen 1");
    lv_obj_add_event_cb(btn_back, event_back_button, LV_EVENT_CLICKED, NULL);

    current_screen = screen2; // ���µ�ǰ��Ļ
    lv_scr_load(screen2); // ��������Ļ
}

