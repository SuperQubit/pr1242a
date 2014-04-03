#include "stm32F10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_dma.h"
#include "hal.h"
#include "fast_adc.h"
#include <math.h>

#define ADC1_DR_Address    ((uint32_t)0x4001244C)

uint16_t FAST_ADC_Data[DMADATA_SIZE]; 

uint16_t *pdata;
uint16_t *edata;

uint32_t m1,m2,m3,m4;
int32_t p1,P1,aP1;
int32_t v1,V1,aV1;
int32_t i1,I1,aI1;

uint16_t A1,A2,A3,A4;
uint16_t M1,M2,M3,M4;
uint16_t L1,L2,L3,L4;
uint16_t Vopp_i,Iopp_i;

uint32_t aA1,aA2,aA3,aA4;
uint32_t aM1,aM2,aM3,aM4;
uint32_t aL1,aL2,aL3,aL4;

void FAST_ADC_1HALF_Process(void){	
  uint16_t d1,d2,d3,d4;
  uint16_t X1,X2,X3,X4;
	uint16_t x1,x2,x3,x4;
	register int32_t dV,dI;
	
	//HAL_LED_R_ON();
	pdata=FAST_ADC_Data;
	edata=&FAST_ADC_Data[(DMADATA_SIZE/2)];
	X1=X2=X3=X4=0;
	x1=x2=x3=x4=0xFFFF;
	m1=m2=m3=m4=0;
	p1=v1=i1=0l;
	
	while(pdata<edata)
	{		
		m1+=(d1=*pdata++);		
		if(X1<d1)X1=d1;
		if(x1>d1)x1=d1;
		m2+=(d2=*pdata++);
		if(X2<d2)X2=d2;
		if(x2>d2)x2=d2;
		m3+=(d3=*pdata++);
		if(X3<d3)X3=d3;
		if(x3>d3)x3=d3;
		m4+=(d4=*pdata++);
		if(X4<d4)X4=d4;
		if(x4>d4)x4=d4;
		dV=(int32_t)(d1-A1);
		dI=(int32_t)(d2-A2);
		p1-= dV*dI;	
		v1+=dV*dV;
		i1+=dI*dI;
	}	
	
	//A1=m1>>4;A2=m2>>4;A3=m3>>4;A4=m4>>4;
	//x2 = (x1*(N-1)+x)/N; N*xx = xx*N - xx + x;
	
	aA4+= (m4>>NSAMPLE_POW2)-A4; A4 = aA4>>8;
	aA3+= (m3>>NSAMPLE_POW2)-A3; A3 = aA3>>8;
	aA2+= (m2>>NSAMPLE_POW2)-A2; A2 = aA2>>8;
	aA1+= (m1>>NSAMPLE_POW2)-A1; A1 = aA1>>8;
	aP1+= (p1/16)-P1; P1= aP1/256;
	aV1+= (v1/16)-V1; V1= aV1/256;
	aI1+= (i1/16)-I1; I1= aI1/256;
	
	aM4+= X4-M4; M4 = aM4>>8;
	aM3+= X3-M3; M3 = aM3>>8;
	aM2+= X2-M2; M2 = aM2>>8;
	aM1+= X1-M1; M1 = aM1>>8;

	aL4+= x4-L4; L4 = aL4>>8;
	aL3+= x3-L3; L3 = aL3>>8;
	aL2+= x2-L2; L2 = aL2>>8;
	aL1+= x1-L1; L1 = aL1>>8;	
	Vopp_i=X1-x1;
	Iopp_i=X2-x2;
	
	//HAL_LED_R_OFF();
}

