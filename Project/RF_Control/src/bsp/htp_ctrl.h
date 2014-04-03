/**
  ******************************************************************************
  * @file    htp_ctrl.h
  * @author  DL
  * @version V1.00
  * @date    26-06-2013
  * @brief   This file provides all the HTP protocol decoder header declarations.
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; COPYRIGHT 2013 ArSilicii</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HTP_CTRL_H
#define __HTP_CTRL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/** @addtogroup HTPCTRL
  * @{
  */
#include "bsp.h"
 	 

/** @defgroup HTPCTRL_Exported_Types
  * @{
  */
	 
typedef struct
{
	uint8_t PercPower;
	uint8_t Modulazione;		
	uint8_t Manipolo;
	uint8_t Frequenza;
#ifdef HTP_V2
	int8_t  Var_Power;
	int8_t  Var_TriggerON;
	int8_t  Var_TriggerOFF;
	int8_t  Var_Stimulus;
#endif	
} tHTPInFrame;

typedef struct
{
	uint8_t Power_App;
	uint8_t Power_Active;
	uint8_t Power_Reactive;
	uint8_t ModulatoreActual;
	uint8_t PercPowerActual;	
	uint8_t Frequenza;
	uint8_t FrequenzaReal;
	uint8_t Manipolo;
	uint8_t LoadStatus;
#ifdef HTP_V2
	int8_t  Var_Power;
	int8_t  Var_TriggerON;
	int8_t  Var_TriggerOFF;
	int8_t  Var_Stimulus;
#endif		
} tHTPOutFrame;

typedef struct
{
	uint8_t Status;
	tHTPInFrame  InMessage;
	tHTPOutFrame OutMessage;
} tHTPMailBox;

/**
  * @}
  */

/** @defgroup HTPCTRL_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @defgroup HTPCTRL_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup HTPCTRL_Exported_Functions
  * @{
  */
extern void HTP_CTRL_Init(void) ;
extern void HTP_CTRL_Clk(void);



#ifdef __cplusplus
}
#endif

#endif /* __HTPCTRL_H */

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
