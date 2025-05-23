#ifndef __BSP_FT6336_H__
#define __BSP_FT6336_H__

#include "bsp_driver_i2c.h"
//#include "rtthread.h"

// FT6336�Ĵ���
#define FT6336_REG_DEV_MODE 0x00
#define FT6336_REG_GEST_ID 0x01
#define FT6336_REG_TD_STATUS 0x02
#define FT6336_REG_P1_XH 0x03
#define FT6336_REG_P1_XL 0x04
#define FT6336_REG_P1_YH 0x05
#define FT6336_REG_P1_YL 0x06
#define FT6336_REG_P1_WEIGHT 0x07
#define FT6336_REG_P1_MISC 0x08
#define FT6336_REG_P2_XH 0x09
#define FT6336_REG_P2_XL 0x0A
#define FT6336_REG_P2_YH 0x0B
#define FT6336_REG_P2_YL 0x0C
#define FT6336_REG_P2_WEIGHT 0x0D
#define FT6336_REG_P2_MISC 0x0E
#define FT6336_REG_TH_GROUP 0x80
#define FT6336_REG_TH_DIFF 0x85
#define FT6336_REG_CTRL 0x86
#define FT6336_REG_TIMEENTERMONITOR 0x87
#define FT6336_REG_PERIODACTIVE 0x88
#define FT6336_REG_PERIODMONITOR 0x89
#define FT6336_REG_REG_RADIAN_VALUE 0x91
#define FT6336_REG_OFFSET_LEFT_RIGHT 0x92
#define FT6336_REG_OFFSET_UP_DOWN 0x93
#define FT6336_REG_DISTANCE_LEFT_RIGHT 0x94
#define FT6336_REG_DISTANCE_UP_DOWN 0x95
#define FT6336_REG_DISTANCE_ZOOM 0x97
#define FT6336_REG_LIB_VER_H 0xA1
#define FT6336_REG_LIB_VER_L 0xA2
#define FT6336_REG_LIB_CIPHER 0xA3
#define FT6336_REG_G_MODE 0xA4
#define FT6336_REG_POWER_MODE 0xA5
#define FT6336_REG_FIRMID 0xA6
#define FT6336_REG_FOCALTECH_ID 0xA8
#define FT6336_REG_ID_G_VIRTUAL_KEY_THERS 0xA9
#define FT6336_REG_ID_FACTORY_MODE 0xAE
#define FT6336_REG_RELEASE_CODE_ID 0xAF
#define FT6336_REG_ID_G_FACE_DEC_MODE 0xB0
#define FT6336_REG_ID_STATE 0xBC
#define FT6336_REG_ID_G_SPEC_GESTURE_ENABLE 0xD0
#define FT6336_REG_ID_G_SPEC_GESTURE_ENABLE_SIGN 0xD1
#define FT6336_REG_ID_G_SPEC_GESTURE_ID 0xD3

#define MODE_TRIGGER 0x01
#define MODE_POLLING 0x00
#define CTP_PENIRQ 0
#define CTP_PENIRQ_Read()  HAL_GPIO_ReadPin(CTP_PEN_GPIO_Port, CTP_PEN_Pin)

void FT6336_Init(void);
uint8_t FT6336_ReadID(uint8_t addr);
void FT6336_ReadReg(uint8_t addr, uint8_t *buf, uint8_t N);
void FT6336_Write_N_Data(uint8_t addr,uint8_t *buf,uint8_t N);
void FT6336_ReadXY(int32_t *x,int32_t *y,uint8_t scr_scan_mode);
uint8_t CTP_TouchDetect(void);
#endif 
