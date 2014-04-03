


#define DDS_SPREAD_ON()  		(GPIO_SetBits(GPIOB, GPIO_Pin_4)) 
#define DDS_SPREAD_OFF() 		(GPIO_ResetBits(GPIOB, GPIO_Pin_4))

#define DDS_SCALE_ON()  		(GPIO_SetBits(GPIOB, GPIO_Pin_5)) 
#define DDS_SCALE_OFF() 		(GPIO_ResetBits(GPIOB, GPIO_Pin_5)) 

#define DDS_FSELECT_ON()  		(GPIOA->ODR |= GPIO_Pin_6) 
#define DDS_FSELECT_OFF() 		(GPIOA->ODR &= ~GPIO_Pin_6) 
#define DDS_FSELECT_TOG() 		(GPIOA->ODR ^= GPIO_Pin_6) 


#define DDS_FSELECT_TOG() 		(GPIOA->ODR ^= GPIO_Pin_6) 

#define DDS_ADJ_DUTY	 TIM4_CH4_DUTY				//DDS_ADJ_DUTY --> Duty del ADJ da 0 a 4095 fondo scala Rinfrescato dal Clock TIM4_PWM_Clk();

extern void DDS_ADJ_Update(uint16_t val);		//Aggiorna immediatamente il registro del TIMER

extern uint16_t DDS_AmplitudeGet(void);
extern char  DDS_AmplitudeSet(uint16_t amp); 

extern void DDS_FRQ_Set(uint16_t val);			//Scrive il valore SPI1_Freq_Reg del modulo spi1.h   Ricordati di staccarlo dall'SPI dell HTP
extern void DDS_Init(void);
extern void DDS_Clk(void);



