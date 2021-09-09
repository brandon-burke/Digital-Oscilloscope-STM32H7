/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_ll_adc.h"
#include "stm32h7xx_ll_bdma.h"
#include "stm32h7xx_ll_bus.h"
#include "stm32h7xx_ll_comp.h"
#include "stm32h7xx_ll_cordic.h"
#include "stm32h7xx_ll_cortex.h"
#include "stm32h7xx_ll_crc.h"
#include "stm32h7xx_ll_crs.h"
#include "stm32h7xx_ll_dac.h"
#include "stm32h7xx_ll_dma.h"
#include "stm32h7xx_ll_dma2d.h"
#include "stm32h7xx_ll_dmamux.h"
#include "stm32h7xx_ll_exti.h"
#include "stm32h7xx_ll_fmac.h"
#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_hrtim.h"
#include "stm32h7xx_ll_hsem.h"
#include "stm32h7xx_ll_i2c.h"
#include "stm32h7xx_ll_iwdg.h"
#include "stm32h7xx_ll_lptim.h"
#include "stm32h7xx_ll_lpuart.h"
#include "stm32h7xx_ll_mdma.h"
#include "stm32h7xx_ll_opamp.h"
#include "stm32h7xx_ll_pwr.h"
#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_rng.h"
#include "stm32h7xx_ll_rtc.h"
#include "stm32h7xx_ll_spi.h"
#include "stm32h7xx_ll_swpmi.h"
#include "stm32h7xx_ll_system.h"
#include "stm32h7xx_ll_tim.h"
#include "stm32h7xx_ll_usart.h"
#include "stm32h7xx_ll_utils.h"
#include "stm32h7xx_ll_wwdg.h"

/*Data structure to hold a*/
typedef struct Coordinate {
	float x;
	float y;
}Coordinate;

/*Simple function to utilize the PLL and overclock the M7*/
void overclock_128Mhz();


/*Time Delay Functions*/
void Delay_Init(uint16_t clk_freq);
void delay_ms(uint16_t delay);
void delay_us(uint16_t delay);

/*ADC3 Functions*/
void ADC3_PC2_enable();
void ADC3_PERPH_CLK_ENABLE();
void ADC3_calibrate();
void ADC3_enable();
void ADC3_configure();
void ADC3_TRGO_config();
void ADC3_start();
void ADC3_stop();

/*DMA1 Functions*/
void DMA_ADC3_init(uint32_t *data);
void DMA1_Stream0_IRQHandler();

/*TIM1 Functions*/
void TIM1_PE11_connect();
void TIM1_init();
void TIM1_PWM_config(uint16_t period, uint16_t interval);
void TIM1_UP_IRQHandler();

/*TIM8 Functions*/
void TIM8_init_TRGO();

/*SPI4 functions*/
#define ILI9341_DC_PIN			LL_GPIO_PIN_3
#define ILI9341_SCK_PIN			LL_GPIO_PIN_2
#define ILI9341_RST_PIN			LL_GPIO_PIN_5
#define ILI9341_SS_PIN			LL_GPIO_PIN_4
#define ILI9341_MOSI_PIN		LL_GPIO_PIN_6
void ILI9341_SPI4_init();
void ILI9341_SPI_Start();
void ILI9341_SPI_Stop();
void ILI9341_wait();
void ILI9341_set8();
void ILI9341_set16();
void ILI9341_GPIO_init();

/*Transmission Functions*/
void Transmit_Cmd(uint8_t);
void Transmit_Data8(uint8_t data);
void Transmit_Data16(uint16_t data);
void Transmit_Data16_array(uint32_t num_of_writes, uint16_t *data);
void Transmit_Data16_Cont(uint16_t data, uint32_t num_of_writes);


/*LCD initialization Functions*/
#define LCD_WIDTH 320
#define LCD_HEIGHT 240
void LCD_init();
void LCD_Exit_Standby();
void LCD_Reset();
void LCD_Configure();
void LCD_Set_Address_Window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/*LCD Writing Functions*/
void LCD_fill_screen(uint16_t color);
void LCD_set_pixel(uint16_t color, uint16_t x, uint16_t y);
void draw_horizontal_line(uint16_t color, uint16_t y_pos);
void draw_vertical_line(uint16_t color, uint16_t x_pos);


/*Graphing/Data functions*/
void Convert_Raw_Data(Coordinate *converted_data, uint16_t *raw_data);
void plot_pts(Coordinate pts[], float x_scale, float y_scale);

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */






/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
