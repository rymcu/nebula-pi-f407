#include "bsp_ft6336_lcd.h"

/**
 * @brief FT6336��ʼ��
 *
 * @note cs������������1ms�����ߣ����ȴ��㹻��ʱ����к�������
 *
 * @param None
 *
 * @retval None
 */
void FT6336_Init(void)
{
    HAL_GPIO_WritePin(CTP_CS_GPIO_Port, CTP_CS_Pin, GPIO_PIN_RESET); // �͵�ƽ��λ���ٳ���1ms
    HAL_Delay(500);//rt_thread_delay(50);
    HAL_GPIO_WritePin(CTP_CS_GPIO_Port, CTP_CS_Pin, GPIO_PIN_SET); // �ĵ�Ҫ��SDA��SCL����
		HAL_Delay(500);//rt_thread_delay(500);
		uint8_t buf = MODE_POLLING;
		FT6336_Write_N_Data(FT6336_REG_G_MODE,&buf,1);//����Ϊ��ѯģʽ��������ʱ�ж�����һֱ����͵�ƽ
}

/**
 * @brief ����FT6336���ݵ�ַ
 *
 * @note 1.���������ź�
 * @note 2.�����豸��ַ��дģʽ
 * @note 3.�ȴ�ack
 * @note 4.����1 byte���ݵ�ַ
 * @note 5.�ȴ�ack
 * @note 6.����ֹͣ�ź�
 *
 * @param None
 *
 * @retval None
 */
void FT6336_WriteAddr(uint8_t addr)
{
    CTP_IIC_Start();
    CTP_IIC_Send_Byte(0x70); // �豸��ַ��дģʽ
    CTP_IIC_Wait_Ack();
    CTP_IIC_Send_Byte(addr); // ���ݵ�ַ
    CTP_IIC_Wait_Ack();
}
/**
 * @brief ��ȡFT6336 N byte����
 *
 * @note 1.���������ź�
 * @note 2.�����豸��ַ����ģʽ
 * @note 3.�ȴ�ack
 * @note 4.��ȡN byte���ݲ�ÿ�ֽں���ack
 * @note 5.����ֹͣ�ź�
 *
 * @param buf ��ȡ���ݻ�����
 * @param N ��ȡ���ݳ���
 *
 * @retval None
 */
void FT6336_Read_N_Data(uint8_t *buf, uint8_t N)
{
    CTP_IIC_Start();
    CTP_IIC_Send_Byte(0x71); // �豸��ַ����ģʽ
    CTP_IIC_Wait_Ack();
    for (int8_t i = 0; i < N; i++)
        buf[i] = CTP_IIC_Read_Byte(1); // ��ȡ���ݣ�ÿ�ֽں���ACK
    CTP_IIC_Stop();
}
/**
 * @brief д��FT6336 N byte����
 *
 * @note 1.���������ź�
 * @note 2.�����豸��ַ��дģʽ
 * @note 3.�ȴ�ack
 * @note 4.д�Ĵ�����ַ�����ȴ�ACK
 * @note 5.����N�ֽ����ݣ�ÿ�ֽڵȴ�ACK
 * @note 6.����ֹͣ�ź�
 *
 * @param addr �Ĵ�����ַ
 * @param buf д������ַ
 * @param N д�����ݳ���
 *
 * @retval None
 */
void FT6336_Write_N_Data(uint8_t addr,uint8_t *buf,uint8_t N)
{
    CTP_IIC_Start();
    CTP_IIC_Send_Byte(0x70); //�豸��ַ��дģʽ
    CTP_IIC_Wait_Ack();
    CTP_IIC_Send_Byte(addr); 
    CTP_IIC_Wait_Ack();
		for (int8_t i = 0; i < N; i++) 
		{
			CTP_IIC_Send_Byte(buf[i]); 
			CTP_IIC_Wait_Ack();
		}
		CTP_IIC_Stop();
}
/**
 * @brief ��ȡFT6336 ID����
 *
 * @note 1.дID�Ĵ�����ַ����
 * @note 2.��ȡID����
 * @note 3.����ID����
 *
 * @param addr ID�Ĵ�����ַ
 * @param N ��ȡ���ݳ���
 *
 * @retval ID����
 */
uint8_t FT6336_ReadID(uint8_t addr)
{
    uint8_t buf[1];
    FT6336_WriteAddr(addr);
    FT6336_Read_N_Data(buf, 1);
    return buf[0];
}

/**
 * @brief ��ȡFT6336�Ĵ�������
 *
 * @param addr ID�Ĵ�����ַ
 * @param N ��ȡ���ݳ���
 * @param buf ��ȡ���ݻ�����
 *
 * @retval none
 */
void FT6336_ReadReg(uint8_t addr, uint8_t *buf, uint8_t N)
{
    FT6336_WriteAddr(addr);
    FT6336_Read_N_Data(buf, N);
}
/**
 * @brief ��ȡFT6336������������
 *
 * @param x X��������ֵ
 * @param y Y��������ֵ
 *
 * @retval none
 */
void FT6336_ReadXY(int32_t *x,int32_t *y,uint8_t scr_scan_mode)
{
    uint8_t buf[4];
    FT6336_ReadReg(FT6336_REG_P1_XH, buf, 4); // ��ȡXH��XL��YH��YL
		uint16_t temp;
		//if(0x80 == (buf[0] & 0xF0))
		//{
			temp = buf[0] & 0x0F;
			*x = buf[1] + (temp << 8);

			temp = buf[2] & 0x0F;
			*y = buf[3] + (temp << 8);	

		if(scr_scan_mode == 6)
		{
			*x = 239 - *x;
			*y = 319 - *y;
		}			
		//}

		//else
		//{
			//*x = -1;
			//*y = -1;
		//}
}
/**
 * @brief �������
 *
 * @param none

 * @retval 1 �д�����0 �޴���
 */
uint8_t CTP_TouchDetect(void)
{
	uint8_t buf;
	if(CTP_PENIRQ_Read() == CTP_PENIRQ)
	{
		delay_us(10);//20us����
		if(CTP_PENIRQ_Read() == CTP_PENIRQ)
		{
			FT6336_ReadReg(FT6336_REG_P1_XH, &buf, 1);
			if(0x80 == (buf & 0xF0)) return 1;
			else return 0;
		}
	return 0;	
	}
	else
		return 0;
}
