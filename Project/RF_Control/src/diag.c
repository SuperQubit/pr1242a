/**
  ******************************************************************************
  * @file    diag.c
  * @author  DL
  * @version V1.00
  * @date    02-05-2013
  * @brief   This file provides all the Diagnostic firmware functions.
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
#include "diag.h"
#include <math.h>
#include <stdio.h>
#include <stdarg.h>


/** @defgroup DIAG_Private_Functions
  * @{
  */
#define SET_TIMEOUT(x)	(TOCounter=x)
#define WAIT_TIMEOUT()	 if(TOCounter)break
#define CHECK_TIMEOUT()	 (TOCounter==0)
#define UPDATE_TIMEOUT() if(TOCounter)TOCounter--
#define DEFINE_TIMEOUT()	static uint16_t TOCounter=0

tDiagStatus lStatus=DIAG_OK;
volatile enum {
			INIT,
			TEST_BARRA,
			TEST_BIAS,TEST_BIAS_CHECK_BORDER,TEST_BIAS_VALIDATE_BORDER,
			TEST_BIAS_GATE1_1A,TEST_BIAS_VALIDATE_GATE1_1A,TEST_BIAS_GATE2_1A,TEST_BIAS_VALIDATE_GATE2_1A,
			TEST_BIAS_GATE1,TEST_BIAS_VALIDATE_GATE1,TEST_BIAS_GATE2,TEST_BIAS_VALIDATE_GATE2,
			TEST_END} DIAG_State=INIT;

			
char BIAS_SetValueIterate(char begin, char test)
{
	 static uint16_t TryValue=4096,Span=0;
	 if(begin)
	 {
		 TryValue=2048;
		 Span=1024;
	 }else
	 {		 
		 if(test) TryValue += Span; else TryValue -=Span;		 
		 Span>>=1;
	 }	 
	 BIAS_SetValue(TryValue);	 
	 return(Span==0);	 
}			

char BIAS_SetG1ValueIterate(char begin, char test)
{
	 static uint16_t TryValue=4096,Span=0;
	 if(begin)
	 {
		 TryValue=2048;
		 Span=1024;
	 }else
	 {		 
		 if(test) TryValue += Span; else TryValue -=Span;		 
		 Span>>=1;
	 }
	 BIAS_VBIAS_1_WriteValue(TryValue);
	 TIM4_PWM_Clk();	
	 
	 return(Span==0);	 
}			

char BIAS_SetG2ValueIterate(char begin, char test)
{
	 static uint16_t TryValue=4096,Span=0;
	 if(begin)
	 {
		 TryValue=2048;
		 Span=1024;
	 }else
	 {		 
		 if(test) TryValue += Span; else TryValue -=Span;		 
		 Span>>=1;
	 }
	 BIAS_VBIAS_2_WriteValue(TryValue);
	 TIM4_PWM_Clk();	
	 
	 return(Span==0);	 
}			

/**
  * @}
  */

/** @defgroup DIAG_Public_Functions
  * @{
  */
tDiagStatus DIAG_StatusCheck(void) 
{
	
	
	return( lStatus);
}

tDiagReport DiagReport;



/*
float IBAR_OFFSET=0;
float IBAR_BORDER=0;
uint32_t VBIAS_BORDER=4096;
float IBAR_GATE_1=0;
uint16_t VBIAS_GATE_1=4096;
float IBAR_GATE_2=0;
uint16_t VBIAS_GATE_2=4096;
*/
#define IBIAS_NOM		(0.2)
#define IBIAS_1A		(0.6)

#define IBAR_OFFSET	(DiagReport.IBARP_OFFSET)
#define IBAR_BORDER	(DiagReport.IBARP_BORDER)
#define IBAR_GATE_1	(DiagReport.IBARP_GATE_1)
#define IBAR_GATE_2	(DiagReport.IBARP_GATE_2)
#define IBAR1A_GATE_1	(DiagReport.IBARP1_GATE_1)
#define IBAR1A_GATE_2	(DiagReport.IBARP1_GATE_2)

#define TEMPINI			(DiagReport.TINI)
#define VBAR_NOMINAL	(DiagReport.VBAR_NOM)
#define VBIAS_BORDER	(DiagReport.VBIASP_BORDER)
#define VBIAS_GATE_1	(DiagReport.VBIASP_GATE_1)
#define VBIAS_GATE_2	(DiagReport.VBIASP_GATE_2)
#define VBIAS1A_GATE_1	(DiagReport.VBIASP1_GATE_1)
#define VBIAS1A_GATE_2	(DiagReport.VBIASP1_GATE_2)

