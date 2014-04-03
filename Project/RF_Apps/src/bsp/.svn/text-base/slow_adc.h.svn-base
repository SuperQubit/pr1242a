/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SLOW_ADC_H
#define __SLOW_ADC_H
typedef struct
{
	float VBAR;
	float IBAR;
	float VOUT;		
	float IOUT;
	float TEMP;
	float VREF;
} tSlowGauge;

// Rapporto 1/5
//#define VOUT_CONV   		(3.3/4096.0/ 0.0048)
//#define VOUT_CONV_OFF   (18.75)
// Rapporto 1/4
#define VOUT_CONV   		(3.3/4096.0/ 0.006)
#define VOUT_CONV_OFF   (15)
// Ibar 58K/2
//#define IBAR1_CONV  		(3.3/4096.0/ 0.25)
// Ibar 58K/3
//#define IBAR1_CONV  		(3.3/4096.0/ 0.167)
//#define IBAR2_CONV  		(3.3/4096.0/ 2.25)

// Ibar 22K (Tipo E)
#define IBAR1_CONV  		(3.3/4096.0/ 0.2)
#define IBAR2_CONV  		(3.3/4096.0/ 2.2)
// Generic
#define VBAR_CONV  			(3.3/4096.0 / 0.04)

#define IOUT_CONV   		(3.3/4096.0/ 7.2*4.0)
#define TEMP_CONV   		(3.3/40.96)

#define VREF_CONV   		(3.3/4096.0)
#define VREF_NOMINAL       (1.25)
#define VREF_NOMINAL_RAW    

extern tSlowGauge SLOW_Gauge;

#define ADCTC_FAST		(1)
#define ADCTC_SLOW		(0) 

extern uint16_t SLOW_ADC_TimeCostant;
extern uint16_t SLOW_ADC_Data[]; 
extern uint16_t SLOW_ADC_Data_Avg[]; 	
extern uint16_t SLOW_ADC_Data_Avg1[]; 	
extern uint32_t SLOW_ADC_Data32[]; 

extern void SLOW_ADC_IBar_SetScale(char scale);
extern void SLOW_ADC_PhisCalc(void);

extern void SLOW_ADC_Update(void);
extern void SLOW_ADC_Init(void) ;
extern void SLOW_ADC_Clk(void);





#endif 