void FAST_ADC_2HALF_Process(void){
	uint16_t d1,d2,d3,d4;
  uint16_t X1,X2,X3,X4;
	uint16_t x1,x2,x3,x4;
	register int32_t dV,dI;
	
	//HAL_LED_R_ON();	
	pdata=&FAST_ADC_Data[(DMADATA_SIZE/2)];;
	edata=&FAST_ADC_Data[(DMADATA_SIZE)];

	X1=X2=X3=X4=0;
	x1=x2=x3=x4=0xFFFF;
	m1=m2=m3=m4=0;
	p1=v1=i1=0l;
	
	while(pdata<edata)
	{		
		m1+=(d1=*pdata++);		
		if(X1<d1)X1=d1;
		if(x1>d1)x1=d1;
		m2+=(d2=*pdata++);
		if(X2<d2)X2=d2;
		if(x2>d2)x2=d2;
		m3+=(d3=*pdata++);
		if(X3<d3)X3=d3;
		if(x3>d3)x3=d3;
		m4+=(d4=*pdata++);
		if(X4<d4)X4=d4;
		if(x4>d4)x4=d4;
		dV=(int32_t)(d1-A1);
		dI=(int32_t)(d2-A2);
		p1-= dV*dI;	
		v1+=dV*dV;
		i1+=dI*dI;
	}	
	
	//A1=m1>>4;A2=m2>>4;A3=m3>>4;A4=m4>>4;
	//x2 = (x1*(N-1)+x)/N; N*xx = xx*N - xx + x;
	
	aA4+= (m4>>NSAMPLE_POW2)-A4; A4 = aA4>>8;
	aA3+= (m3>>NSAMPLE_POW2)-A3; A3 = aA3>>8;
	aA2+= (m2>>NSAMPLE_POW2)-A2; A2 = aA2>>8;
	aA1+= (m1>>NSAMPLE_POW2)-A1; A1 = aA1>>8;
	aP1+= (p1/16)-P1; P1= aP1/256;
	aV1+= (v1/16)-V1; V1= aV1/256;
	aI1+= (i1/16)-I1; I1= aI1/256;
	
	aM4+= X4-M4; M4 = aM4>>8;
	aM3+= X3-M3; M3 = aM3>>8;
	aM2+= X2-M2; M2 = aM2>>8;
	aM1+= X1-M1; M1 = aM1>>8;

	aL4+= x4-L4; L4 = aL4>>8;
	aL3+= x3-L3; L3 = aL3>>8;
	aL2+= x2-L2; L2 = aL2>>8;
	aL1+= x1-L1; L1 = aL1>>8;	
	Vopp_i=X1-x1;
	Iopp_i=X2-x2;	
	//HAL_LED_R_OFF();
}

void TIM8_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	
	TIM_DeInit(TIM8);
	
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 490;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	TIM_ETRClockMode1Config (TIM8, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
	TIM_TIxExternalClockConfig (TIM8, TIM_TIxExternalCLK1Source_TI1ED, TIM_ICPolarity_BothEdge, 0) ;
	   
	//TIM_ARRPreloadConfig(TIM8, ENABLE);

 // TIM8 PWM2 Mode configuration: Channel2
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 10;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC2Init(TIM8, &TIM_OCInitStructure);
  // Output compare preload enable
  TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Disable) ;
  //TIM_OC2FastConfig (TIM8, TIM_OCFast_Enable);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 250;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC3Init(TIM8, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Disable) ;
  // One Pulse Mode selection
  TIM_SelectOnePulseMode(TIM8, TIM_OPMode_Repetitive);
  // Output Trigger selection 
  TIM_SelectOutputTrigger(TIM8, TIM_TRGOSource_OC2Ref);
  // Slave Mode selection: Trigger Mode 
  TIM_SelectSlaveMode(TIM8, TIM_SlaveMode_External1);
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
  /* TIM8 enable counter */
  TIM_Cmd(TIM8, ENABLE);
}

