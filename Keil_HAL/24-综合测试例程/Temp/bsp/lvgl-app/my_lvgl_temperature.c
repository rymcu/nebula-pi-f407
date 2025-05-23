#include "my_lvgl_temperature.h"
#include "adc.h"
#include "stdio.h"

static lv_obj_t * temperature_label; // 温度显示标签
static lv_obj_t * temperature_screen;
static float current_temperature; // 当前温度

// 样式初始化


// 创建表格以显示STM32开发板信息
void create_product_info_table(lv_obj_t *parent) {
    // 创建表格对象
    lv_obj_t *table = lv_table_create(parent);
    lv_obj_set_size(table, 240, 320); // 设置表格大小以适应屏幕
    lv_obj_align(table, LV_ALIGN_CENTER, 0, 0); // 将表格居中

    // 设置行和列的数量
    lv_table_set_col_cnt(table, 2);
    lv_table_set_row_cnt(table, 4); // 4 行用于显示规格

    // 设置表头
    lv_table_set_cell_value(table, 0, 0, "Specification");
    lv_table_set_cell_value(table, 0, 1, "Details");

    // 填充STM32开发板规格信息
    lv_table_set_cell_value(table, 1, 0, "Processor");
    lv_table_set_cell_value(table, 1, 1, "STM32F103VE");

    lv_table_set_cell_value(table, 2, 0, "Memory");
    lv_table_set_cell_value(table, 2, 1, "512KB Flash");

    lv_table_set_cell_value(table, 3, 0, "I/O");
    lv_table_set_cell_value(table, 3, 1, "37 GPIOs");
		
    lv_table_set_cell_value(table, 4, 0, "Compaly");
    lv_table_set_cell_value(table, 4, 1, "37 GPIOs");
    // 设置每列的宽度（可选）
    lv_table_set_col_width(table, 0, 120); // 规格列宽
    lv_table_set_col_width(table, 1, 120); // 详细信息列宽

    // 鍒涘缓涓�涓柊鐨勬牱寮�
    // 璁剧疆瀛椾綋棰滆壊涓鸿摑鑹�
    // 璁剧疆琛ㄦ牸鑳屾櫙棰滆壊涓烘祬鐏拌壊
		  // 璁剧疆鏂囨湰棰滆壊涓烘繁绱壊
    // 设置单元格样式
    //style_table_cells(table);

		lv_obj_set_style_bg_color(table, lv_color_hex(0x0), 0); 
	lv_obj_set_style_bg_grad_color(table,lv_color_hex(0x0), 0); 
	lv_obj_set_style_bg_grad_dir(table, LV_GRAD_DIR_VER, 0); 
	lv_obj_set_style_bg_opa(table, LV_OPA_COVER, 0); 
}

// 获取温度值
float  my_get_cpu_temperature(void)
{
    uint32_t Temp;//温度采样分层值
    float Vsense = 0.0;//温度采样电压值
    float Temperature = 0.0;//温度值
    //数据手册温度转换公式：T = ((V25-Vsense)/Avg_Slope) + 25
    float V25 = 1.43;//查阅手册获得
		 //uint16_t V25 = *(__IO uint16_t*)0x1FFFF7B8;  // 25℃时的电压校准值
    float Avg_Slope = 0.0043;//4.3mV/摄氏度
		//int16_t Avg_Slope = *(__IO int16_t*)0x1FFFF7C2;  // 温度斜率校准值
    //LV_LOG_USER("\r\n\r\n------------------MCU内部温度传感器测试------------------\r\n\r\n");
    //step1 启动ADC
    HAL_ADC_Start(&hadc1);
    //step2 温度采集转换
    HAL_ADC_PollForConversion(&hadc1,10);
    //step3 转换计算
    Temp = HAL_ADC_GetValue(&hadc1);//获取采样值分层值
   // Vsense = (Temp/4095.0)*3.3;//采样精度12bit,最大分层值4096
		Vsense = (float)Temp * 3.3f / 4096.0f;
    Temperature = ((V25-Vsense)/Avg_Slope) + 25;//按公式计算温度值

    //LV_LOG_USER("温度分层值：%d\r\n温度电压值：%f\r\n温度采样值：%0.3f\r\n",Temp,Vsense,Temperature);
	return Temperature;
}
char temp_str[20]= "25.1";
void my_timer_callback(lv_timer_t * timer)
{
	//char temp_str[20]= "25.1";
	current_temperature = my_get_cpu_temperature();
	
	sprintf(temp_str,"%0.1f", current_temperature); // 格式化字符串
	uint16_t tt = (uint16_t)current_temperature;
	lv_label_set_text(temperature_label, temp_str); // 更新标签文本
}	

void draw_thermometer(lv_obj_t * parent)
{

    // 缁樺埗娓╁害璁＄殑涓讳綋
    lv_obj_t * body = lv_obj_create(parent);
    lv_obj_set_size(body, 10, 60);
    //lv_obj_center_x(body);
    lv_obj_align_to(body, parent, LV_ALIGN_BOTTOM_LEFT, 40,-40);
    lv_obj_set_style_bg_color(body, lv_color_white(), 0);
    lv_obj_set_style_radius(body, LV_RADIUS_CIRCLE, 0);

    // 缁樺埗娓╁害璁＄殑搴曢儴鍦嗗舰
    lv_obj_t * bottom = lv_obj_create(parent);
    lv_obj_set_size(bottom, 30, 30);
    lv_obj_align_to(bottom, body, LV_ALIGN_BOTTOM_MID, 0, 10);
    lv_obj_set_style_bg_color(bottom, lv_color_white(), 0);
    lv_obj_set_style_radius(bottom, LV_RADIUS_CIRCLE, 0);

    // 缁樺埗娓╁害璁″唴鐨勬按閾舵煴
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
		
			lv_obj_t *label2 = lv_label_create(temperature_screen); // 源莰要签
	lv_label_set_long_mode(label2, LV_LABEL_LONG_WRAP);
	lv_label_set_text(label2, "Core temperature"); // 源莰要签蟿貣
	lv_obj_set_pos(label2, 60, 215);	// 狮變要签位變

	static lv_style_t label2_style;												// 要签夜式
	lv_style_init(&label2_style);												// 缘始郫要签夜式
	lv_style_set_text_color(&label2_style, lv_palette_main(LV_PALETTE_ORANGE)); // 要签蟿丫視色
	lv_style_set_text_font(&label2_style, &lv_font_montserrat_12);				// 要签蟿丫貣庭
	lv_obj_add_style(label2, &label2_style, 0);		
	return temperature_screen;
}
