#include "hal.h"
#include "slow_adc.h"

const uint8_t SLOW_ADC_Index[] = 
{
	ADC_Channel_10,		
	ADC_Channel_11,
	ADC_Channel_13,
	ADC_Channel_14,
	ADC_Channel_15,
	ADC_Channel_17
};


uint16_t SLOW_ADC_Data[sizeof(SLOW_ADC_Index)]; 
uint16_t SLOW_ADC_Data_Avg[sizeof(SLOW_ADC_Index)]; 

uint8_t SLOW_ADC_IbarScale=1;

void SLOW_ADC_IBar_SetScale(char scale){
	switch(scale)
	{
			case 0:
				GPIO_ResetBits(GPIOB, GPIO_Pin_2);
				SLOW_ADC_IbarScale=0;
				break;
			
			case 1:
				GPIO_SetBits(GPIOB, GPIO_Pin_2);
				SLOW_ADC_IbarScale=1;			
				break;
			default:
				GPIO_SetBits(GPIOB, GPIO_Pin_2);
				SLOW_ADC_IbarScale=1;			
				break;
	}
}

void SLOW_ADC_Config(void) {
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	 /* configure  as output push-pull  */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
	GPIO_SetBits(GPIOB, GPIO_Pin_2);	
}

volatile uint8_t Adc_Updater=0;
void SLOW_ADC_Update(void)
{
	Adc_Updater=1;
}

void SLOW_ADC_Init(void) 
{
	SLOW_ADC_Config();
	SLOW_ADC_IBar_SetScale(1);
}

uint32_t aD1,aD2,aD3,aD4,aD5,aD6;
uint16_t D1,D2,D3,D4,D5,D6;
#define POW2_NUMTAP_FAST	 (4)
#define POW2_NUMTAP_SLOW	 (8)
#define POW2_NUMTAP_VT  	(12)

uint16_t SLOW_ADC_TimeCostant=0;

void SLOW_ADC_Process()
{
	uint16_t d1,d2,d3,d4,d5,d6;		 			
		
	
	d1=SLOW_ADC_Data[0]= ADC1->JDR1;  //VBAR
	d2=SLOW_ADC_Data[1]= ADC2->JDR1;	//IBAR
	d3=SLOW_ADC_Data[2]= ADC1->JDR2;	//VOUTmid
	d4=SLOW_ADC_Data[3]= ADC2->JDR2;  //VOUTrms
 	d5=SLOW_ADC_Data[4]= ADC2->JDR3;	//TEMP
	d6=SLOW_ADC_Data[5]= ADC1->JDR3;	//Vref
	
	switch(SLOW_ADC_TimeCostant)
	{
			case 0:				
				if(Adc_Updater)
				{
						Adc_Updater=0;
						D1=d1; 
						D2=d2;
						D3=d3;
						D4=d4;
						D5=d5;
						D6=d6;
						aD1 = D1<<POW2_NUMTAP_SLOW;	
						aD2 = D2<<POW2_NUMTAP_SLOW;	
						aD3 = D3<<POW2_NUMTAP_SLOW;	
						aD4 = D4<<POW2_NUMTAP_SLOW;	
						aD5 = D5<<POW2_NUMTAP_VT;	
						aD6 = D6<<POW2_NUMTAP_VT;	
				}else
				{
						aD1+=d1-D1;
						aD2+=d2-D2;
						aD3+=d3-D3;
						aD4+=d4-D4;
						aD5+=d5-D5;
						aD6+=d6-D6; 
						D1 = aD1>>POW2_NUMTAP_SLOW;	
						D2 = aD2>>POW2_NUMTAP_SLOW;	
						D3 = aD3>>POW2_NUMTAP_SLOW;	
						D4 = aD4>>POW2_NUMTAP_SLOW;	
						D5 = aD5>>POW2_NUMTAP_VT;	
						D6 = aD6>>POW2_NUMTAP_VT;	
				}
				break;
			case 1:
				if(Adc_Updater)
				{
						Adc_Updater=0;
						D1=d1; 
						D2=d2;
						D3=d3;
						D4=d4;
						D5=d5;
						D6=d6;
						aD1 = D1<<POW2_NUMTAP_FAST;	
						aD2 = D2<<POW2_NUMTAP_FAST;	
						aD3 = D3<<POW2_NUMTAP_FAST;	
						aD4 = D4<<POW2_NUMTAP_FAST;
						aD5 = D5<<POW2_NUMTAP_VT;	
						aD6 = D6<<POW2_NUMTAP_VT;				
				}else
				{		aD1+=d1-D1;
						aD2+=d2-D2;
						aD3+=d3-D3;
						aD4+=d4-D4;
						aD5+=d5-D5;
						aD6+=d6-D6; 
						D1 = aD1>>POW2_NUMTAP_FAST;	
						D2 = aD2>>POW2_NUMTAP_FAST;	
						D3 = aD3>>POW2_NUMTAP_FAST;	
						D4 = aD4>>POW2_NUMTAP_FAST;
						D5 = aD5>>POW2_NUMTAP_VT;	
						D6 = aD6>>POW2_NUMTAP_VT;				
				}
				break;		
	}
}

tSlowGauge SLOW_Gauge;

void SLOW_ADC_PhisCalc(void)
{
		SLOW_ADC_Data_Avg[0]=D1;
		SLOW_ADC_Data_Avg[1]=D2;
		SLOW_ADC_Data_Avg[2]=D3;
		SLOW_ADC_Data_Avg[3]=D4;
		SLOW_ADC_Data_Avg[4]=D5;
		SLOW_ADC_Data_Avg[5]=D6;
  
		SLOW_Gauge.VBAR= D1 * 	VBAR_CONV ;
		SLOW_Gauge.IBAR= D2 * (SLOW_ADC_IbarScale ? IBAR1_CONV   : IBAR2_CONV );
#ifdef _TIPO_E	
    SLOW_Gauge.VOUT= D3 * (3.3/4096.0);
		SLOW_Gauge.IOUT= D4 * (3.3/4096.0);	
#else
		SLOW_Gauge.VOUT= D3 *	VOUT_CONV	+	VOUT_CONV_OFF;
		SLOW_Gauge.IOUT= D4 *  IOUT_CONV  ;
#endif
	
		SLOW_Gauge.TEMP= D5 *  TEMP_CONV  ;
		SLOW_Gauge.VREF= D6 *  VREF_CONV  ;
		SLOW_Gauge.PWRDC= (SLOW_Gauge.VBAR*SLOW_Gauge.IBAR);
	  if(SLOW_Gauge.PWRDC<0)SLOW_Gauge.PWRDC=0;
}


void SLOW_ADC_Clk(void)
{		
}

