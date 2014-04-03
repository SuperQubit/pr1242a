#define BIAS_V1_VALUE	 TIM4_CH2_DUTY				
//BIAS_V1_VALUE --> Duty del BIAS V1 da 0 a 4095 fondo scala Rinfrescato dal Clock TIM4_PWM_Clk();
#define BIAS_V2_VALUE	 TIM4_CH3_DUTY				
//BIAS_V2_VALUE --> Duty del BIAS V2 da 0 a 4095 fondo scala Rinfrescato dal Clock TIM4_PWM_Clk();


#define BIAS_ADJ_ON()  		(GPIOC->ODR |= GPIO_Pin_9) 
#define BIAS_ADJ_OFF() 		(GPIOC->ODR &= ~GPIO_Pin_9) 
#define BIAS_ADJ_TOG() 		(GPIOC->ODR ^= GPIO_Pin_9) 

extern void BIAS_VBIAS_1_WriteValue(uint16_t val); /*Chimando questo rinfresco subito il Registro del TIMer */
extern void BIAS_VBIAS_2_WriteValue(uint16_t val); /*Chimando questo rinfresco subito il Registro del TIMer */

extern uint16_t BIAS_DAC_Value; /*Valore del Registro che viene scritto nella periferica solo se cambia il suo valore*/
extern void BIAS_SetValue(uint16_t val_dac); /*Imposta il Registro del DAC 0 .. 4095   fondo scala 3.3V*/
extern char BIAS_VAUX_GetValue(void);
extern void BIAS_VAUX_SetValue(char val);

extern void BIAS_Init(void);
extern void BIAS_Clk(void);
