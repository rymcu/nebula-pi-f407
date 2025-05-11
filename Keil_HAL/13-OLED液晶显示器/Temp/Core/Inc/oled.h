#ifndef __OLED_H
#define __OLED_H		

#include "stm32f1xx_hal.h"
//#include "stm32f1xx_hal_i2c.h"
#define  u8 unsigned char 
#define  u32 unsigned int 

// extern I2C_HandleTypeDef hi2c1;
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define OLED_MODE 0

 
#define Max_Column	128  

//-----------------OLED端口定义----------------  					   

void delay_ms(unsigned int ms);


//OLED控制用函数  
void OLED_Init(void);//初始化
void OLED_Display_On(void);//开启OLED
void OLED_Display_Off(void);//关闭OLED	   							   		    
void OLED_Clear(void);//清屏
void OLED_On(void);//OLED点亮，全部显示

void OLED_Set_Pos(unsigned char x, unsigned char y);//设置坐标


void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size,u8 flag);//显示字符
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 Char_Size,u8 flag);//显示数字
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size,u8 flag);//显示字符串
void OLED_ShowCHinese(u8 x,u8 y,u8 Num,u8 flag);//显示中文字符
void OLED_DrawBMP(u8 x0,u8 y0,u8 BMP[],u8 flag);//显示图片

void OLED_ClearGram(void);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);  //点亮或熄灭屏幕上的某一个区域
void OLED_DrawPoint(u8 x,u8 y,u8 t);//点亮屏幕上的某一点
void OLED_Refresh_Gram(void);
void OLED_DrawLine(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);
#endif  
