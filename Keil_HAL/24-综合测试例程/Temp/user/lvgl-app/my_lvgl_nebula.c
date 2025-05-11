#include "my_lvgl_nebula.h"
#include "lvgl.h"

#include "my_lvgl_anim.h"

const lv_point_precise_t line2_points[] = {{60, 80}, {180, 80}};		  // ����㼯��
#define LINE2_POINTS_NUM (sizeof(line2_points) / sizeof(line2_points[0])) // �����ĸ���
void create_buttons(lv_obj_t * parent);

lv_obj_t *my_lvgl_nebula(void) // ��ں���
{
	/****************************************************************************************************************
	 *�����ϰ벿��ɫ����
	 *
	 ****************************************************************************************************************/
	lv_obj_t *scr = lv_screen_active();	   // ��ȡ��ǰ��Ļ����
	lv_obj_t *line2 = lv_line_create(scr); // ������������

	lv_obj_set_pos(line2, 0, 0);							   // ������������
	lv_line_set_points(line2, line2_points, LINE2_POINTS_NUM); // ��������㼯��,ͬʱҲ���ڴ��ڲ��������������Ĵ�С

	static lv_style_t line2_style;											 // ����������ʽ
	lv_style_init(&line2_style);											 // ��ʼ��������ʽ
	lv_style_set_line_color(&line2_style, lv_palette_main(LV_PALETTE_GREY)); // ������������ɫ
	lv_style_set_line_width(&line2_style, 4);								 // �����������
	lv_style_set_line_rounded(&line2_style, true);							 ////����������ĩ��ΪԲ��
	lv_obj_add_style(line2, &line2_style, 0);								 // ������ʽ
	/****************************************************************************************************************
	 *�����ϰ벿��ɫ��������
	 *
	 ****************************************************************************************************************/

	lv_anim_t a;				// ������������
	lv_anim_init(&a);			// ��ʼ��
	lv_anim_set_var(&a, line2); // ��������
	lv_anim_set_values(&a, -80, 0);
	lv_anim_set_duration(&a, 1000);
	lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
	lv_anim_set_path_cb(&a, lv_anim_path_overshoot);
	lv_anim_start(&a);

	/****************************************************************************************************************
	 *�����ϰ벿��ǩ��nebula-pi
	 *
	 ****************************************************************************************************************/
	lv_obj_t *label1 = lv_label_create(scr); // ������ǩ
	lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);
	lv_label_set_text(label1, "nebula-pi"); // ������ǩ����
	lv_obj_set_pos(label1, 67, 48);			// ���ñ�ǩλ��

	static lv_style_t label1_style;											 // ��ǩ��ʽ
	lv_style_init(&label1_style);											 // ��ʼ����ǩ��ʽ
	lv_style_set_text_color(&label1_style, lv_palette_main(LV_PALETTE_RED)); // ��ǩ�ı���ɫ
	lv_style_set_text_font(&label1_style, &lv_font_montserrat_22);			 // ��ǩ�ı�����
	lv_obj_add_style(label1, &label1_style, 0);								 // ������ǩ��ʽ
	/****************************************************************************************************************
	 *�����ϰ벿��ǩ��nebula-pi����
	 *
	 ****************************************************************************************************************/

	lv_anim_t b;				 // ������������
	lv_anim_init(&b);			 // ��ʼ��
	lv_anim_set_var(&b, label1); // ��������
	lv_anim_set_values(&b, 0, 48);
	lv_anim_set_duration(&b, 1500);
	lv_anim_set_exec_cb(&b, (lv_anim_exec_xcb_t)lv_obj_set_y);
	lv_anim_set_path_cb(&b, lv_anim_path_overshoot); // lv_anim_path_overshoot
	lv_anim_start(&b);

	/****************************************************************************************************************
	 *�����в�����ǩ��powered by:LVGL 9.2
	 *
	 ****************************************************************************************************************/
	lv_obj_t *label_lvgl = lv_label_create(scr); // ������ǩ
	lv_label_set_long_mode(label_lvgl, LV_LABEL_LONG_SCROLL_CIRCULAR);
	lv_obj_set_width(label_lvgl, 80);
	lv_label_set_text(label_lvgl, "powered by:LVGL 9.2     "); // ������ǩ����
	lv_obj_align(label_lvgl, LV_ALIGN_CENTER, 0, -60);

	static lv_style_t label_lvgl_style;											 // ��ǩ��ʽ
	lv_style_init(&label_lvgl_style);											 // ��ʼ����ǩ��ʽ
	lv_style_set_text_color(&label_lvgl_style, lv_palette_main(LV_PALETTE_RED)); // ��ǩ�ı���ɫ
	lv_style_set_text_font(&label_lvgl_style, &lv_font_montserrat_12);			 // ��ǩ�ı�����
	lv_obj_add_style(label_lvgl, &label_lvgl_style, 0);							 // ������ǩ��ʽ
	/****************************************************************************************************************
	 *���ñ���ɫ
	 *
	 ****************************************************************************************************************/
