#include "my_lvgl_temperature.h"
#include "adc.h"
#include "stdio.h"

static lv_obj_t * temperature_label; // �¶���ʾ��ǩ
static lv_obj_t * temperature_screen;
static float current_temperature; // ��ǰ�¶�

// ��ʽ��ʼ��


// �����������ʾSTM32��������Ϣ
void create_product_info_table(lv_obj_t *parent) {
    // ����������
    lv_obj_t *table = lv_table_create(parent);
    lv_obj_set_size(table, 240, 320); // ���ñ���С����Ӧ��Ļ
    lv_obj_align(table, LV_ALIGN_CENTER, 0, 0); // ��������

    // �����к��е�����
    lv_table_set_col_cnt(table, 2);
    lv_table_set_row_cnt(table, 4); // 4 ��������ʾ���

    // ���ñ�ͷ
    lv_table_set_cell_value(table, 0, 0, "Specification");
    lv_table_set_cell_value(table, 0, 1, "Details");

    // ���STM32����������Ϣ
    lv_table_set_cell_value(table, 1, 0, "Processor");
    lv_table_set_cell_value(table, 1, 1, "STM32F103VE");

    lv_table_set_cell_value(table, 2, 0, "Memory");
    lv_table_set_cell_value(table, 2, 1, "512KB Flash");

    lv_table_set_cell_value(table, 3, 0, "I/O");
    lv_table_set_cell_value(table, 3, 1, "37 GPIOs");
		
    lv_table_set_cell_value(table, 4, 0, "Compaly");
    lv_table_set_cell_value(table, 4, 1, "37 GPIOs");
    // ����ÿ�еĿ�ȣ���ѡ��
    lv_table_set_col_width(table, 0, 120); // ����п�
    lv_table_set_col_width(table, 1, 120); // ��ϸ��Ϣ�п�

    // 创建一个新的样式
    // 设置字体颜色为蓝色
    // 设置表格背景颜色为浅灰色
		  // 设置文本颜色为深紫色
    // ���õ�Ԫ����ʽ
    //style_table_cells(table);

		lv_obj_set_style_bg_color(table, lv_color_hex(0x0), 0); 
	lv_obj_set_style_bg_grad_color(table,lv_color_hex(0x0), 0); 
	lv_obj_set_style_bg_grad_dir(table, LV_GRAD_DIR_VER, 0); 
	lv_obj_set_style_bg_opa(table, LV_OPA_COVER, 0); 
}

// ��ȡ�¶�ֵ
float  my_get_cpu_temperature(void)
{
    uint32_t Temp;//�¶Ȳ����ֲ�ֵ
    float Vsense = 0.0;//�¶Ȳ�����ѹֵ
    float Temperature = 0.0;//�¶�ֵ
    //�����ֲ��¶�ת����ʽ��T = ((V25-Vsense)/Avg_Slope) + 25
    float V25 = 1.43;//�����ֲ���
		 //uint16_t V25 = *(__IO uint16_t*)0x1FFFF7B8;  // 25��ʱ�ĵ�ѹУ׼ֵ
    float Avg_Slope = 0.0043;//4.3mV/���϶�
		//int16_t Avg_Slope = *(__IO int16_t*)0x1FFFF7C2;  // �¶�б��У׼ֵ
    //LV_LOG_USER("\r\n\r\n------------------MCU�ڲ��¶ȴ���������------------------\r\n\r\n");
    //step1 ����ADC
    HAL_ADC_Start(&hadc1);
    //step2 �¶Ȳɼ�ת��
    HAL_ADC_PollForConversion(&hadc1,10);
    //step3 ת������
    Temp = HAL_ADC_GetValue(&hadc1);//��ȡ����ֵ�ֲ�ֵ
   // Vsense = (Temp/4095.0)*3.3;//��������12bit,���ֲ�ֵ4096
		Vsense = (float)Temp * 3.3f / 4096.0f;
    Temperature = ((V25-Vsense)/Avg_Slope) + 25;//����ʽ�����¶�ֵ

    //LV_LOG_USER("�¶ȷֲ�ֵ��%d\r\n�¶ȵ�ѹֵ��%f\r\n�¶Ȳ���ֵ��%0.3f\r\n",Temp,Vsense,Temperature);
	return Temperature;
}
char temp_str[20]= "25.1";
void my_timer_callback(lv_timer_t * timer)
{
	//char temp_str[20]= "25.1";
	current_temperature = my_get_cpu_temperature();
	
	sprintf(temp_str,"%0.1f", current_temperature); // ��ʽ���ַ���
	uint16_t tt = (uint16_t)current_temperature;
	lv_label_set_text(temperature_label, temp_str); // ���±�ǩ�ı�
}	

