#ifndef __U8G2_HAL_I2C_H
#define __U8G2_HAL_I2C_H

#include "stm32f1xx_hal.h"
#include "u8g2.h"

// ��ʼ������
void U8g2_Init(void);
// ��ȡU8g2����ָ�루�����ⲿ���û�ͼ������
u8g2_t* GetU8g2Ptr(void);

#endif /* __U8G2_HAL_I2C_H */
