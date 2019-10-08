/***********************************************************************//**
 * @file        config.c
 * @brief       Contains all functions support for CONFIG firmware library on LPC17xx
 * @version     1.0
 * @date        6. October. 2019
 * @author      Monsierra Lucas Gabriel - Lenta Luis
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
 **********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup TP_FINAL
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "config.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_exti.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */
#ifdef __BUILD_WITH_EXAMPLE__
#include "lpc17xx_libcfg.h"
#else
#include "lpc17xx_libcfg_default.h"
#endif /* __BUILD_WITH_EXAMPLE__ */


#ifdef _AUX_FUNC

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup AUX_FUNC_Public_Functions
 * @{
 */

/* AUX_FUNC ------------------------------------------------------------------------------ */

/*********************************************************************//**
 * @brief       Turn on red led (LPC 1769 rev. D)
 * @param 		None
 * @return 		None
 **********************************************************************/
void redLedOn(void)
{
	GPIO_ClearValue(PINSEL_PORT_0, (1 << REDLED));
	GPIO_SetValue(PINSEL_PORT_3, (1 << GREENLED));
}

/*********************************************************************//*
 * @brief    	    Turn on green led
 * @param[in]		None
 * @return 			None
 ***********************************************************************/
void greenLedOn(void)
{
	GPIO_ClearValue(PINSEL_PORT_3, (1 << GREENLED));
	GPIO_SetValue(PINSEL_PORT_0, (1 << REDLED));
}

/*********************************************************************//*
 * @brief    	    Configuration for EXTI
 * @param[in]		None
 * @return 			None
 ***********************************************************************/


/*********************************************************************//*
 * @brief    	    Configuration for GPIO
 * @param[in]		None
 * @return 			None
 ***********************************************************************/



/**
 * @}
 */

#endif /* _AUX_FUNC */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
