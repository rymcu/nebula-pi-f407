#include "my_lvgl_anim.h"
#include "rtthread.h"
#include "my_lvgl_nebula.h"		 //��ҳ��ʵ��
#include "my_lvgl_rgb.h"		 //RGB����ҳ��ʵ��
#include "my_lvgl_temperature.h" //�¶Ƚ���
#include "my_lvgl_uart.h"		 //���ڽ���
#include "my_lvgl_led.h"		 //���8��led
#include "bsp_rgb.h"
LV_FONT_DECLARE(my_muyu)

lv_obj_t *one=NULL, *two=NULL, *three=NULL, *four=NULL, *five=NULL;
//static lv_obj_t *one, *two, *three, *four, *five;
#define MY_ICON_MUYU "\xEE\x98\x9E"

static lv_obj_t *five_btn;
static lv_obj_t *label5, *plus_one, *plus_sum;


static lv_obj_t *home_label;
static lv_style_t home_style; // ��ǩ��ʽ
int counter = 0;			  // ��������

void my_set_bg(lv_obj_t *obj)
{
	lv_obj_set_style_bg_color(obj, lv_color_hex(0x0), 0); 
	lv_obj_set_style_bg_grad_color(obj,lv_color_hex(0x0), 0); 
	lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, 0); 
	lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0); 
}

// ���������¼�������
static void button_event_handler_press(lv_event_t *e)
{
	// ��ȡ��ť����
	// lv_obj_t * button = lv_event_get_target(e);

	// �ı��ǩ��ɫ
	lv_obj_set_style_text_color(label5, lv_color_hex(0xFFFFFF), 0); // ����Ϊ��ɫ
	lv_obj_remove_flag(plus_one, LV_OBJ_FLAG_HIDDEN);
	lv_label_set_text_fmt(plus_sum, "%d", counter++);

	ry_set_rgb(counter % 10 * 25, 0, 0); // ���ÿ�����RGB��
}

// �����ɿ��¼�������
static void button_event_handler_release(lv_event_t *e)
{
	// ��ȡ��ť����
	// lv_obj_t * button = lv_event_get_target(e);

	// �ָ���ǩ��ɫ
	lv_obj_set_style_text_color(label5, lv_color_hex(0xFF0000), 0); // �ָ�Ϊ��ɫ
	lv_obj_add_flag(plus_one, LV_OBJ_FLAG_HIDDEN);
}

typedef struct my_page
{
	uint32_t obj_addr;
} my_page_t;

