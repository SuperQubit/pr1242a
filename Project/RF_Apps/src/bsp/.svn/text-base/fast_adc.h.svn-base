/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FAST_ADC_H
#define __FAST_ADC_H

#define DMADATA_SIZE	(128)
#define NSAMPLE_POW2	(4)

#define VoD 	(0.077)
// Rapporto 1/3
//#define VoC (VoD * 3.0)
// Rapporto 1/4
#define VoC (VoD * 4.0)
// Rapporto 1/5
//#define VoC (VoD * 5.0)

// Rapport Io 1/4
#define IoC	(8E-4)
// Rapport Io 1/2
//#define IoC	(4.25E-4)

#define V2C		(VoC*VoC)
#define VoutC (VoC/2.82)
#define IoutC (IoC/2.82)
#define PoutC (VoC*IoC)

// Output Forward Conversion x Setpoint

#define VBAR2PWM(x)			((uint16_t)((x)*64.0))
#define PWM2VBAR(x)			((float)(x)/64.0)
#define VD2MIS(x)				((uint16_t)(x/VoD))
//#define VDMIS2PWMBAR(x)	((uint16_t)((x)*VoD*8.0))
#define VDMIS2PWMBAR(x)	((uint16_t)((x)*VoD*6.0))

extern uint16_t FAST_ADC_Data[DMADATA_SIZE]; 
extern uint16_t A1,A2,A3,A4;
extern uint16_t M1,M2,M3,M4;
extern uint16_t L1,L2,L3,L4;
extern uint16_t Vopp_i,Iopp_i;

typedef struct
{
	float VAout;		// Tensione in uscita in V, Vpp/fattore di forma;
	float Vorms;		// Tensione in uscita in V, true rms;
	float IAout;		// Corrente in uscita in A, Ipp/fattore di forma;
	float Iorms;		// Corrente in uscita in A, true rms;
	float PPout;	  // Potenza attiva in uscita in W
	float PAout;		// Potenza apparente in uscita in VA
	float PQout;		// Potenza reattiva e deformante in uscita in VAR
	float Qr;				// Fattpre di merito
	float ZL;				// Impedenza del carico in Ohm
	float XL;				// Reattanza del carico in Ohm
	float RL;				// Resistenza del carico in Ohm
} tFGauge;

extern tFGauge FGauge;

#define Vout	(FGauge.VAout)
#define Iout	(FGauge.IAout)
#define Pout	(FGauge.PPout)
#define Zext	(FGauge.ZL)
#define Power	(FGauge.PAout)
#define Qout	(FGauge.PQout)

extern void FAST_ADC_Init(void) ;
extern void FAST_ADC_Clk(void) ;
extern void FAST_ADC_1HALF_Process(void);
extern void FAST_ADC_2HALF_Process(void);
extern void FAST_ADC_SetTCFromFrequency(uint32_t freq);
#endif
