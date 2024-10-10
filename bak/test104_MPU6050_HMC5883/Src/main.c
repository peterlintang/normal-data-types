/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#include "stdio.h"
#include  <math.h>    //Keil library  
#include "HMC5883.h"
#include "mpu6050.h"
extern unsigned char BUF[];
unsigned char tx_data[20];
unsigned char sumcheck = 0;
unsigned char addcheck = 0;
signed short roll = 0;
signed short pitch = 0;
signed short yaw = 0;
extern float  Q_ANGLE_X, Q_ANGLE_Y, Q_ANGLE_Z;   
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);
void AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	short mx,my,mz;
	double angle;
	unsigned char i,j;
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
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
	HAL_Delay(500);
	MPU6050_Init();
	MPU6050_calibrate();
	Init_HMC5883(); 
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		HAL_Delay(100);
		Multiple_Read_HMC5883();      //????,??BUF???
		mx = (short)(BUF[0] << 8) | BUF[1]; //x???????
		mz = (short)(BUF[2] << 8) | BUF[3]; //z???????
		my = (short)(BUF[4] << 8) | BUF[5]; //y???????
/*
		printf("x = %d\r\ny = %d\r\nz = %d\r\n",x,y,z);
		angle= atan2((double)y,(double)x) * (180 / 3.14159265) + 180; //????
		printf("A = %f\r\n",angle);
		printf("\r\n");
		*/
		
		MPU6050_Get();
				//????,????????
		/*
		IMUupdate((float)gx*500/57.3/65536, (float)gy*500/57.3/65536, (float)gz*500/57.3/65536,
							(float)ax*9.8/16384, (float)ay*9.8/16384, (float)az*9.8/16384);
		*/
		AHRSupdate((float)gx*500/57.3/65536, (float)gy*500/57.3/65536, (float)gz*500/57.3/65536,
							(float)ax, (float)ay, (float)az,
							(float)mx, (float)my, (float)mz);
		i = 0;	
		tx_data[i++] = 0xAA;
		tx_data[i++] = 0xFF;
		tx_data[i++] = 0x03;
		tx_data[i++] = 7;		
		
		roll = Q_ANGLE_X*100;		// ???100
		tx_data[i++] = roll&0xff;
		tx_data[i++] = roll>>8;
		
		pitch = Q_ANGLE_Y*100;
		tx_data[i++] = pitch&0xff;
		tx_data[i++] = pitch>>8;
		
		yaw = Q_ANGLE_Z*100;
		tx_data[i++] = yaw&0xff;
		tx_data[i++] = yaw>>8;
		tx_data[i++] = 0;
		sumcheck = 0; 
		addcheck = 0; 
		for(j=0; j < (tx_data[3]+4); j++) 
		{ 
			 sumcheck += tx_data[j];    //?????
			 addcheck += sumcheck;      //?????
		} 
		tx_data[i++] = sumcheck;
		tx_data[i++] = addcheck;
		HAL_UART_Transmit_IT(&huart1, tx_data, i);	
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