lv_ll_t ll_test;
my_page_t *page_temp;
/*--------------------------------------------------------------------------------------------------------
*��ʼ��������ҳ��������ӵ�������ָ��ͷ�ڵ�
*
--------------------------------------------------------------------------------------------------------*/
void ry_list_init(void)
{
	_lv_ll_init(&ll_test, sizeof(my_page_t));

	page_temp = _lv_ll_ins_head(&ll_test);
	page_temp->obj_addr = (uint32_t)five;

	page_temp = _lv_ll_ins_head(&ll_test);
	page_temp->obj_addr = (uint32_t)four;
	page_temp = _lv_ll_ins_head(&ll_test);
	page_temp->obj_addr = (uint32_t)three;
	page_temp = _lv_ll_ins_head(&ll_test);
	page_temp->obj_addr = (uint32_t)two;
	page_temp = _lv_ll_ins_head(&ll_test);
	page_temp->obj_addr = (uint32_t)one;

	page_temp = _lv_ll_get_head(&ll_test);
}
/*--------------------------------------------------------------------------------------------------------
*��ȡ�ڵ�
*
--------------------------------------------------------------------------------------------------------*/
void ry_get_prev(void)
{
	page_temp = _lv_ll_get_prev(&ll_test, page_temp);
	if (page_temp == NULL)
	{
		//		page_temp = _lv_ll_get_head(&ll_test);
		//		LV_LOG_USER("the begin page,no more!");
		page_temp = _lv_ll_get_tail(&ll_test); // ѭ��ģʽ
		LV_LOG_USER("the end page!");
	}
}
void ry_get_next(void)
{
	page_temp = _lv_ll_get_next(&ll_test, page_temp);
	if (page_temp == NULL)
	{
		// page_temp = _lv_ll_get_tail(&ll_test);
		// LV_LOG_USER("the end page,no more!");

		page_temp = _lv_ll_get_head(&ll_test); // ѭ��ģʽ
		LV_LOG_USER("the begin page!");
	}
}
/*--------------------------------------------------------------------------------------------------------
*�����¼�����
*
--------------------------------------------------------------------------------------------------------*/
void ry_gesture_event(lv_event_t *e)
{/*
	lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act()); // ��ȡ���Ʒ���
	lv_obj_t *target_obj = lv_event_get_target_obj(e);

	switch (dir)
	{
	case LV_DIR_TOP: // ��������
	{
		ry_get_prev();
		lv_scr_load_anim((lv_obj_t *)(page_temp->obj_addr), LV_SCR_LOAD_ANIM_OUT_TOP, 1000, 0, false);
		break;
	}
	case LV_DIR_BOTTOM: // ��������
	{
		ry_get_next();
		lv_scr_load_anim((lv_obj_t *)(page_temp->obj_addr), LV_SCR_LOAD_ANIM_OUT_BOTTOM, 1000, 0, false);
		break;
	}
	case LV_DIR_LEFT: // ��������
	{
		// ȡ��RGBҳ�����һ�������
		if ((lv_obj_t *)(page_temp->obj_addr) == two)
			break;
		ry_get_next();
		lv_scr_load((lv_obj_t *)(page_temp->obj_addr)); // ��ʾ
		// lv_scr_load_anim((lv_obj_t *)(page_temp->obj_addr), LV_SCR_LOAD_ANIM_FADE_IN, 2000, 0, false);
		break;
	}
	case LV_DIR_RIGHT: // ��������
	{
		// ȡ��RGBҳ�����һ�������
		if ((lv_obj_t *)(page_temp->obj_addr) == two)
			break;
		ry_get_prev();
		lv_scr_load((lv_obj_t *)(page_temp->obj_addr)); // ��ʾ
		// lv_scr_load_anim((lv_obj_t *)(page_temp->obj_addr), LV_SCR_LOAD_ANIM_FADE_IN, 2000, 0, false);
		break;
	}
	default:
		break;
	}*/
}
/*--------------------------------------------------------------------------------------------------------
*ÿ��ҳ������½���ʾҳ������
*
--------------------------------------------------------------------------------------------------------*/
void create_page_label(_lv_obj_t *obj, uint8_t i)
{
	lv_obj_t *label = lv_label_create(obj);
	lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -20);
	
	static lv_style_t style;
  lv_style_init(&style);
	lv_style_set_text_color(&style, lv_color_hex(0xFFFFFF)); // �����ı���ɫΪ��ɫ
	lv_obj_add_style(label, &style, 0); // 0��ʾ��һ����ʽ
	
	switch (i)
	{
	case 1:
		lv_label_set_text(label, "1");
		break;
	case 2:
		lv_label_set_text(label, "2");
		break;
	case 3:
		lv_label_set_text(label, "3");
		break;
	case 4:
		lv_label_set_text(label, "4");
		break;
	case 5:
		lv_label_set_text(label, "5");
		break;
	default:
		break;
	}
}

void back_home(lv_event_t *e)
{
	page_temp = _lv_ll_get_head(&ll_test); // back to homepage
	lv_scr_load_anim((lv_obj_t *)(page_temp->obj_addr), LV_SCR_LOAD_ANIM_FADE_IN, 0, 0, false);
	LV_LOG_USER("back to homepage!");
}
void create_home_button(lv_obj_t *parent)
{
	// ����һ����ť
	lv_obj_t *btn = lv_btn_create(parent);

	// ���ð�ť��С��λ��
	lv_obj_set_size(btn, 30, 30);				 // �趨��ť�Ŀ�Ⱥ͸߶�
	lv_obj_align(btn, LV_ALIGN_TOP_RIGHT, 0, 0); // ���϶���

	// ���ð�ť��͸����ʽ
	lv_obj_set_style_bg_color(btn, lv_color_hex(0xFFFFFF), 0);	   // ������ɫ
	lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, 0);				   // ���ñ���͸����
	lv_obj_set_style_border_color(btn, lv_color_hex(0xFFFFFF), 0); // �߿���ɫ
	lv_obj_set_style_border_opa(btn, LV_OPA_TRANSP, 0);			   // �߿�͸��
	lv_obj_set_style_outline_width(btn, 0, 0);					   // ȥ��������
	lv_obj_set_style_shadow_width(btn, 0, 0);					   // ȥ����Ӱ

	// ����¼�������
	lv_obj_add_event_cb(btn, back_home, LV_EVENT_CLICKED, NULL);

	home_label = lv_label_create(btn); // ������ǩ
	lv_label_set_long_mode(home_label, LV_LABEL_LONG_WRAP);
	lv_label_set_text(home_label, LV_SYMBOL_HOME); // ������ǩ����
	lv_obj_align(home_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_pos(home_label, 0, 0); // ���ñ�ǩλ��

	lv_style_init(&home_style);												// ��ʼ����ǩ��ʽ
	lv_style_set_text_color(&home_style, lv_palette_main(LV_PALETTE_BLUE)); // ��ǩ�ı���ɫ
	lv_style_set_text_font(&home_style, &lv_font_montserrat_12);			// ��ǩ�ı����壬�Զ�������
	lv_obj_add_style(home_label, &home_style, 0);							// ������ǩ��ʽ
}

