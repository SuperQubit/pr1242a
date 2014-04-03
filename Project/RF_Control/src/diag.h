/**
  ******************************************************************************
  * @file    diag.h
  * @author  DL
  * @version V1.00
  * @date    22-04-2013
  * @brief   This file provides all the Diagnostic header declarations.
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; COPYRIGHT 2013 ArSilicii</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DIAG_H
#define __DIAG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/** @addtogroup DIAG
  * @{
  */

/** @defgroup DIAG_Exported_Types
  * @{
  */
	 
typedef enum
{
	DIAG_OK=0,
	DIAG_BUSY,
	DIAG_ERROR
} tDiagStatus;
	 
typedef enum
{
	SYSID_NONE=0,
	SYSID_TIPOD,
	SYSID_TIPOE,
	SYSID_TIPOEL,
} tDiagSysID;


typedef struct 
{
	uint32_t SYSID;
	uint32_t VBIASP_BORDER;
	uint16_t VBIASP_GATE_1;
	uint16_t VBIASP_GATE_2;
	uint16_t VBIASP1_GATE_1;
	uint16_t VBIASP1_GATE_2;	
	float TINI;
	float VBAR_NOM;
	float IBARP_OFFSET;
	float IBARP_BORDER;
	float IBARP_GATE_1;
	float IBARP_GATE_2;
	float IBARP1_GATE_1;
	float IBARP1_GATE_2;
	float VT;
	float VT_1;
	float VT_2;
	float BETA_1;
	float BETA_2;
	float NKAMP;
} tDiagReport;

extern tDiagReport DiagReport;
/**
  * @}
  */

/** @defgroup DIAG_Exported_Constants
  * @{
  */
/**
  * @}
  */

/** @defgroup DIAG_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup DIAG_Exported_Functions
  * @{
  */

tDiagStatus DIAG_StatusCheck(void);
void DIAG_Init(void);
void DIAG_Clk(void);

#ifdef __cplusplus
}
#endif

#endif /* __DIAG_H */

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