void TIM8_PWM_PORT_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  /*GPIOB Configuration: TIM8 channel  2, 3*/
  GPIO_InitStructure.GPIO_Pin =  (GPIO_Pin_7 | GPIO_Pin_8 );
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void FAST_ADC_ConfigDualRegular(void) {
	/* Private typedef ---------------------------------------------------------*/
	
	ADC_InitTypeDef  ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;
	
	 /* ADCCLK = PCLK2/4 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
    
  /* Enable peripheral clocks ------------------------------------------------*/
  /* Enable DMA1 and DMA2 clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);

  /* Enable ADC1, and GPIOA clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1| RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	// Configura i pin FMIS
	GPIO_InitStruct.GPIO_Pin = (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
		
	GPIO_PinRemapConfig( GPIO_Remap_ADC1_ETRGREG, ENABLE);
	GPIO_PinRemapConfig( GPIO_Remap_SWJ_NoJTRST, ENABLE);
	
	// Configura i pin SMIS
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Pin = (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	/* Put everything back to power-on defaults */
	ADC_DeInit(ADC1);
	ADC_DeInit(ADC2);
	
	/* Enable Vrefint channel17 */
	ADC_TempSensorVrefintCmd(ENABLE);
	
	// Enable the DMA1 channel1 interrupt
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	  /* Enable ADC1_2 IRQChannel */
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  /* TIM1 configuration ------------------------------------------------------*/ 
  /* Time Base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 36000;          
  TIM_TimeBaseStructure.TIM_Prescaler = 0x2;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  
  /* TIM1 channel1 configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
  TIM_OCInitStructure.TIM_Pulse = 0x7F; 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;         
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	
/* DMA1 channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)FAST_ADC_Data;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = DMADATA_SIZE/2 ;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);  
  /* Enable DMA1 channel1 */
  DMA_ITConfig(DMA1_Channel1, DMA_IT_TC | DMA_IT_HT, ENABLE);	
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	 /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_RegInjecSimult; //ADC_Mode_RegSimult;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_Ext_IT11_TIM8_TRGO;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 2;
	ADC_Init(ADC1, &ADC_InitStructure);
  /* ADC1 Regular Channels */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5); 	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_1Cycles5); 
	
  /* Set Injected sequencer length on ADC1 */
  ADC_InjectedSequencerLengthConfig(ADC1, 3);
  /* ADC1 Injected Channel Configuration */ 
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_7Cycles5);
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_13, 2, ADC_SampleTime_7Cycles5);
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_17, 3, ADC_SampleTime_7Cycles5); //Vrefint
	
  /* ADC1 injected external trigger configuration */
  ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T1_CC4);
  /* Enable injected external trigger conversion on ADC1*/
  ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);

  /* Enable ADC1 JEOC interrupt */
  ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);

	/* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
	ADC_ExternalTrigConvCmd(ADC1, ENABLE);	
	
	  /* ADC2 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 2;
  ADC_Init(ADC2, &ADC_InitStructure);
	
  /* ADC2 regular channels configuration */ 
	ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5); 	
	ADC_RegularChannelConfig(ADC2, ADC_Channel_3, 2, ADC_SampleTime_1Cycles5); 
	
	ADC_InjectedSequencerLengthConfig(ADC2, 3);
  /*  ADC2 Injected Channel Configuration */ 
	ADC_InjectedChannelConfig(ADC2, ADC_Channel_11, 1, ADC_SampleTime_7Cycles5);
	ADC_InjectedChannelConfig(ADC2, ADC_Channel_14, 2, ADC_SampleTime_7Cycles5);
	ADC_InjectedChannelConfig(ADC2, ADC_Channel_15, 3, ADC_SampleTime_7Cycles5);
	
  /* ADC2 injected external trigger config */
  ADC_ExternalTrigInjectedConvConfig(ADC2, ADC_ExternalTrigInjecConv_T1_CC4);  
  /* Enable injected external trigger conversion on ADC2 */
  ADC_ExternalTrigInjectedConvCmd(ADC2, ENABLE);	
	
	/* Enable ADC2 external trigger conversion */
  ADC_ExternalTrigConvCmd(ADC2, ENABLE);
	
	TIM_Cmd(TIM1, ENABLE);  
  /* TIM1 main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);  
	
	ADC_Cmd(ADC1, ENABLE);	
	/* Enable ADC1 reset calibaration register */ 
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));

 /* Enable ADC2 */
  ADC_Cmd(ADC2, ENABLE);
  /* Enable ADC2 reset calibration register */   
  ADC_ResetCalibration(ADC2);
  /* Check the end of ADC2 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC2));
  /* Start ADC2 calibration */
  ADC_StartCalibration(ADC2);
  /* Check the end of ADC2 calibration */
  while(ADC_GetCalibrationStatus(ADC2));
}

void FAST_ADC_SetTCFromFrequency(uint32_t freq)
{
		uint16_t NARR=490;
		uint32_t TC;
	
		if(freq==1000){
			NARR=490;
		}
		else
		{		
				TC=(500000l/freq);
				TC-=TC/64;
			  NARR=TC;
		}
		TIM8->ARR = NARR;
}

void FAST_ADC_Init(void) 
{	
	TIM8_Config();	
	TIM8_PWM_PORT_Config();
	FAST_ADC_ConfigDualRegular();
}

tFGauge FGauge;
float v1c= VoC;
float i1c= IoC;
float VSQ;
void FAST_ADC_Clk(void) 
{
	static int8_t firstcounter=10;
	if(firstcounter)
	{
		firstcounter--;
		return;
	}
	//------------- Statistica del primo Ordine;
	FGauge.VAout=(M1-L1)* VoutC;
	FGauge.IAout=(M2-L2)* IoutC;
	
	FGauge.PPout = P1 * PoutC;
	VSQ= (float)V1;
	FGauge.Vorms=  sqrt(VSQ)*v1c;
	VSQ = VSQ* V2C;
	FGauge.Iorms=  sqrt((float)I1)*i1c;
	Power = FGauge.Vorms*FGauge.Iorms;
	Qout = sqrt(Power*Power-Pout*Pout);
	
	if(FGauge.Iorms>1e-2)
	{
			
		Zext =fabs(FGauge.Vorms/FGauge.Iorms);
		if(FGauge.PPout> 0.1)
		{
			FGauge.RL=  fabs(VSQ /FGauge.PPout);
			FGauge.XL=  fabs(VSQ /FGauge.PQout);
			FGauge.Qr=  FGauge.PQout/FGauge.PPout;
		}
		else
		{
			FGauge.RL=  3e4;
			FGauge.XL=  4e4;
			FGauge.Qr= 0.75;
			
		}

	}
	else
	{
			Zext = 4e4;
			FGauge.RL=  3e4;
			FGauge.XL=  4e4;
			FGauge.Qr= 0.75; 
	}
	
	
	
	
	
	
}
