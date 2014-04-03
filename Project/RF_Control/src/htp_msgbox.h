/**
  ******************************************************************************
  * @file    htp_msgbox.h
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
#ifndef __HTP_MBOX_H
#define __HTP_MBOX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/** @addtogroup HTPMBOX
  * @{
  */
#include "bsp.h"

#define SPI2_RIDX_PERCPOWER			0
#define SPI2_RIDX_MODULATOR			2
#define SPI2_RIDX_MANIPOLO			4
#define SPI2_RIDX_FREQUENZA			6
#ifdef HTP_V2
#define SPI2_RIDX_VAR_POWER			8
#define SPI2_RIDX_VAR_TRIGON		9
#define SPI2_RIDX_VAR_TRIGOFF 	10
#define SPI2_RIDX_VAR_STIMULUS	11
#endif

#define SPI2_TIDX_APPPOWER		0
#define SPI2_TIDX_ACTPOWERR		1
#define SPI2_TIDX_RCTPOWER		2
#define SPI2_TIDX_MODULATOR		3
#define SPI2_TIDX_PERCPOWER		4
#define SPI2_TIDX_FREQUENZA		5
#define SPI2_TIDX_FREQREAL		6
#define SPI2_TIDX_MANIPOLOL		7
#ifdef HTP_V2
#define SPI2_TIDX_DCPOWER		0
#define SPI2_TIDX_VAR_POWER			8
#define SPI2_TIDX_VAR_TRIGON		9
#define SPI2_TIDX_VAR_TRIGOFF 	10
#define SPI2_TIDX_VAR_STIMULUS	11
#endif

#define SPI2RX_PERCPOWER		(SPI2_Buffer_Rx[SPI2_RIDX_PERCPOWER])
#define SPI2RX_MODULATOR		(SPI2_Buffer_Rx[SPI2_RIDX_MODULATOR])
#define SPI2RX_MANIPOLO			(SPI2_Buffer_Rx[SPI2_RIDX_MANIPOLO])
#define SPI2RX_FREQUENZA		(SPI2_Buffer_Rx[SPI2_RIDX_FREQUENZA])
#ifdef HTP_V2
#define SPI2RX_VAR_POWER		(SPI2_Buffer_Rx[SPI2_RIDX_VAR_POWER])
#define SPI2RX_VAR_TRIGON		(SPI2_Buffer_Rx[SPI2_RIDX_VAR_TRIGON])
#define SPI2RX_VAR_TRIGOFF 	(SPI2_Buffer_Rx[SPI2_RIDX_VAR_TRIGOFF])
#define SPI2RX_VAR_STIMULUS	(SPI2_Buffer_Rx[SPI2_RIDX_VAR_STIMULUS])
#endif

#define SPI2TX_APPPOWER		  (SPI2_Buffer_Tx[SPI2_TIDX_APPPOWER])
#define SPI2TX_ACTPOWERR		(SPI2_Buffer_Tx[SPI2_TIDX_ACTPOWERR])
#define SPI2TX_RCTPOWER		  (SPI2_Buffer_Tx[SPI2_TIDX_RCTPOWER])
#define SPI2TX_MODULATOR		(SPI2_Buffer_Tx[SPI2_TIDX_MODULATOR])
#define SPI2TX_PERCPOWER		(SPI2_Buffer_Tx[SPI2_TIDX_PERCPOWER])
#define SPI2TX_FREQUENZA		(SPI2_Buffer_Tx[SPI2_TIDX_FREQUENZA])
#define SPI2TX_FREQREAL		  (SPI2_Buffer_Tx[SPI2_TIDX_FREQREAL])
#define SPI2TX_MANIPOLOL		(SPI2_Buffer_Tx[SPI2_TIDX_MANIPOLOL])

#ifdef HTP_V2
#define SPI2TX_DCPOWER		  (SPI2_Buffer_Tx[SPI2_TIDX_DCPOWER])
#define SPI2TX_VAR_POWER		(SPI2_Buffer_Tx[SPI2_TIDX_VAR_POWER])
#define SPI2TX_VAR_TRIGON		(SPI2_Buffer_Tx[SPI2_TIDX_VAR_TRIGON])
#define SPI2TX_VAR_TRIGOFF 	(SPI2_Buffer_Tx[SPI2_TIDX_VAR_TRIGOFF])
#define SPI2TX_VAR_STIMULUS	(SPI2_Buffer_Tx[SPI2_TIDX_VAR_STIMULUS])
#endif

/** @defgroup HTPMBOX_Exported_Types
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

/** @defgroup HTPMBOX_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @defgroup HTPMBOX_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup HTPMBOX_Exported_Functions
  * @{
  */
extern void HTP_MBOX_Init(void) ;
extern void HTP_MBOX_Clk(void);



#ifdef __cplusplus
}
#endif

#endif /* __HTPMBOX_H */

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
