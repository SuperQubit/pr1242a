/**
  ******************************************************************************
  * @file    supervisor.c
  * @author  DL
  * @version V1.00
  * @date    02-05-2013
  * @brief   This file provides system protection functions.
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
#include <stdio.h>
#include <stdarg.h>
/*


typedef struct
{
	float VAout;		// Tensione in uscita in V, Vpp/fattore di forma;
	float Vorms;		// Tensione in uscita in V, true rms;
	float IAout;		// Corrente in uscita in A, Ipp/fattore di forma;
	float Iorms;		// Corrente in uscita in A, true rms;
	float PPout;	  // Potenza attiva in uscita in W
	float PAout;		// Potenza apparente in uscita in VA
	float PQout;		// Potenza reattiva e deformante in uscita in VAR
	float ZL;				// Impedenza del carico in Ohm
	float XL;				// Reattanza del carico in Ohm
	float RL;				// Resistenza del carico in Ohm
} tFGauge;

typedef struct
{
	float VBAR;
	float IBAR;
	float VOUT;		
	float IOUT;
	float TEMP;
	float VREF;
} tSlowGauge;

typedef struct 
{
	uint32_t VBIASP_BORDER;
	uint16_t VBIASP_GATE_1;
	uint16_t VBIASP_GATE_2;
	float TINI;
	float IBAR_OFFSET;
	float IBAR_BORDER;
	float IBAR_GATE_1;
	float IBAR_GATE_2;
} tDiagReport;



*/

/** @defgroup SUPERVISOR_Private_Functions
  * @{
  */
char FANSTATE=0;
//uint16_t 
void SV_RapidShutdown()
{
		SUPPLY_OFF();	
		BIAS_DAC_Value;
		TIM4_CH1_DUTY;
		TIM4_CH2_DUTY;
		TIM4_CH3_DUTY;
		TIM4_CH4_DUTY;	
}


void SV_RapidRecovery()
{

}


/**
  * @}
  */

/** @defgroup SUPERVISOR_Public_Functions
  * @{
  */
float TJp;
float TJ;
float TCSoglia;
float PDM2;

#define RTHALF_JC_MAX	(0.45)
#define IDM2			(SLOW_Gauge.IBAR)
#define VDSM			(SLOW_Gauge.VBAR)

void SV_Observer(void)
{
		PDM2 =(SLOW_Gauge.VBAR*SLOW_Gauge.IBAR-FGauge.PPout); 
		TJ=	(PDM2)*RTHALF_JC_MAX+SLOW_Gauge.TEMP;
		TJp = (TJp*3.0+TJ)/4.0;			
}



char  SV_CheckPredictive_SOA(void)
{
		

}

void SUPERVISOR_Init(void) 
{
		FANSTATE=0;
}

#define SET_TIMEOUT(x)	(TOCounter=x)
#define WAIT_TIMEOUT()	 if(TOCounter)break
#define CHECK_TIMEOUT()	 (TOCounter==0)
#define UPDATE_TIMEOUT() if(TOCounter)TOCounter--
#define DEFINE_TIMEOUT()	static uint16_t TOCounter=0



void SUPERVISOR_Clk(void) 
{
			DEFINE_TIMEOUT();
			UPDATE_TIMEOUT();
	
			SV_Observer();
	
			if(TJp>140.0 && FANSTATE<2)
			{
					TCSoglia=SLOW_Gauge.TEMP;
					SUPPLY_OFF();	
					FANSTATE=2;	
					
			}
	
			switch(FANSTATE)
			{
					case 1:
							if(SLOW_Gauge.TEMP < 40.0) 
							{
								SUPPLY_FANOFF();
								FANSTATE=0;
							}
							if(SLOW_Gauge.TEMP > 75.0) 
							{
								SUPPLY_OFF();
							}		
							break;

					case 0:
							if(SLOW_Gauge.TEMP> 45.0) 
							{
								SUPPLY_FANON();
								FANSTATE=1;
							}		
							
							if(SLOW_Gauge.TEMP< 36.0) 
							{
								SUPPLY_ON();
							}	
														
							break;
					
					case 2:
							SUPPLY_FANON();
							logger_assert("PROTECTION: ");				
							logger_print("TJ Overtemp (%5.2f), TC=%5.2f\r",TJp,SLOW_Gauge.TEMP);	
							FANSTATE=3;
							SET_TIMEOUT(60);
							break;
							
					case 3:
							if(SLOW_Gauge.TEMP < (TCSoglia-5.0) || SLOW_Gauge.TEMP < (40.0)) 
							{								
								SUPPLY_ON();
								FANSTATE=1;
							}
							WAIT_TIMEOUT();
							SUPPLY_ON();
							FANSTATE=1;
							break;
			}		
		
			
			
			
			
}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2013 ArSilicii *****END OF FILE****/
