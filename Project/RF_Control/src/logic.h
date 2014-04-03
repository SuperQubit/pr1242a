/**
  ******************************************************************************
  * @file    logic.h
  * @author  DL
  * @version V1.00
  * @date    22-04-2013
  * @brief   This file provides all the main logic header declarations.
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; COPYRIGHT 2013 ArSilicii</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LOGIC_H
#define __LOGIC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/** @addtogroup LOGIC
  * @{
  */

/** @defgroup LOGIC_Exported_Types
  * @{
  */
	 
/**
  * @}
  */

/** @defgroup LOGIC_Exported_Constants
  * @{
  */
/**
  * @}
  */

/** @defgroup LOGIC_Exported_Macros
  * @{
  */
#define SYS_Version        (0x10)
#define SYS_VersionString  "1.00"
#define SYS_Release        (50)
#define SYS_ReleaseString  "a50"

/**
  * @}
  */

/** @defgroup LOGIC_Exported_Functions
  * @{
  */

void LOGIC_Init(void);
void LOGIC_Clk(void);

#ifdef __cplusplus
}
#endif

#endif /* __LOGIC_H */

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
