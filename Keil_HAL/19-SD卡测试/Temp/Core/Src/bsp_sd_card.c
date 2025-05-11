#include "bsp_sd_card.h"
#include <string.h>

// SPI�����Ƭѡ���Ŷ���
//SPI_HandleTypeDef hspi1;  // ��CubeMX����
#define SD_CS_PORT       GPIOC
#define SD_CS_PIN        GPIO_PIN_4

// ˽�к�������
uint8_t SD_CS_Enable(void);
static void SD_CS_Disable(void);
static uint8_t SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc);
static uint8_t SD_GetResponse(uint8_t response, uint32_t timeout);
static uint8_t SD_ReadData(uint8_t *buf, uint16_t len);
static uint8_t SD_WriteData(const uint8_t *buf, uint16_t len);

// SD������
uint8_t SD_Type = 0;

//-----------------------------------------------------
// �ȴ�׼����
//-----------------------------------------------------
uint8_t SD_Wait_Ready(void)
{
  uint8_t r1;
  uint32_t retries = 0x1F;
  do {
    r1 = SPI_ReadWriteByte(0xFF);
    retries--;
  } while ((r1!=0xFF) && retries);

  return r1;//��ȷ����0xFF
}
//-----------------------------------------------------
// Ƭѡ����
//-----------------------------------------------------
static void SD_CS_Disable(void) {
  HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_SET);
	SPI_ReadWriteByte(0xFF);//�ӳ�8��ʱ��
}

uint8_t SD_CS_Enable(void) {
  HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_RESET);
  if(0xFF == SD_Wait_Ready()) return 0x00;//Ƭѡ�ɹ�
	SD_CS_Disable();
	return 1;//Ƭѡʧ��
	
}
//-----------------------------------------------------
// SPI��д���ֽ�
//-----------------------------------------------------
 uint8_t SPI_ReadWriteByte(uint8_t data) {
  uint8_t rx_data;
  HAL_SPI_TransmitReceive(&hspi1, &data, &rx_data, 1, 100);
  return rx_data;
}

//-----------------------------------------------------
// ����SD������
//-----------------------------------------------------
static uint8_t SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc) {
  uint8_t tx_buf[6];
  uint8_t r1;
  SD_CS_Disable();
	if(SD_CS_Enable()) return 0xFF;//Ƭѡʧ��
  // ���������
  tx_buf[0] = cmd | 0x40;
  tx_buf[1] = (arg >> 24) & 0xFF;
  tx_buf[2] = (arg >> 16) & 0xFF;
  tx_buf[3] = (arg >> 8) & 0xFF;
  tx_buf[4] = arg & 0xFF;
  tx_buf[5] = crc;

  SD_CS_Enable();
  HAL_SPI_Transmit(&hspi1, tx_buf, 6, 100);
	if(cmd==CMD12)SPI_ReadWriteByte(0xff);
  // �ȴ���Ӧ
  uint32_t retries = 0x1F;
  do {
    r1 = SPI_ReadWriteByte(0xFF);
    retries--;
  } while ((r1 & 0x80) && retries);

  return r1;
}

//-----------------------------------------------------
// SD����ʼ��
//-----------------------------------------------------
uint8_t SD_Init(void) {
  uint8_t r1;
  uint32_t retries;
  uint8_t ocr[4];

  // ����SPI����ģʽ��CubeMXĬ�����ã�
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  HAL_SPI_Init(&hspi1);

  // ��������74��ʱ������
  SD_CS_Disable();
  for (int i=0; i<10; i++) SPI_ReadWriteByte(0xFF);

  // CMD0: ��λ������״̬
  retries = 20;
  do {
    r1 = SD_SendCmd(CMD0, 0, 0x95);
  } while (r1 != 0x01 && retries--);
  if (r1 != 0x01) return 1;

  // CMD8: ���SD���汾
  if (SD_SendCmd(CMD8, 0x1AA, 0x87) == 0x01) {
    // SD V2.0+
    for (int i=0; i<4; i++) ocr[i] = SPI_ReadWriteByte(0xFF);
    if (ocr[2] != 0x01 || ocr[3] != 0xAA) return 2;

    // ACMD41��ʼ��
    retries = 0xFFF;
    do {
      SD_SendCmd(CMD55, 0, 0x01);
      r1 = SD_SendCmd(CMD41, 0x40000000, 0x01);
    } while (r1 != 0x00 && retries--);
    if (retries == 0) return 3;

    // ����Ƿ�֧�ָ�����
    SD_SendCmd(CMD58, 0, 0x01);
    for (int i=0; i<4; i++) ocr[i] = SPI_ReadWriteByte(0xFF);
    SD_Type = (ocr[0] & 0x40) ? SD_TYPE_V2HC : SD_TYPE_V2;
  } else {
    // SD V1.x��MMC��
    SD_SendCmd(CMD55, 0, 0x01);
    r1 = SD_SendCmd(CMD41, 0, 0x01);
    SD_Type = (r1 <= 1) ? SD_TYPE_V1 : SD_TYPE_MMC;
  }

  // �л�SPI������ģʽ
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  HAL_SPI_Init(&hspi1);
  SD_CS_Disable();
  return 0;
}

