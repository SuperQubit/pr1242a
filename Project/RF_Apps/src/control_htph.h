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
#ifndef __CONTROL_HTPH_H
#define __CONTROL_HTPH_H

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

//------------------------  HTP_Heuristic_1 (HTPH1) Control

	 
typedef enum
{
	NO_HANDPIECE=0,
	PLASTIC,
	CERAMIC,
	BALLS,
	PEN,
	NAIL,
	EMI=128
} tHandpiece;

typedef enum
{
	PULSE_MODE_OFF=0,
	PULSE_MODE_CONTINUOS=255
} tPulse;

typedef struct
{	
	tHandpiece Handpiece;
	tPulse	Pulse;
	float Percent;
	uint16_t Frequency;
} tHTPH_Properties;

typedef struct
{
	tHandpiece Handpiece;
} tHTPH_Parameter;


/**
  * @}
  */

/** @defgroup CONTROL_Exported_Constants
  * @{
  */
extern tHTPH_Properties HTPH_Properties;

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
  * @{
  */
extern void CONTROL_HTPH_Init(void);	
extern void CONTROL_HTPH_Clk(void);

extern uint16_t CONTROL_SetPulse(tPulse pulse);
extern uint16_t CONTROL_GetPulse(void);
tHandpiece CONTROL_GetManipolo(void);
void CONTROL_SetManipolo(tHandpiece);
extern uint8_t CONTROL_GetLoadStatus(void);
extern uint8_t CONTROL_GetPulseStatus(void);

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
