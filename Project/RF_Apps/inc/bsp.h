/**
  ******************************************************************************
  * @file    bsp.h
  * @author  DL
  * @version V1.00
  * @date    22-04-2013
  * @brief   This file provides all the BSP header declarations.
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; COPYRIGHT 2013 ArSilicii</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/** @addtogroup BSP
  * @{
  */

#include "..\bsp\dds.h"
#include "..\bsp\bias.h"
#include "..\bsp\slow_adc.h"	 
#include "..\bsp\fast_adc.h"
#include "..\bsp\supply.h"
#include "..\bsp\htp_ctrl.h"
#include "..\bsp\com.h"
	 
/** @defgroup BSP_Exported_Types
  * @{
  */
 
	 	 
/**
  * @}
  */

/** @defgroup BSP_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @defgroup BSP_Exported_Macros
  * @{
  */

#define BSP_Init()		DDS_Init();HTP_CTRL_Init(); SLOW_ADC_Init();\
											FAST_ADC_Init();BIAS_Init();	SUPPLY_Init();COM_Init()
/**
  * @}
  */

/** @defgroup BSP_Exported_Functions
  * @{
  */

//export void BSP_Init(void)

#ifdef __cplusplus
}
#endif

#endif /* __BSP_H */

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
