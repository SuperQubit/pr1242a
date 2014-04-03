/**
  ******************************************************************************
  * @file    control.h
  * @author  DL
  * @version V1.00
  * @date    22-04-2013
  * @brief   This file provides all the Control System header declarations.
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; COPYRIGHT 2013 ArSilicii</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROL_H
#define __CONTROL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/** @addtogroup CONTROL
  * @{
  */

	 	 

/** @defgroup CONTROL_Exported_Types
  * @{
  */

typedef enum
{
	SP_OK=0,
	SP_HIGH,
	SP_LOW,
	SP_ERROR
} tSetPointResult;
	 
typedef struct
{
	uint16_t DDS_AMPL;
	uint16_t PWR_VBAR;
	uint16_t BIAS_SET;		
} RAWSetPoint;
	 	 
typedef enum
{
	NONE=0,
	TABLE_1,
	PRECOG_SOA_MAXPWR,
	CLASS_A,
	CLASS_B,
	CLASS_AB,
	HTPH1,
	FVIP
} tControlMethod;

typedef void * tControlParameter;

typedef enum
{
	UNKNOWN=0,
	LIGHT,
	RESISTIVE,
	CAPACITIVE_SAFE,
	CAPACITIVE
} tLoadType;

typedef enum
{
	CON_NONE=0,
	CON_VMAX,
	CON_PMAX,
	CON_IMAX,
	CON_VMAXPMAX
} tPercentMode;

typedef enum
{
	MODE_NONE=0,
	MODE_RAW,
	MODE_SOA
} tControlMode;

typedef struct
{	
	float VMAX;
	float IMAX;
	float PMAX;
	float	VDMIN;
  float pKamp;
  float pKdT;
	float pKiup;
  float pKidw;
} tSOAParameter;

typedef struct
{	
	tControlMode ControlMode;
	tPercentMode PercentMode;
	tSOAParameter SOAP;
} tControlSysProperties;

typedef struct
{
	tLoadType Type;
	tPercentMode Mode;	
	float VMAX;
	float IMAX;
	float PMAX;
	float	VDMIN;
} tLoadProperties;


/**
  * @}
  */

/** @defgroup CONTROL_Exported_Constants
  * @{
  */
extern RAWSetPoint ActualSetPoint;
extern tLoadProperties LoadProperties;
extern tControlSysProperties ConSys;
/**
  * @}
  */

/** @defgroup CONTROL_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup CONTROL_Exported_Functions
  * @{  */

void CONTROL_CalcOCSetPoint(uint16_t percent);

void CONTROL_SetPercent(uint16_t percent);
void CONTROL_SetFrequency(uint16_t frequency);
extern uint8_t CONTROL_GetLoadStatus(void);

void CONTROL_Setup(tControlMethod, tControlParameter);
void CONTROL_SetParameter(tControlMethod, tControlParameter);
void CONTROL_GetParameter(tControlMethod, tControlParameter);

void CONTROL_Init(void);
void CONTROL_Clk(void);

extern tLoadType LOAD_Analyzer(void);
extern tSetPointResult VBAR_PIDController(uint16_t  VDMin_SetPoint);
extern tSetPointResult Amplitude_PIV_PIDController(uint16_t PSetPoint, uint16_t VMAXSetPoint,uint16_t IMAXSetPoint);
extern tSetPointResult BIASOptimizer_Controller(int16_t Offset);
extern void CONTROL_Output(void);
extern void SOA_Control_Init(void);
extern void SOA_Control_SetIC(RAWSetPoint IC);
extern RAWSetPoint SOA_Control_GetIC(void);


extern float fKamp;
extern float fKdT;
extern float VOUT_MAX;
extern float IOUT_MAX;
extern float POUT_MAX;
extern float VDMIN_SET;
extern float Kiup;
extern float Kidw;

#ifdef __cplusplus
}
#endif

#endif /* __CONTROL_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2013 ArSilicii *****END OF FILE****/
