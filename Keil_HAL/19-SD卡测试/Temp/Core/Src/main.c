/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"//printf函数�?
//添加头文件支�?
#include "string.h"
#include "bsp_spi_flash.h"//SPI flash驱动
#include "usbd_cdc_if.h"
#include "bsp_sd_card.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t rx_buff[100];  //接收缓存
uint8_t rx_done = 0; //接收完成标志
uint8_t rx_cnt = 0;//接收数据长度

//IIC全局变量
#define ADDR_WR_AT24CXX 0xA0 //写器件地�?
#define ADDR_RD_AT24CXX 0xA1 //读器件地�?
#define BuffSize 256
uint8_t Wr_buff[BuffSize],Rd_buff[BuffSize];//读写数据buff
 unsigned char USB_CDC_buff[] = {"STM32 HAL USB virtual port com test!\r\n"};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
SPI_HandleTypeDef *hspi_flash = &hspi1; // 必须定义
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void FatFs_Test(void);
void spi_flash_test(void);
void E2PROM_I2C_Test(void);

// 测试用缓冲区
uint8_t write_buffer[512];
uint8_t read_buffer[512];

// 填充测试数据（0x00~0xFF循环）
void fill_test_pattern(uint8_t* buf) {
    for (uint16_t i = 0; i < 512; i++) {
        buf[i] = i % 256;
    }
}

// 数据校验函数
uint8_t verify_data(uint8_t* buf1, uint8_t* buf2) {
    for (uint16_t i = 0; i < 512; i++) {
        if (buf1[i] != buf2[i]) {
          printf("Data is inconsistent @ offset %d: Written=0x%02X, Read=0x%02X\r\n", i, buf1[i], buf2[i]);
            return 0;
        }
    }
    return 1;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
 printf("\r\n===== SD Card Read/Write Test Start =====\r\n\r\n");

  // 1. Initialize SD Card
  uint8_t ret = SD_Init();
  if (ret != 0) {
      printf("[ERROR] SD Card Initialization Failed. Error Code: 0x%02X\r\n", ret);
      while(1); // Halt if initialization fails
  }
  printf("[INFO] SD Card Initialized Successfully!\r\n");

  // 2. Get and Display Capacity Information
  uint32_t capacity = SD_GetCapacity();
  printf("[INFO] Total Sectors: %lu (Approx %.2f GB)\r\n", 
         capacity, (float)capacity * 512 / 1000 / 1000 / 1000);

  // 3. Prepare Test Data
  fill_test_pattern(write_buffer);
  const uint32_t test_sector = 1000; // Test sector (avoid overwriting critical data)

  // 4. Write Test
  printf("[TEST] Writing to Sector %lu...\r\n", test_sector);
  ret = SD_WriteBlock(test_sector, write_buffer);
  if (ret != 0) {
      printf("[ERROR] Sector Write Failed. Error Code: 0x%02X\r\n", ret);
      while(1);
  }
  printf("[TEST] Write Successful. Waiting 1 second...\r\n");
  HAL_Delay(1000); // Ensure data is physically written

  // 5. Read Test
  printf("[TEST] Reading from Sector %lu...\r\n", test_sector);
  ret = SD_ReadBlock(test_sector, read_buffer);
  if (ret != 0) {
      printf("[ERROR] Sector Read Failed. Error Code: 0x%02X\r\n", ret);
      while(1);
  }

  // 6. Data Verification
  printf("[TEST] Verifying Data...\r\n");
  if (verify_data(write_buffer, read_buffer)) {
      printf("[RESULT] Test Passed! Read/Write Data Matched!\r\n");
  } else {
      printf("[ERROR] Test Failed! Data Mismatch Detected!\r\n");
  }
  printf("\r\n================== Test Completed ===========\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//CDC_Transmit_FS(USB_CDC_buff, sizeof(USB_CDC_buff));
    //HAL_Delay(2000);
    if(rx_done == 1)//判读是否接收完成
    {
        rx_done = 0;//清除接收标志
        //数据处理，打印接收长度�?�接收的数据
        printf("length of rx data: %d!\r\n",rx_cnt);
        for(int i = 0;i<rx_cnt;i++) printf("%c",rx_buff[i]);
        printf("\r\n");

        rx_cnt =0;//清除接收长度
    } 
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin); // 切换亮�?�灭状�?�，添加此语句防止优�?   
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch,FILE *f)
#endif /* __GNUC__ */
//重定向printf函数
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,HAL_MAX_DELAY);//输出指向串口USART1
    return ch;
}
//重写GPIO中断处理函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    //翻转LED1
    HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
    //打印中断处理信息
    printf("K2 ExTi interrput!\r\n");
}

