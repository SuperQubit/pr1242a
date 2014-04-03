/**
  ******************************************************************************
  * @file    hal.h
  * @author  STM 32 HAL 
  * @version V1.0.0
  * @date    07/09/2011
  * @brief   Header file for STM32 ArSilicii Hardware
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2011 ArSilicii srl </center></h2>
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_H
#define __HAL_H

#define _TIPO_E

/* Includes ------------------------------------------------------------------*/
#include "STM32f10x.h"
#include "core_cm3.h"
#include "..\hal\uart.h"
#include "..\hal\spi1.h"
#include "..\hal\spi2.h"
#include "..\hal\i2c1.h"
#include "..\hal\clkgen.h"
#include "..\hal\adc.h"
#include "..\hal\ds1086.h"
#include "..\hal\tim4_pwm.h"

#define DWT_CYCCNT     (*(volatile unsigned int *)0xE0001004)
#define DWT_CONTROL    (*(volatile unsigned int *)0xE0001000)
#define SCB_DEMCR  (*(volatile unsigned int *)0xE000EDFC)
#define HAL_Enable_CYCCNT()					(SCB_DEMCR |=0x01000000, DWT_CYCCNT = 0, DWT_CONTROL |= 1)
#define HAL_Get_CYCCNT()		(DWT_CYCCNT)
#define HAL_Clear_CYCCNT()	(DWT_CYCCNT=0)


#define SET(module,pin)	      (module##_PORT->ODR |= (module##_##pin)) 
#define CLR(module,pin)	      (module##_PORT->ODR &= ~(module##_##pin)) 
#define INV(module,pin)	      (module##_PORT->ODR ^= (module##_##pin)) 



/*
#define SetPort(module,pin)	      (module->ODR |= (pin)) 
#define ClrPort(module,pin)	      (module->ODR &= ~(pin)) 
#define InvPort(module,pin)	      (module->ODR ^= (pin)) 
#define TestPort(module,pin)	    ((module->ODR & (pin))==(pin)) 
#define TestPin(module,pin)	    	((module->IDR & (pin))==(pin))  
*/

#define SetPort(x)	  	GPIO_SetBits(x)
#define ClrPort(x) 		GPIO_ResetBits(x)
#define TestPort(x)	 	GPIO_ReadOutputDataBit(x)
#define TestPin(x)	   	GPIO_ReadInputDataBit(x)

/*Define LED Board Parameter*/

#define HAL_LED1_PRPH_CLK       RCC_APB2Periph_GPIOC  
#define HAL_LED1_PORT      			GPIOC
#define HAL_LED1_PIN      		 	GPIO_Pin_10  

#define HAL_LED2_PRPH_CLK       RCC_APB2Periph_GPIOC  
#define HAL_LED2_PORT      			GPIOC
#define HAL_LED2_PIN       			GPIO_Pin_11  

#define HAL_LED1_ON()    					(HAL_LED1_PORT->ODR |= HAL_LED1_PIN) 
#define HAL_LED1_OFF()   					(HAL_LED1_PORT->ODR &= ~HAL_LED1_PIN) 
#define HAL_LED1_TOG()    				(HAL_LED1_PORT->ODR ^= HAL_LED1_PIN) 


#define HAL_LED2_ON()    					(HAL_LED2_PORT->ODR |= HAL_LED2_PIN) 
#define HAL_LED2_OFF()   					(HAL_LED2_PORT->ODR &= ~HAL_LED2_PIN) 
#define HAL_LED2_TOG()    				(HAL_LED2_PORT->ODR ^= HAL_LED2_PIN) 


#define HAL_LED_R_ON()		HAL_LED1_ON()     			
#define HAL_LED_R_OFF()  	HAL_LED1_OFF()  				
#define HAL_LED_R_TOG()  	HAL_LED1_TOG()   		

#define HAL_LED_G_ON()		HAL_LED2_ON()     			
#define HAL_LED_G_OFF()  	HAL_LED2_OFF()  				
#define HAL_LED_G_TOG()  	HAL_LED2_TOG()   			





#define HAL_VSUP_ON()     (HAL_VSUP_PORT->ODR |= HAL_VSUP_PIN) 
#define HAL_VSUP_OFF()   (HAL_VSUP_PORT->ODR &= ~HAL_VSUP_PIN) 
#define HAL_VSUP_TOG()    (HAL_VSUP_PORT->ODR ^= HAL_VSUP_PIN) 



#define HAL_MISO_TOG()    (GPIOB->ODR ^= GPIO_Pin_14) 


#define  HAL_Micro_Delay()		 for(volatile int dly = 40 ;dly;--dly)
//extern void HAL_Delay(uint32_t nTime);
//extern void HAL_TimingDelay_Decrement(void);



extern void HAL_Init(void);
extern void HAL_Clk(void);


#define HAL_COM                        USART1
#define HAL_COM_CLK                    RCC_APB1Periph_USART1
#define HAL_COM_TX_PIN                 GPIO_Pin_10
#define HAL_COM_TX_GPIO_PORT           GPIOB
#define HAL_COM_TX_GPIO_CLK            RCC_APB2Periph_GPIOB
#define HAL_COM_RX_PIN                 GPIO_Pin_11
#define HAL_COM_RX_GPIO_PORT           GPIOB
#define HAL_COM_RX_GPIO_CLK            RCC_APB2Periph_GPIOB
#define HAL_COM_IRQn                   USART1_IRQn

#endif 


  
/******************* (C) COPYRIGHT 2011 ArSilicii *****END OF FILE****/
