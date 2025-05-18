#include "bsp_rgb.h"

// ���͵�������
#define HIGH 5
#define LOW  2
uint16_t sendBit(uint8_t bitVal)
{
    if (bitVal)
    {
        HAL_GPIO_WritePin(WS2812_GPIO_Port, WS2812_Pin, GPIO_PIN_SET);
        // ����WS2812ʱ��Ҫ�󣬸ߵ�ƽʱ��
        for (int i = 0; i < HIGH; i++)
        {
					__ASM volatile ("nop");//14ns
        }
        HAL_GPIO_WritePin(WS2812_GPIO_Port, WS2812_Pin, GPIO_PIN_RESET);
        // �͵�ƽʱ��
        for (int j = 0; j < LOW; j++)
        {
					__ASM volatile ("nop");
        }
    }
    else
    {
        HAL_GPIO_WritePin(WS2812_GPIO_Port, WS2812_Pin, GPIO_PIN_SET);
        for (int i = 0; i < LOW; i++)
        {
					__ASM volatile ("nop");
        }
        HAL_GPIO_WritePin(WS2812_GPIO_Port, WS2812_Pin, GPIO_PIN_RESET);
        for (int j = 0; j < HIGH; j++)
        {
					__ASM volatile ("nop");
        }
    }
}

// ����һ���ֽ�
void sendByte(uint8_t byte)
{
    for (int i = 0; i < 8; i++)
    {
        sendBit(byte & 0x80);
        byte <<= 1;
    }
}

void ry_set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    sendByte(g);
    sendByte(r);
    sendByte(b);
}