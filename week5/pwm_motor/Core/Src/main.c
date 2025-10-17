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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct get_speed_value {
  int last_encoder_value;
  int reg_dir;
} get_speed_value;
typedef struct motorState {
  int encoder_value;
  double encoder_angle;
  double wheel_angle;
  double wheel_speed;
} motorState;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define GEAR_ENCODER 30
#define GEAR_WHEEL 68
#define GEAR_MOTOR 16
#define SPEED_TIME 100 // ms
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
int pwm = 0;
/* USER CODE END PM */
static rspeed;
/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

get_speed_value motorValue1;
static motorState motor1;
static int i = 20,j=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
double getSpeed(void);
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
  motorValue1.last_encoder_value = 0;
  motorValue1.reg_dir = 0;
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
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);

  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim4);
  __HAL_TIM_SET_AUTORELOAD(&htim3, SPEED_TIME * 10);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  
  while (1) {
    //__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 40);
    // for (; i < 50; i++) {
    //   __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, i);
    //   HAL_Delay(50);
    // }

    // for (; i >= 20; i--) {
    //   __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, i);
    //   HAL_Delay(50);
    // }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim == &htim3){
    motor1.wheel_speed = getSpeed();
  }else if(htim==&htim4){
    if(i>20&&i<50){
      if(j==0){
        i++;
      }else{
        i--;
      }
    }else{
      if(i<=20){
        i=21;
        j=0;
      }else{
        i=49;
        j=1;
      }
    }
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, i);
    HAL_GPIO_TogglePin(RELAY_GPIO_Port, RELAY_Pin);
  }
  // 
}
double getSpeed(void) {
  // double speed=0;
  int encoder_now = __HAL_TIM_GET_COUNTER(&htim1);

  int encoder_last = motorValue1.last_encoder_value;
  int length = abs(encoder_now - encoder_last);
  if (encoder_now >= encoder_last) {
    if (length >= 32768) {
      motorValue1.reg_dir = -1;
      length = encoder_last + 65536 - encoder_now;
    } else {
      motorValue1.reg_dir = 1;
    }
  } else {
    if (length < 32768) {
      motorValue1.reg_dir = -1;
    } else {
      motorValue1.reg_dir = 1;
      length = encoder_now + 65536 - encoder_last;
    }
  }
  motorValue1.last_encoder_value = encoder_now;

  char message[100];
  // motor1.encoder_angle = (double)(encoder_now % 1024) * 0.3515625;
  // motor1.wheel_angle =
  //     ((double)encoder_now / 1024.0) * 360 * GEAR_ENCODER / GEAR_WHEEL;
  // motor1.wheel_angle =
  //     motor1.wheel_angle - (int)(motor1.wheel_angle / 360) * 360;

  double speed =
      (double)length * 1000 / 1024.0 * GEAR_ENCODER / GEAR_WHEEL / SPEED_TIME;
  sprintf(message, "%f,%d,%d,%d,%d\n ", speed, encoder_now, encoder_last, length,i);
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)message, strlen(message));
  // char message[100];
  // sprintf(message,"%d\n ",encoder_now);
  // HAL_UART_Transmit_IT(&huart2, (uint8_t *)message, strlen(message));
  rspeed=speed;
  return speed;
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
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
     line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
