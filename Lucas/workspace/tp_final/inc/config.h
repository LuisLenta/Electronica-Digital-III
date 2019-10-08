/***********************************************************************//**
 * @file         config.h
 * @brief        Contains all macro definitions and function prototypes
 *               support for TP_FINAL
 * @version      1.0
 * @date         6. October. 2019
 * @author       Monsierra Lucas Gabriel - Lenta Luis
 **************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **************************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup TP_FINAL
 * @ingroup CONFIGURACIONES
 * @{
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Private Macros ------------------------------------------------------------- */
/** @defgroup CONFIG_Private_Macros CONFIG Private Macros
 * @{
 */
/*********************************************************************//**
 * Macro defines for CONFIG control register
 **********************************************************************/
#define INPUT		0
#define OUTPUT		1
#define TX			2
#define RX			3
#define AOUT		26
#define ADC			23
#define EINT0		10
#define EINT1		11
#define REDLED 		22
#define GREENLED 	25
#define BAUDRATE	9600


/**
 * @}
 */
/* Public Functions ----------------------------------------------------------- */
/** @defgroup CONFIG_Public_Functions CONFIG Public Functions
 * @{
 */

void CONFIG_Init(void);

void CONFIG_pinselConfig(void);
void CONFIG_extiConfig(void);
void CONFIG_gpioConfig(void);
void CONFIG_uart(void);
void CONFIG_gpdmaConfig(void);


void CONFIG_dacConfig(void);
void CONFIG_adcConfig(void);
void CONFIG_gpioIntConfig(void);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif /* CONFIG_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
