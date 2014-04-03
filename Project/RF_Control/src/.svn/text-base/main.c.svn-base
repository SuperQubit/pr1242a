/******************** (C) COPYRIGHT  2012 ArSilicii ********************
* File Name          : main.c
* Author             : DL
* Version            : V1.00 
* Date               : 21-10-2012
* Description        : Main module
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "hal.h"
#include "bsp.h"

#include "control.h"
#include "logic.h"
#include "diag.h"
#include "supervisor.h"
#include "parser.h"

#include <stdio.h>
#include <stdarg.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main.\
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

int main(void)
{
	static char WATCH=0;
	HAL_Init();
	BSP_Init();

	logger_assert("INIT\r");
	LOGIC_Init();
	DIAG_Init();
	CONTROL_Init();
	SUPERVISOR_Init();	
	PARSER_Init();
	logger_assert("EXEC\r");	
	
	
	RCC_LSICmd(ENABLE);
    /* Wait till LSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET){}
	/* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency dispersion) */
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  /* IWDG counter clock: LSI/32 */
  IWDG_SetPrescaler(IWDG_Prescaler_32);
  /* Set counter reload value to obtain 250ms IWDG TimeOut.
     Counter Reload Value = 250ms/IWDG counter clock period
                          = 250ms / (LSI/32)
                          = 0.25s / (LsiFreq/32)
                          = LsiFreq/(32 * 4)
                          = LsiFreq/128
   */
  IWDG_SetReload(40000/128);
  /* Reload IWDG counter */
  IWDG_ReloadCounter();
  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable();
	
	
	while (1)
  {
		ClkGen_Ack();		
		
		if(ClkGen_2ms) 
		{ 
			HTP_CTRL_Clk(); 			
		}		

		if(ClkGen_4ms) 
		{ 
			FAST_ADC_Clk();	
			//INPUT_ClK();			
			CONTROL_Clk();
			SUPERVISOR_Clk();
			//OUTPUT_Clk();
			BIAS_Clk();
		}
		
		if(ClkGen_32ms) 
		{ 				
			LOGIC_Clk();			
		}
		
		if(ClkGen_64ms) 
		{ 
			SLOW_ADC_PhisCalc();	
			PARSER_Clk();
			if(WATCH==0)IWDG_ReloadCounter();  
		}
			
		if(ClkGen_256ms) 
		{ 
			DDS_Clk();
			//printf("T=%5d\n",	ClkGen_Counter);		
		}		
	}
}


#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif
#define USE_RETARGET_PUTCHAR
#ifdef USE_RETARGET_PUTCHAR
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

PUTCHAR_PROTOTYPE
{
	COM_tx((uint8_t) ch);
  return ch;
}

#endif

#ifdef  USE_LOGGER_ASSERT
/*******************************************************************************
* Function Name  : logger customer
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void logout_line(uint32_t line, uint8_t* func,uint8_t* file, const char * msg, ...)
{		va_list args;
	  va_start(args, msg);
		//printf("%-8s(%3d)@",file,line);
		//printf("%10.7f:",HAL_Get_CYCCNT()/72.0E6);
		printf("%-10.7f@",HAL_Get_CYCCNT()/72.0E6);
		printf("%-8s(%3d):",file,line);
	  printf(msg,args);
		va_end(args);
}

void logout_print(const char * msg, ...)
{		va_list args;
	  va_start(args, msg);	
		printf(msg,args);
		va_end(args);
}
#endif


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