#define VBAR_TIMEOUT	(10)
#define BORDER_TIMEOUT	(5)
#define VGATE_TIMEOUT (5)

#define VBias	(7.5)
#define Kbg		(0.00040293)
float VG(uint16_t ns,uint16_t nd)
{
	float res;
	res=9.38-(4*nd+ns)*Kbg;
  if(res>VBias)res=VBias;
	if(res>3.0) 
	{
		res=res*0.667+1.0;
	}
	return(res);
}

float DVGS(float ID,float BETA)
{
	float res;
	res=sqrtf(2.0*ID/BETA);
	return(res);
}


float BETA(float ID,float DVGS)
{
	float res;
	res=2.0*ID/DVGS/DVGS;
	return(res);
}

tDiagSysID DIAG_CheckSYSID(void)
{
	tDiagSysID SysID;
	if(SLOW_ADC_Data_Avg[3] > 3900) 
	{
			SysID= SYSID_TIPOE;
	}
	else
		SysID=SYSID_TIPOD;
	
	return(SysID);

}

void DIAG_Init(void) 
{
	lStatus=DIAG_OK;
	DIAG_State=INIT;
	VBIAS_GATE_2=VBIAS_GATE_1=VBIAS_BORDER=4096;
}

void DIAG_Clk(void) 
{
	DEFINE_TIMEOUT();
	UPDATE_TIMEOUT();
	
	switch(DIAG_State)
  {
			case INIT:
				logger_assert("TEST INIT:");
				HAL_LED_R_ON();
				lStatus=DIAG_BUSY;
				SLOW_ADC_TimeCostant=ADCTC_FAST;
				SLOW_ADC_Update();
				//---------------- VB
				SUPPLY_ON();
				DDS_AmplitudeSet(3800);
				BIAS_VAUX_SetValue(0);
				SUPPLY_SetValue(1600);
				BIAS_SetValue(4095);
				BIAS_VBIAS_1_WriteValue(4095);
				BIAS_VBIAS_2_WriteValue(4095);
				SLOW_ADC_IBar_SetScale(0);						
				TIM4_PWM_Clk();	
			
				SET_TIMEOUT(VBAR_TIMEOUT);
				DIAG_State=TEST_BARRA;
				break;
					
			case TEST_BARRA:
			  WAIT_TIMEOUT();
				TEMPINI=SLOW_Gauge.TEMP;
				logger_print(" T0=%5.2f\r",TEMPINI);
			  logger_assert("TEST VBAR:");
				if((SLOW_Gauge.VBAR> 22.5) && (SLOW_Gauge.VBAR< 25.5))
				{
				  DIAG_State=TEST_BIAS;	
					IBAR_OFFSET=SLOW_Gauge.IBAR;
					VBAR_NOMINAL=SLOW_Gauge.VBAR;
					SLOW_ADC_TimeCostant=ADCTC_FAST;
					logger_print(" OK VB=%5.2f",SLOW_Gauge.VBAR);
				}
				else
				{
					logger_print(" ERROR VB=%5.2f\r",SLOW_Gauge.VBAR);
					SUPPLY_OFF();
					lStatus=DIAG_ERROR;
					DIAG_State=TEST_END;						
				}				
				break;
			
			case TEST_BIAS:							
				if(IBAR_OFFSET <  20e-3)
				{
					logger_print(", IB=%6.4f\r",IBAR_OFFSET);
					logger_assert("TEST BIAS:");
				  DIAG_State=TEST_BIAS_CHECK_BORDER;
					BIAS_SetValueIterate(1,0);
					SET_TIMEOUT(BORDER_TIMEOUT);
				}
				else
				{
					logger_print(", ERROR IB=%6.4f\r",IBAR_OFFSET);
					lStatus=DIAG_ERROR;
					DIAG_State=TEST_END;						
				}			
				break;
				
		  case TEST_BIAS_CHECK_BORDER:							
			 WAIT_TIMEOUT();
			 SLOW_ADC_PhisCalc();
			 if(BIAS_SetValueIterate(0,SLOW_Gauge.IBAR> (2.0*IBAR_OFFSET+10e-3)))
			 {
					VBIAS_BORDER=BIAS_DAC_Value;	
				  
					if(VBIAS_BORDER > 500 && VBIAS_BORDER< 3500)
					{								 
								 DIAG_State=TEST_BIAS_VALIDATE_BORDER;						 						
					}
					else
					{
						logger_print(" ERROR VDAC=%4d\r", VBIAS_BORDER);
						BIAS_SetValue(4095);
						DIAG_State=TEST_END;	
						lStatus=DIAG_ERROR;						
					}							 				
			 }
				else
			 {
				 SET_TIMEOUT(BORDER_TIMEOUT);
			 }
			 break;
				
			 case TEST_BIAS_VALIDATE_BORDER:	
				 SLOW_ADC_PhisCalc();
				  if(SLOW_Gauge.IBAR < (40e-3+2.0*IBAR_OFFSET))
					{						  
							//logger_print(" OK VDAC=%4.0f, IB=%6.4f\r",(float)VBIAS_BORDER,SLOW_Gauge.IBAR);						  			
							logger_print(" OK VDAC=%d, IB=%6.4f\r",VBIAS_BORDER,SLOW_Gauge.IBAR);								
							IBAR_BORDER=SLOW_Gauge.IBAR;
						  //------------- 							
							logger_assert("TEST BIAS_M1A:");
							DIAG_State=TEST_BIAS_GATE1_1A;
							BIAS_SetG1ValueIterate(1,0);
						  SET_TIMEOUT(VGATE_TIMEOUT);
					}
					else
					{
							logger_print(" ERROR IB=%6.4f\r",SLOW_Gauge.IBAR);							
							lStatus=DIAG_ERROR;
							DIAG_State=TEST_END;							
					}							 			

				 break;
					
		case TEST_BIAS_GATE1_1A:
				WAIT_TIMEOUT();
			  SLOW_ADC_PhisCalc();
				if(BIAS_SetG1ValueIterate(0,SLOW_Gauge.IBAR > (IBAR_BORDER+IBIAS_1A)))
			  {
					VBIAS1A_GATE_1=BIAS_V1_VALUE;					 
					if(VBIAS1A_GATE_1 > 100 && VBIAS1A_GATE_1< 3500)
					{				
								 DIAG_State=TEST_BIAS_VALIDATE_GATE1_1A;	
					}
					else
					{
						logger_print(" ERROR PWM_G1A=%4d\r", VBIAS1A_GATE_1);
						BIAS_SetValue(4095);
					  BIAS_VBIAS_1_WriteValue(4095);
						BIAS_VBIAS_2_WriteValue(4095);		
						DIAG_State=TEST_END;	
						lStatus=DIAG_ERROR;
					}							 				
			 }
				else
			 {
				 SET_TIMEOUT(VGATE_TIMEOUT);
			 }
				break;
			
			 case TEST_BIAS_VALIDATE_GATE1_1A:	
				 SLOW_ADC_PhisCalc();
					if(SLOW_Gauge.IBAR < (IBIAS_1A+0.05+ IBAR_BORDER))
					{						  
							
						IBAR1A_GATE_1=SLOW_Gauge.IBAR;	
						logger_print(" OK PWM_G1A=%d, ID_M1=%6.4f\r",VBIAS1A_GATE_1,IBAR1A_GATE_1);								
						
						BIAS_VBIAS_1_WriteValue(4095);
						logger_assert("TEST BIAS_M2A:");
						BIAS_SetG2ValueIterate(1,0);
						
						DIAG_State=TEST_BIAS_GATE2_1A;	
					  SET_TIMEOUT(VGATE_TIMEOUT);
					}
					else
					{
							logger_print(" ERROR ID_M1A=%6.4f\r",SLOW_Gauge.IBAR);
							BIAS_SetValue(4095);
							BIAS_VBIAS_1_WriteValue(4095);
							BIAS_VBIAS_2_WriteValue(4095);		
							
							lStatus=DIAG_ERROR;
							DIAG_State=TEST_END;							
					}							 			

				 break;

			case TEST_BIAS_GATE2_1A:
				WAIT_TIMEOUT();
			  SLOW_ADC_PhisCalc();
				if(BIAS_SetG2ValueIterate(0,SLOW_Gauge.IBAR > (IBAR_BORDER+IBIAS_1A)))
			  {
					VBIAS1A_GATE_2=BIAS_V2_VALUE;					 
					if(VBIAS1A_GATE_2 > 100 && VBIAS1A_GATE_2< 3500)
					{				
								 DIAG_State=TEST_BIAS_VALIDATE_GATE2_1A;	
					}
					else
					{
						logger_print(" ERROR PWM_G2A=%4d\r", VBIAS1A_GATE_2);
						BIAS_SetValue(4095);
					  BIAS_VBIAS_1_WriteValue(4095);
						BIAS_VBIAS_2_WriteValue(4095);		
						DIAG_State=TEST_END;	
						lStatus=DIAG_ERROR;
						HAL_LED_R_ON();
					}							 				
			 }
				else
			 {
				 SET_TIMEOUT(VGATE_TIMEOUT);
			 }
				break;
			
			 case TEST_BIAS_VALIDATE_GATE2_1A:	
				 SLOW_ADC_PhisCalc();
					if(SLOW_Gauge.IBAR < (IBIAS_1A+0.05 + IBAR_BORDER))
					{						  
						IBAR1A_GATE_2=SLOW_Gauge.IBAR;	
						logger_print(" OK PWM_G2A=%d, ID_M2=%6.4f\r",VBIAS1A_GATE_2,IBAR1A_GATE_2);								

						BIAS_VBIAS_2_WriteValue(4095);	
						logger_assert("TEST BIAS_M1:");
						DIAG_State=TEST_BIAS_GATE1;
						BIAS_SetG1ValueIterate(1,0);
						SET_TIMEOUT(VGATE_TIMEOUT);
					}
					else
					{
							logger_print(" ERROR ID_M2A=%6.4f\r",SLOW_Gauge.IBAR);
							BIAS_SetValue(4095);
							BIAS_VBIAS_1_WriteValue(4095);
							BIAS_VBIAS_2_WriteValue(4095);
							lStatus=DIAG_ERROR;
							DIAG_State=TEST_END;
							HAL_LED_R_ON();
					}							 			

				 break;
					
					
			case TEST_BIAS_GATE1:
				WAIT_TIMEOUT();
			  SLOW_ADC_PhisCalc();
				if(BIAS_SetG1ValueIterate(0,SLOW_Gauge.IBAR > (IBAR_BORDER+IBIAS_NOM)))
			  {
					VBIAS_GATE_1=BIAS_V1_VALUE;					 
					if(VBIAS_GATE_1 > 500 && VBIAS_GATE_1< 3500)
					{				
								 DIAG_State=TEST_BIAS_VALIDATE_GATE1;	
					}
					else
					{
						logger_print(" ERROR PWM_G1=%4d\r", VBIAS_GATE_1);
						BIAS_SetValue(4095);
					  BIAS_VBIAS_1_WriteValue(4095);
						BIAS_VBIAS_2_WriteValue(4095);		
						DIAG_State=TEST_END;	
						lStatus=DIAG_ERROR;
					}							 				
			 }
				else
			 {
				 SET_TIMEOUT(VGATE_TIMEOUT);
			 }
				break;
			
			 case TEST_BIAS_VALIDATE_GATE1:	
				 SLOW_ADC_PhisCalc();
					if(SLOW_Gauge.IBAR < (0.25 + IBAR_BORDER))
					{						  
							
						IBAR_GATE_1=SLOW_Gauge.IBAR;	
						logger_print(" OK PWM_G1=%d, ID_M1=%6.4f\r",VBIAS_GATE_1,IBAR_GATE_1);								
						
						BIAS_VBIAS_1_WriteValue(4095);
						logger_assert("TEST BIAS_M2:");
						BIAS_SetG2ValueIterate(1,0);
						
						DIAG_State=TEST_BIAS_GATE2;	
					  SET_TIMEOUT(VGATE_TIMEOUT);
					}
					else
					{
							logger_print(" ERROR ID_M1=%6.4f\r",SLOW_Gauge.IBAR);
							BIAS_SetValue(4095);
							BIAS_VBIAS_1_WriteValue(4095);
							BIAS_VBIAS_2_WriteValue(4095);		
							
							lStatus=DIAG_ERROR;
							DIAG_State=TEST_END;							
					}							 			

				 break;

			case TEST_BIAS_GATE2:
				WAIT_TIMEOUT();
			  SLOW_ADC_PhisCalc();
				if(BIAS_SetG2ValueIterate(0,SLOW_Gauge.IBAR > (IBAR_BORDER+0.2)))
			  {
					VBIAS_GATE_2=BIAS_V2_VALUE;					 
					if(VBIAS_GATE_2 > 500 && VBIAS_GATE_2< 3500)
					{				
								 DIAG_State=TEST_BIAS_VALIDATE_GATE2;	
					}
					else
					{
						logger_print(" ERROR PWM_G2=%4d\r", VBIAS_GATE_2);
						BIAS_SetValue(4095);
					  BIAS_VBIAS_1_WriteValue(4095);
						BIAS_VBIAS_2_WriteValue(4095);		
						DIAG_State=TEST_END;	
						lStatus=DIAG_ERROR;
						HAL_LED_R_ON();
					}							 				
			 }
				else
			 {
				 SET_TIMEOUT(VGATE_TIMEOUT);
			 }
				break;
			
			 case TEST_BIAS_VALIDATE_GATE2:	
				 SLOW_ADC_PhisCalc();
					if(SLOW_Gauge.IBAR < (0.25 + IBAR_BORDER))
					{						  
						IBAR_GATE_2=SLOW_Gauge.IBAR;	
						logger_print(" OK PWM_G2=%d, ID_M2=%6.4f\r",VBIAS_GATE_2,IBAR_GATE_2);								

						BIAS_SetValue(4095);
						BIAS_VBIAS_1_WriteValue(4095);
						BIAS_VBIAS_2_WriteValue(4095);		

						BIAS_SetValue(VBIAS_BORDER);
						BIAS_VBIAS_1_WriteValue(VBIAS_GATE_1);
						BIAS_VBIAS_2_WriteValue(VBIAS_GATE_2);
						
						SLOW_ADC_IBar_SetScale(1);
						SLOW_ADC_TimeCostant=ADCTC_SLOW;
						SLOW_ADC_Update();
						//Calculate Parameter
						DiagReport.VT=VG(4095,VBIAS_BORDER);	
						
						DiagReport.BETA_1=BETA(IBAR1A_GATE_1,VG(VBIAS1A_GATE_1,VBIAS_BORDER)-DiagReport.VT);
						DiagReport.VT_1=VG(VBIAS_GATE_1,VBIAS_BORDER)-DVGS(IBAR_GATE_1,DiagReport.BETA_1);
						DiagReport.BETA_1=BETA(IBAR1A_GATE_1,VG(VBIAS1A_GATE_1,VBIAS_BORDER)-DiagReport.VT_1);
						
						DiagReport.BETA_2=BETA(IBAR1A_GATE_2,VG(VBIAS1A_GATE_2,VBIAS_BORDER)-DiagReport.VT);						
						DiagReport.VT_2=VG(VBIAS_GATE_2,VBIAS_BORDER)-DVGS(IBAR_GATE_2,DiagReport.BETA_2);
						DiagReport.BETA_2=BETA(IBAR1A_GATE_2,VG(VBIAS1A_GATE_2,VBIAS_BORDER)-DiagReport.VT_2);
						
						DiagReport.SYSID=DIAG_CheckSYSID();
						DiagReport.NKAMP=5.0/(DiagReport.BETA_1+DiagReport.BETA_2);
						logger_assert("TEST OK:");
						if(DiagReport.SYSID==2)
						{
								logger_print(" TIPO E\r",0);
						}
						else
						{
								logger_print(" TIPO D\r",0);
						}
						//---------------- VB
						SUPPLY_ON();
						DDS_AmplitudeSet(3800);
						BIAS_VAUX_SetValue(0);
						SUPPLY_SetValue(800);
						DIAG_State=TEST_END;
						logger_assert("REPORT:");
						logger_print("VT=%4.2f ",DiagReport.VT);
						logger_print("NKMAP=%4.2f ",DiagReport.NKAMP);
						logger_print("M1=%4.2f,",DiagReport.BETA_1);
						logger_print("%4.2f ",DiagReport.VT_1);
						logger_print("M2=%4.2f,",DiagReport.BETA_2);
						logger_print("%4.2f\r",DiagReport.VT_2);
						

					}
					else
					{
							logger_print(" ERROR ID_M2=%6.4f\r",SLOW_Gauge.IBAR);
							BIAS_SetValue(4095);
							BIAS_VBIAS_1_WriteValue(4095);
							BIAS_VBIAS_2_WriteValue(4095);
							lStatus=DIAG_ERROR;
							DIAG_State=TEST_END;
							HAL_LED_R_ON();
					}							 			

				 break;
			 
					
			 case TEST_END:	
				 lStatus=DIAG_OK;			 
				 break;
			 
			 
	}
	
}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2013 ArSilicii *****END OF FILE****/