//EEPROM测试
void E2PROM_I2C_Test(void)
{
    HAL_StatusTypeDef error;
    uint8_t length = 8 ;
    for(uint16_t i = 0;i<256;i++) Wr_buff[i] = i;
    //AT24Cxx地址00弿始写兿8Byte数据
    error = HAL_I2C_Mem_Write(&hi2c1,ADDR_WR_AT24CXX,0x00,I2C_MEMADD_SIZE_16BIT,Wr_buff,length,1000);
    if(error == HAL_OK) printf("e2prom write done!\r\n");
    else                printf("e2prom write fail!\r\n");
    HAL_Delay(1000);
    //从地�?0x00弿始�?8Byte数据，并打印�?
    error = HAL_I2C_Mem_Read(&hi2c1,ADDR_RD_AT24CXX,0x00,I2C_MEMADD_SIZE_16BIT,Rd_buff,length,1000);
    if(error == HAL_OK)
    {
        printf("e2prom read done!\r\n");
        for(uint16_t i=0;i<length;i++)
        {
            printf("0x%02x ",Rd_buff[i]);
        }
        printf("\r\n");
    }
    else  printf("e2prom write fail!\r\n");
}
void spi_flash_test(void)
{
  /* 测试Flash驱动 */
  uint16_t manu_id, dev_id;
  uint8_t jedec_id[3];
  uint8_t write_buf[256] = {0xAA, 0xBB, 0xCC, 0xDD};
  uint8_t read_buf[256];
  
  Flash_ReadID(&manu_id, &dev_id);
  printf("Flash Manufacturer ID: 0x%04X\r\n", manu_id);
  printf("Flash Device ID: 0x%04X\r\n", dev_id);
  Flash_ReadJedecID(jedec_id);
  printf("Flash JEDEC ID: 0x%02X, 0x%02X, 0x%02X\r\n", jedec_id[0], jedec_id[1], jedec_id[2]);
  
  if (Flash_SectorErase(0x0000) == HAL_OK) {
    Flash_PageProgram(0x0000, write_buf, 4);
    Flash_ReadData(0x0000, read_buf, 4);
  }
  printf("Flash Write Data: 0x%02X, 0x%02X, 0x%02X, 0x%02X\r\n", write_buf[0], write_buf[1], write_buf[2], write_buf[3]);
  printf("Flash Read  Data: 0x%02X, 0x%02X, 0x%02X, 0x%02X\r\n", read_buf[0], read_buf[1], read_buf[2], read_buf[3]);
  // 验证数据
  if (memcmp(write_buf, read_buf, 4) == 0) {
    printf("\r\nSPI Flash Test Passed!\r\n");
  }
} 

FATFS fs;       // 文件系统对象
FIL fil;        // 文件对象
FRESULT res;    // FatFs 函数返回�?
uint8_t work_buf[4096]; // 定义工作缓冲区（�?足够大，建议4KB以上�?

void FatFs_Test(void) {
    // 步骤1：首次尝试挂载文件系统（不自动格式化�?
    res = f_mount(&fs, "0:", 1);  // 第三个参�?0：不自动格式�?
    if (res == FR_OK) {
        printf("f_mount ok\n");
    } else {
        printf("f_mount failed: 0x%02X\n", res);

        // 步骤2：若因未初始化文件系统（FR_NO_FILESYSTEM），尝试格式�?
        if (res == FR_NO_FILESYSTEM) {
            printf("FR_NO_FILESYSTEM,begin f_mkfs...\n");
            
            // 格式化参数：自动选择FAT类型（FM_ANY），快�?�格式化（FM_SFD�?
            res = f_mkfs("0:", FM_ANY | FM_SFD, 0, work_buf, sizeof(work_buf));
            
            if (res == FR_OK) {
                printf("f_mkfs ok,f_mount again...\n");
                res = f_mount(&fs, "0:", 1);  // 重新挂载（自动格式化已完成）
                if (res == FR_OK) {
                    printf("f_mount ok\n");
                } else {
                    printf("f_mount failed: 0x%02X\n", res);
                    return;
                }
            } else {
                printf("f_mkfs failed: 0x%02X\n", res);
                return;
            }
        } else {
            // 其他错误（如设备未就绪�?�硬件错误）
            printf("can not f_mkfs\n");
            return;
        }
    }

    // 步骤3：文件读写测�?
    res = f_open(&fil, "test.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if (res != FR_OK) {
        printf("f_open test.txt failed: 0x%02X\n", res);
			return;
    }
		else
		{
			printf("f_open ok\n");
		}

    char *msg = "Hello, SPI Flash with FatFs  rymcu.com!";
    UINT bytes_written;
    res = f_write(&fil, msg, strlen(msg), &bytes_written);
    if (res != FR_OK) {
        printf("f_write failed: 0x%02X\n", res);
        f_close(&fil);
        return;
    }
    printf("f_write ok: %d\n", bytes_written);
    f_close(&fil);

    // 步骤4：读取验�?
    res = f_open(&fil, "test.txt", FA_READ);
    if (res != FR_OK) {
        printf("f_open failed: 0x%02X\n", res);
        return;
    }

    static char buf[200];
    UINT bytes_read;
    res = f_read(&fil, buf, sizeof(buf)-1, &bytes_read);
    if (res != FR_OK) {
        printf("f_read failed: 0x%02X\n", res);
        f_close(&fil);
        return;
    }
    buf[bytes_read] = '\0';
    printf("f_read: %s\n", buf);
    f_close(&fil);

    // 步骤5：卸载文件系�?
    f_mount(NULL, "0:", 0);
    printf("test done\n\n");
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