void draw_thermometer(lv_obj_t * parent)
{

    // 绘制温度计的主体
    lv_obj_t * body = lv_obj_create(parent);
    lv_obj_set_size(body, 10, 60);
    //lv_obj_center_x(body);
    lv_obj_align_to(body, parent, LV_ALIGN_BOTTOM_LEFT, 40,-40);
    lv_obj_set_style_bg_color(body, lv_color_white(), 0);
    lv_obj_set_style_radius(body, LV_RADIUS_CIRCLE, 0);

    // 绘制温度计的底部圆形
    lv_obj_t * bottom = lv_obj_create(parent);
    lv_obj_set_size(bottom, 30, 30);
    lv_obj_align_to(bottom, body, LV_ALIGN_BOTTOM_MID, 0, 10);
    lv_obj_set_style_bg_color(bottom, lv_color_white(), 0);
    lv_obj_set_style_radius(bottom, LV_RADIUS_CIRCLE, 0);

    // 绘制温度计内的水银柱
    lv_obj_t * mercury = lv_obj_create(body);
    lv_obj_set_size(mercury, 6, 40);
    lv_obj_align_to(mercury, body, LV_ALIGN_BOTTOM_MID, 0, -2);
    lv_obj_set_style_bg_color(mercury, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_set_style_radius(mercury, LV_RADIUS_CIRCLE, 0);
}
lv_obj_t* my_lvgl_temperature(void) 
	{
		 lv_timer_create(my_timer_callback, 1000, NULL); 
    temperature_screen = lv_obj_create(NULL); 
    lv_obj_set_style_bg_color(temperature_screen, lv_color_hex(0x0), 0); 
		
		create_product_info_table(temperature_screen);
    temperature_label = lv_label_create(temperature_screen); 
    lv_obj_align(temperature_label, LV_ALIGN_CENTER, 0, 100); 
		
		static lv_style_t temp_style;											  
	lv_style_init(&temp_style);												 
	lv_style_set_text_color(&temp_style, lv_palette_main(LV_PALETTE_ORANGE)); 
	lv_style_set_text_font(&temp_style, &lv_font_montserrat_48);			 
	lv_obj_add_style(temperature_label, &temp_style, 0);
		draw_thermometer(temperature_screen);
		
			lv_obj_t *label2 = lv_label_create(temperature_screen); // ԴݨҪǩ
	lv_label_set_long_mode(label2, LV_LABEL_LONG_WRAP);
	lv_label_set_text(label2, "Core temperature"); // ԴݨҪǩτؖ
	lv_obj_set_pos(label2, 60, 215);	// ʨ׃Ҫǩλ׃

	static lv_style_t label2_style;												// Ҫǩҹʽ
	lv_style_init(&label2_style);												// ԵʼۯҪǩҹʽ
	lv_style_set_text_color(&label2_style, lv_palette_main(LV_PALETTE_ORANGE)); // ҪǩτѾҕɫ
	lv_style_set_text_font(&label2_style, &lv_font_montserrat_12);				// ҪǩτѾؖͥ
	lv_obj_add_style(label2, &label2_style, 0);		
	return temperature_screen;
}