/*--------------------------------------------------------------------------------------------------------
*����5��С�ƣ�ָʾҳ��״̬
*
--------------------------------------------------------------------------------------------------------*/
// ����LED�Ƶĺ���
lv_obj_t *create_led(lv_obj_t *parent, int x, int y, lv_color_t color)
{
	// ����һ��Բ�ζ���
	lv_obj_t *led = lv_obj_create(parent);

	// ���ô�СΪ5x5
	lv_obj_set_size(led, 5, 5);

	// ����Բ����ʽ
	lv_obj_set_style_radius(led, LV_RADIUS_CIRCLE, 0); // ʹ��ΪԲ��
	lv_obj_set_style_bg_color(led, color, 0);		   // ������ɫ
	lv_obj_set_style_bg_opa(led, LV_OPA_COVER, 0);	   // ��Ϊ��͸��

	// ����λ��
	lv_obj_set_pos(led, x, y);

	return led;
}

void create_leds(lv_obj_t *parent, uint8_t page_num)
{
	// ��ȡ��Ļ���
	int screen_width = lv_obj_get_width(parent);

	// LED�������ʹ�С
	int led_count = 5;
	int led_size = 5;
	int spacing = 5; // LED�Ƶļ��

	// �����ܿ��
	int total_width = (led_count * led_size) + ((led_count - 1) * spacing);

	// ������ʼλ�ã�ȷ��LED�ƾ���
	int start_x = (screen_width - total_width) / 2;
	int start_y = lv_obj_get_height(parent) - 20; // ���þ���ײ��ĸ߶�

	// ����5��LED�Ʋ��������ǵ�λ�ú���ɫ
	for (int i = 0; i < led_count; i++)
	{
		int x_position = start_x + i * (led_size + spacing);
		// ������ɫ��������ÿ��LED�Ƶ���ɫ
		if (i == (page_num - 1))
			create_led(parent, x_position, start_y, lv_palette_main(LV_PALETTE_RED));
		else
			create_led(parent, x_position, start_y, lv_palette_main(LV_PALETTE_BLUE));
	}
}

/*--------------------------------------------------------------------------------------------------------
*����4����ͬ��ɫҳ�棬�����������¼�
*
--------------------------------------------------------------------------------------------------------*/
void create_page1(void)
{
	one = my_lvgl_nebula();
	if(one == NULL)
	{
		//rt_kprintf("one fail\n");
		return;
	}
	//rt_kprintf("one ok\n");
	// create_home_button(one);//��ҳ����Ҫ
	create_leds(one, 1);
	create_page_label(one, 1);
	//lv_obj_add_event_cb(one, ry_gesture_event, LV_EVENT_GESTURE, NULL);
	//my_set_bg(one);
}
void create_page2(void)
{
	two = my_lvgl_rgb();
	if(two == NULL)
	{
		rt_kprintf("two fail\n");
		return;
	}
	//rt_kprintf("two ok\n");
	create_home_button(two);
	create_leds(two, 2);
	create_page_label(two, 2);
	create_leds_8(two);
	//lv_obj_add_event_cb(two, ry_gesture_event, LV_EVENT_GESTURE, NULL);
	my_set_bg(two);
}
void create_page3(void)
{
	three = my_lvgl_temperature();
	if(three == NULL)
	{
		rt_kprintf("three fail\n");
		return;
	}
	//rt_kprintf("three ok\n");
	//three = lv_obj_create(NULL);
	
	create_home_button(three);
	create_leds(three, 3);
	create_page_label(three, 3);
//	lv_obj_add_event_cb(three, ry_gesture_event, LV_EVENT_GESTURE, NULL);
	my_set_bg(three);
}
void create_page4(void)
{
	four = my_lvgl_uart();
	//four = lv_obj_create(NULL);
		if(four == NULL)
	{
		rt_kprintf("four fail\n");
		return;
	}
	//rt_kprintf("four OK\n");

	create_home_button(four);
	create_leds(four, 4);
	create_page_label(four, 4);
//	lv_obj_add_event_cb(four, ry_gesture_event, LV_EVENT_GESTURE, NULL);
	my_set_bg(four);
}