//-----------------------------------------------------
// ��ȡ�����飨512�ֽڣ�
//-----------------------------------------------------
uint8_t SD_ReadBlock(uint32_t block_addr, uint8_t *buf) {
  if (SD_Type != SD_TYPE_V2HC) block_addr <<= 9; // �ֽڵ�ַת��

  uint8_t r1 = SD_SendCmd(CMD17, block_addr, 0x01);
  if (r1 != 0x00) return 1;

  // �ȴ���������
  uint32_t timeout = 100000;
  while (SPI_ReadWriteByte(0xFF) != 0xFE && timeout--);
  if (!timeout) return 2;

  // ��ȡ����
  HAL_SPI_Receive(&hspi1, buf, 512, 100);
  SPI_ReadWriteByte(0xFF); // ����CRC
  SPI_ReadWriteByte(0xFF);
  SD_CS_Disable();
  return 0;
}

//-----------------------------------------------------
// д�뵥���飨512�ֽڣ�
//-----------------------------------------------------
uint8_t SD_WriteBlock(uint32_t block_addr, const uint8_t *buf) {
  if (SD_Type != SD_TYPE_V2HC) block_addr <<= 9;

  uint8_t r1 = SD_SendCmd(CMD24, block_addr, 0x01);
  if (r1 != 0x00) return 1;

  // ��������ǰ׺
  SPI_ReadWriteByte(0xFE);
  HAL_SPI_Transmit(&hspi1, (uint8_t*)buf, 512, 100);

  // ����αCRC�ͼ����Ӧ
  SPI_ReadWriteByte(0xFF);
  SPI_ReadWriteByte(0xFF);
  uint8_t resp = SPI_ReadWriteByte(0xFF);
  if ((resp & 0x1F) != 0x05) return 2;

  SD_CS_Disable();
  return 0;
}
// sd_card.c ��������´���

// ��CSD�Ĵ�����ȡ��������������
uint32_t SD_GetCapacity(void) {
    uint8_t csd[16];
    uint32_t capacity = 0;
    uint8_t ret;

    SD_CS_Enable();
    ret = SD_SendCmd(CMD9, 0, 0x01); // ����CMD9��ȡCSD
    if (ret != 0x00) {
        SD_CS_Disable();
        return 0; // ��ȡʧ��
    }

    // �ȴ���������0xFE
    uint32_t timeout = 100000;
    while (SPI_ReadWriteByte(0xFF) != 0xFE && timeout--);
    if (!timeout) {
        SD_CS_Disable();
        return 0; // ��ʱ
    }

    // ��ȡ16�ֽ�CSD����
    HAL_SPI_Receive(&hspi1, csd, 16, 100);
    SPI_ReadWriteByte(0xFF); // ����CRC
    SPI_ReadWriteByte(0xFF);
    SD_CS_Disable();

    // ����CSD����
    if ((csd[0] & 0xC0) == 0x40) { // SDHC/SDXC����CSD�汾2.0��
        uint32_t csize = ((uint32_t)(csd[7] & 0x3F) << 16) 
                       | ((uint32_t)csd[8] << 8)
                       | csd[9];
        capacity = (csize + 1) * 1024; // ÿ���С�̶�Ϊ512�ֽڣ������� = (csize+1)*512*1024/512
    } else { // ��׼��������CSD�汾1.0��
        uint8_t read_bl_len = csd[5] & 0x0F;
        uint16_t c_size = ((csd[6] & 0x03) << 10)
                        | (csd[7] << 2)
                        | ((csd[8] >> 6) & 0x03);
        uint8_t c_size_mult = ((csd[9] & 0x03) << 1)
                            | ((csd[10] >> 7) & 0x01);
        capacity = (uint32_t)(c_size + 1) << (c_size_mult + read_bl_len - 7);
    }

    return capacity; // ������������
}

