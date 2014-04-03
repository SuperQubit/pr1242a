#include "hal.h"
#include "bias.h"

/*DAC_Output = Vref x DOR/4095 */

#define DAC_OUT_BOOT (2482)			//2V per Allign_R
uint16_t BIAS_DAC_Value;
volatile char VAUXState=0;

void BIAS_Config(void){
	DAC_InitTypeDef DAC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_DeInit();
  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	 /* DAC channel1 Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude =  DAC_TriangleAmplitude_1;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	/* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is 
     automatically connected to the DAC converter. */
  DAC_Cmd(DAC_Channel_1, ENABLE);
  /* Set DAC Channel1 DHR12L register */
  //DAC_SetChannel1Data(DAC_Align_12b_L, 0x7FF0);
  //DAC_SetChannel1Data(DAC_Align_12b_L, 0x9B20);
	//DAC_SetChannel1Data(DAC_Align_12b_R, 0x04D9);
	//BIAS_DAC_Value = DAC_GetDataOutputValue(DAC_Channel_1);
	DAC_SetChannel1Data(DAC_Align_12b_R, DAC_OUT_BOOT);
	//BIAS_DAC_Value = DAC_GetDataOutputValue(DAC_Channel_1);
	DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
	BIAS_DAC_Value = DAC_GetDataOutputValue(DAC_Channel_1);
  /* Once the DAC channel is enabled, the corresponding GPIO pin is automatically 
     connected to the DAC converter. In order to avoid parasitic consumption, 
     the GPIO pin should be configured in analog */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	 /* Used peripherals clock enable -------------------------------------------*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	 /* configure PC9 as output push-pull  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_9);	
}
char BIAS_VAUX_GetValue(void){	
	return(VAUXState);
}

extern void BIAS_VAUX_SetValue(char val){
		switch(val)
		{
				case 0:
					VAUXState=0;
					GPIO_SetBits(GPIOC, GPIO_Pin_9);
					break;
			
				case 1:
					VAUXState=1;
					GPIO_ResetBits(GPIOC, GPIO_Pin_9);
					break;
		}
}

void BIAS_VBIAS_1_WriteValue(uint16_t val){
	if(val < 4095) 	TIM4_CH2_DUTY=val;
		else TIM4_CH2_DUTY=4095;
		TIM4->CCR2 = TIM4_CH2_DUTY;
}

void BIAS_VBIAS_2_WriteValue(uint16_t val){
	if(val < 4095) 	TIM4_CH3_DUTY=val;
		else TIM4_CH3_DUTY=4095;
		TIM4->CCR3 = TIM4_CH3_DUTY;
}

void BIAS_SetValue(uint16_t val_dac){
		if (val_dac < 4096) DAC_SetChannel1Data(DAC_Align_12b_R, val_dac);
		else DAC_SetChannel1Data(DAC_Align_12b_R, 4095);
		
		DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);		 
		BIAS_DAC_Value = DAC_GetDataOutputValue(DAC_Channel_1);	
}

void BIAS_Init(void){
	TIM4_PWM_Init();
	BIAS_Config();
	BIAS_VAUX_SetValue(0);
	BIAS_SetValue(4095);
	BIAS_VBIAS_1_WriteValue(4095);
	BIAS_VBIAS_2_WriteValue(4095);
}

void BIAS_Clk(void){
	if (DAC_GetDataOutputValue(DAC_Channel_1) ^ BIAS_DAC_Value)
		{
			 BIAS_SetValue(BIAS_DAC_Value);
			 BIAS_DAC_Value = DAC_GetDataOutputValue(DAC_Channel_1);
		}		
}
	


