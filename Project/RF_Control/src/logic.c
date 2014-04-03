/**
  ******************************************************************************
  * @file    logic.c
  * @author  DL
  * @version V1.00
  * @date    02-05-2013
  * @brief   This file provides all the main logic firmware functions.
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; COPYRIGHT 2013 ArSilicii</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "hal.h"
#include "bsp.h"
#include "supervisor.h"
#include "control.h"
#include "diag.h"
#include "logic.h"
#include <stdio.h>
#include <stdarg.h>


/** @defgroup LOGIC_Private_Functions
  * @{
  */
#define UILED_OFF()					
#define UILED_RED(x)					
#define UILED_GREEN(x)				

#define SET_TIMEOUT(x)	(TOCounter=x)
#define WAIT_TIMEOUT()	 if(TOCounter)break
#define CHECK_TIMEOUT()	 (TOCounter==0)
#define UPDATE_TIMEOUT() if(TOCounter)TOCounter--
#define DEFINE_TIMEOUT()	static uint16_t TOCounter=0

volatile enum {BOOT,DIAGNOSTIC,CALIBRATE,BIASING,RUNNING,SYS_ERROR,OFF} LGC_State=BOOT;

/**
  * @}
  */

/** @defgroup LOGIC_Public_Functions
  * @{
  */

void LOGIC_Init(void) 
{

}

void LOGIC_Clk(void) 
{
	static int rotate=0;
	
	DEFINE_TIMEOUT();	
	UPDATE_TIMEOUT();
	
	switch(LGC_State)
	{
			case BOOT:					
				  UILED_OFF();
					CONTROL_Setup(NONE,0);
					SET_TIMEOUT(5);
					DIAG_Init();
					LGC_State=DIAGNOSTIC;			
					logger_assert("SYSTEM: RFC1,FW:" SYS_VersionString SYS_ReleaseString "-P" HTP_VersionString ",BSP:" BSP_VersionString "-HW" HWB_VersionString "\r");  ;
					logger_assert("DIAGNOSTIC\r");
					break;
			
			case DIAGNOSTIC:
				  WAIT_TIMEOUT();
					DIAG_Clk();
					switch(DIAG_StatusCheck())
					{
						case DIAG_BUSY:
							HAL_LED_R_TOG();
							HAL_LED_G_TOG();
							break;
						case DIAG_OK:
							HAL_LED_G_ON();
							HAL_LED_R_OFF();
							logger_assert("DIAGNOSTIC: OK\r");
							logger_assert("RUNNIG\r");
							LGC_State=RUNNING;	
							CONTROL_Setup(HTPH1,NONE);
							break;
						case DIAG_ERROR:
							logger_assert("DIAGNOSTIC: ERROR, Retring..\r");
							HAL_LED_R_ON();
							HAL_LED_G_OFF();
							SUPPLY_OFF();							
							SET_TIMEOUT(150);
							DIAG_Init();
							LGC_State=DIAGNOSTIC;			
							break;
					}			
				  break;						
					
			case RUNNING:					
					if(ClkGen_1024ms) 
					{
						if((rotate % 10)==0)
						{
							logger_assert("OUT:");
							logger_print(" V=%4.1f,Ve=%4.1f,Ie=%5.2f,",FGauge.VAout, FGauge.Vorms,  FGauge.Iorms);
							logger_print("T=%3.0f,%3.0f\r",TJp,SLOW_Gauge.TEMP);
						}
						logger_assert("P=");
						logger_print("%4.1f,(%4.1f,j%4.1f)\r",FGauge.PAout, FGauge.PPout,  FGauge.PQout);
						rotate++;
					}	
					break;
	}
  
}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2013 ArSilicii *****END OF FILE****/