void create_page5(void)
{
	five = lv_obj_create(NULL);															 
		if(five == NULL)
	{
		rt_kprintf("five fail\n");
		return;
	}
	//rt_kprintf("five ok\n");
	//lv_obj_set_style_bg_color(five, lv_palette_main(LV_PALETTE_GREY), LV_STATE_DEFAULT); 
	// lv_obj_set_size(five, 100, 100); 
	// lv_obj_align(five, LV_ALIGN_CENTER, 0, 0);        

	five_btn = lv_btn_create(five);
	lv_obj_align(five_btn, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_size(five_btn, 80, 80); 

	
	lv_obj_set_style_bg_color(five_btn, lv_color_hex(0xFFFFFF), 0);		
	lv_obj_set_style_bg_opa(five_btn, LV_OPA_TRANSP, 0);				
	lv_obj_set_style_border_color(five_btn, lv_color_hex(0xFFFFFF), 0); 
	lv_obj_set_style_border_opa(five_btn, LV_OPA_TRANSP, 0);			
	lv_obj_set_style_outline_width(five_btn, 0, 0);						
	lv_obj_set_style_shadow_width(five_btn, 0, 0);						

	label5 = lv_label_create(five); 
	lv_label_set_long_mode(label5, LV_LABEL_LONG_WRAP);
	lv_obj_align(label5, LV_ALIGN_CENTER, 0, 0); 
	lv_label_set_text(label5, MY_ICON_MUYU);	 

	static lv_style_t label5_style;											 
	lv_style_init(&label5_style);											 
	lv_style_set_text_color(&label5_style, lv_palette_main(LV_PALETTE_RED)); 
	lv_style_set_text_font(&label5_style, &my_muyu);						
	lv_obj_add_style(label5, &label5_style, 0);								 

	lv_obj_add_event_cb(five_btn, button_event_handler_press, LV_EVENT_PRESSED, NULL);
	lv_obj_add_event_cb(five_btn, button_event_handler_release, LV_EVENT_RELEASED, NULL);
	
	plus_one = lv_label_create(five);
	lv_label_set_long_mode(plus_one, LV_LABEL_LONG_WRAP);
	lv_label_set_text(plus_one, "+1"); // 
	
	static lv_style_t style;
  lv_style_init(&style);
	lv_style_set_text_color(&style, lv_color_hex(0x00FF00)); // 
	lv_obj_add_style(plus_one, &style, 0); // 
	lv_obj_set_pos(plus_one, 80, 100); // 
	// lv_obj_set_hidden(plus_one,true);
	lv_obj_add_flag(plus_one, LV_OBJ_FLAG_HIDDEN);
	// 
	plus_sum = lv_label_create(five);
	lv_label_set_long_mode(plus_sum, LV_LABEL_LONG_WRAP);
	lv_label_set_text_fmt(plus_sum, "%d", counter); // ��ʼ��ʱ��ʾ��ʼֵ
	lv_obj_align(plus_sum, LV_ALIGN_TOP_MID, 0, 60);

	static lv_style_t plus_sum_style;											  // ��ǩ��ʽ
	lv_style_init(&plus_sum_style);												  // ��ʼ����ǩ��ʽ
	lv_style_set_text_color(&plus_sum_style, lv_palette_main(LV_PALETTE_ORANGE)); // ��ǩ�ı���ɫ
	lv_style_set_text_font(&plus_sum_style, &lv_font_montserrat_48);			  // ��ǩ�ı�����
	lv_obj_add_style(plus_sum, &plus_sum_style, 0);								  // ������ǩ��ʽ

	// five = my_lvgl_uart();
	create_leds(five, 5);
	create_page_label(five, 5);
	create_home_button(five);
//	lv_obj_add_event_cb(five, ry_gesture_event, LV_EVENT_GESTURE, NULL);
	
		/****************************************************************************************************************
	 *���ñ���ɫ
	 *
	 ****************************************************************************************************************/
	lv_obj_set_style_bg_color(five, lv_color_hex(0x0), 0); // ���ý�����ʼɫ
	lv_obj_set_style_bg_grad_color(five,lv_color_hex(0x0), 0); // ���ý������ɫ
	lv_obj_set_style_bg_grad_dir(five, LV_GRAD_DIR_VER, 0); // ����Ϊ��ֱ����
	lv_obj_set_style_bg_opa(five, LV_OPA_COVER, 0); // ���ñ�����͸��
	my_set_bg(five);
}
//�����л�ҳ��
void load_my_page(uint8_t page)
{
	switch (page) {
			case 1:
					lv_scr_load(one);
					break;
			case 2:
				 lv_scr_load(two);
					break;
			case 3:
					lv_scr_load(three);
					break;
			case 4:
				 lv_scr_load(four);
					break;  
			case 5:
				 lv_scr_load(five);
					break;  		
			default:
					break;
	}
}
void lv_example_anim_1(void)
{
	create_page1();
	create_page2();
	create_page3();
	create_page4();
	create_page5();
	//rt_kprintf("after create_page5\n");
	ry_list_init(); // ��ʼ������
	//rt_kprintf("after ry_list_init\n");
	lv_scr_load(one); // ��ʾpage1
	//rt_kprintf("after lv_scr_load\n");

	//LV_LOG_USER("begin");
}
