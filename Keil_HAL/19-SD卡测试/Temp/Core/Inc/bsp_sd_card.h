#ifndef __SD_CARD_H
#define __SD_CARD_H

#include "stm32f1xx_hal.h"

// SD�����Ͷ���
#define SD_TYPE_ERR     0x00
#define SD_TYPE_MMC     0x01
#define SD_TYPE_V1      0x02
#define SD_TYPE_V2      0x04
#define SD_TYPE_V2HC    0x06

// SD��ָ���
#define CMD0    0       // ����λ
#define CMD8    8       // ���ͽӿ�����
#define CMD55   55      // APP����ǰ׺
#define CMD41   41      // ��ʼ������
#define CMD58   58      // ��OCR�Ĵ���
#define CMD17   17      //����17����sector
#define CMD24   24      //����24��дsector
#define CMD9    9       //����9 ����CSD����
#define CMD12   12      //ֹͣ���ݴ���
// ȫ�ֱ����ͺ�������
extern SPI_HandleTypeDef hspi1;
extern uint8_t SD_Type;

uint8_t SD_Init(void);
uint8_t SD_ReadBlock(uint32_t block_addr, uint8_t *buf);
uint8_t SD_WriteBlock(uint32_t block_addr, const uint8_t *buf);
uint32_t SD_GetCapacity(void);
uint8_t SPI_ReadWriteByte(uint8_t data);
#endif /* __SD_CARD_H */
