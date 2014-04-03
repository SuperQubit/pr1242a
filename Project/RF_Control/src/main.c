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
#include "htp_msgbox.h"

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
	HAL_Init();
	BSP_Init();

	logger_assert("INIT\r");
	HTP_MBOX_Init();
	LOGIC_Init();
	DIAG_Init();
	CONTROL_Init();
	SUPERVISOR_Init();	
	PARSER_Init();
	logger_assert("EXEC\r");	
	
	// Watchdog Enable
  IWDG_Enable();
	
	while (1)
  {
		ClkGen_Ack();		
		
		if(ClkGen_4ms) 
		{ 
			HTP_MBOX_Clk(); 			
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
			IWDG_ReloadCounter();  
		}
			
		if(ClkGen_256ms) 
		{ 
			DDS_Clk();
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