//	lv_obj_set_style_bg_color(scr, lv_color_hex(0xFFFFFF), 0); // ���ý�����ʼɫ
//	lv_obj_set_style_bg_grad_color(scr,lv_color_hex(0xFFFFFF), 0); // ���ý������ɫ
//	lv_obj_set_style_bg_grad_dir(scr, LV_GRAD_DIR_VER, 0); // ����Ϊ��ֱ����
//	lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0); // ���ñ�����͸��
	

lv_obj_set_style_bg_color(scr, lv_color_white(), 0); // ����ɫΪ��ɫ

	// ����ͼ��
	LV_IMG_DECLARE(logo);
	lv_obj_t * pattern = lv_img_create(scr);
	lv_img_set_src(pattern, &logo); // �����ͼ��ͼƬ�滻Ϊʵ��ͼ��
	lv_obj_align(pattern, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_size(pattern, 100, 100);	
	lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0); // ���ñ�����͸��
	
	/****************************************************************************************************************
	 *�����°벿��ǩ��RYMCU
	 *
	 ****************************************************************************************************************/
	lv_obj_t *label2 = lv_label_create(scr); // ������ǩ
	lv_label_set_long_mode(label2, LV_LABEL_LONG_WRAP);
	lv_label_set_text(label2, "RYMCU"); // ������ǩ����
	lv_obj_set_pos(label2, 80, 240);	// ���ñ�ǩλ��

	static lv_style_t label2_style;												// ��ǩ��ʽ
	lv_style_init(&label2_style);												// ��ʼ����ǩ��ʽ
	lv_style_set_text_color(&label2_style, lv_palette_main(LV_PALETTE_ORANGE)); // ��ǩ�ı���ɫ
	lv_style_set_text_font(&label2_style, &lv_font_montserrat_22);				// ��ǩ�ı�����
	lv_obj_add_style(label2, &label2_style, 0);									// ������ǩ��ʽ
	/****************************************************************************************************************
	 *�����ϰ벿��ǩ��RYMCU����
	 *
	 ****************************************************************************************************************/

	lv_anim_t c;				 // ������������
	lv_anim_init(&c);			 // ��ʼ��
	lv_anim_set_var(&c, label2); // ��������
	lv_anim_set_values(&c, -100, 80);
	lv_anim_set_duration(&c, 1500);
	lv_anim_set_exec_cb(&c, (lv_anim_exec_xcb_t)lv_obj_set_x);
	lv_anim_set_path_cb(&c, lv_anim_path_overshoot); // lv_anim_path_overshoot
	lv_anim_start(&c);
	/****************************************************************************************************************
	 *�����ϰ벿�˵���ǩ��ʹ��ϵͳͼ������
	 ****************************************************************************************************************/
	lv_obj_t *label3 = lv_label_create(scr); // ������ǩ
	lv_label_set_long_mode(label3, LV_LABEL_LONG_WRAP);
	lv_label_set_text(label3, LV_SYMBOL_WIFI " " LV_SYMBOL_BATTERY_3); // ������ǩ����
	lv_obj_set_pos(label3, 198, 5);									   // ���ñ�ǩλ��

	lv_obj_t *label4 = lv_label_create(scr); // ������ǩ
	lv_label_set_long_mode(label4, LV_LABEL_LONG_WRAP);
	lv_label_set_text(label4, LV_SYMBOL_CALL " " LV_SYMBOL_GPS); // ������ǩ����
	lv_obj_set_pos(label4, 8, 5);								 // ���ñ�ǩλ��
	static lv_style_t label3_4_style;
	
	lv_style_init(&label3_4_style);//��ʼ����ǩ��ʽ
	lv_style_set_text_color(&label3_4_style, lv_palette_main(LV_PALETTE_DEEP_PURPLE));//��ǩ�ı���ɫ
	lv_style_set_text_font(&label3_4_style,&lv_font_montserrat_12);//��ǩ�ı�����
	lv_obj_add_style(label3, &label3_4_style, 0);//������ǩ��ʽ
	lv_obj_add_style(label4, &label3_4_style, 0);//������ǩ��ʽ
	/****************************************************************************************************************
	 *�����м�˵���ǩ��ʹ���Զ���ͼ������
	 ****************************************************************************************************************/

	// create_heartbeat_image(scr);
	create_buttons(scr);
	return scr;
}
// ��ť�¼�������
static void btn_event_handler(lv_event_t * e)
{
    lv_obj_t *btn = lv_event_get_target(e); // ��ȡ�¼�Դ���󣨰�ť��
    const char *label = lv_label_get_text(lv_obj_get_child(btn, NULL)); // ��ȡ��ť�ϵı�ǩ

    // �������¼�
    if (lv_strcmp(label, MY_ICON_RGB) == 0) {
        // ��ť 1 �����
        load_my_page(2);
    } else if (lv_strcmp(label,MY_ICON_SET) == 0) {
        // ��ť 2 �����
        load_my_page(3);
    } else if (lv_strcmp(label, MY_ICON_UART) == 0) {
        // ��ť 3 �����
        load_my_page(4);
    } else if (lv_strcmp(label, MY_ICON_MUYU_S) == 0) {
        // ��ť 4 �����
        load_my_page(5);
    }
}
// ������ť����
void create_buttons(lv_obj_t * parent)
{
    const char *button_labels[] = 
		{
        MY_ICON_RGB, 
        MY_ICON_SET, 
        MY_ICON_UART, 
        MY_ICON_MUYU_S
    };

	static lv_style_t label5_style;											  // ��ǩ��ʽ
	lv_style_init(&label5_style);											  // ��ʼ����ǩ��ʽ
	lv_style_set_text_color(&label5_style, lv_palette_main(LV_PALETTE_BLUE)); // ��ǩ�ı���ɫ
	lv_style_set_text_font(&label5_style, &my_web_font_30);					  // ��ǩ�ı����壬�Զ�������
		static lv_obj_t * btn;
		static lv_obj_t * label;
    for (int i = 0; i < 4; i++)
		{
        // ������ť
         btn = lv_btn_create(parent);
        lv_obj_set_size(btn, 52, 55); // ���ð�ť��С
        lv_obj_align(btn, LV_ALIGN_CENTER,(i-1.5)*55,0); // ���ð�ť��ֱ����

				 label= lv_label_create(btn);
				lv_obj_add_style(label, &label5_style, 0);								  // ������ǩ��ʽ
			lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
        // ������ǩ�������ı�
        lv_label_set_text(label, button_labels[i]); // ���ñ�ǩ�ı�
        lv_obj_center(label); // ʹ��ǩ����
				// ���ð�ť��͸����ʽ
				lv_obj_set_style_bg_color(btn, lv_color_hex(0xFFFFFF), 0);	   // ������ɫ
				lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, 0);				   // ���ñ���͸����
				lv_obj_set_style_border_color(btn, lv_color_hex(0xFFFFFF), 0); // �߿���ɫ
				lv_obj_set_style_border_opa(btn, LV_OPA_TRANSP, 0);			   // �߿�͸��
				lv_obj_set_style_outline_width(btn, 0, 0);					   // ȥ��������
				lv_obj_set_style_shadow_width(btn, 0, 0);					   // ȥ����Ӱ
        // Ϊ��ť��ӵ���¼�����
        lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_CLICKED, NULL);
    }
}
