/***********************************************************************//**
 * @file         auxfunction.h
 * @brief        Contains all auxiliary function prototypes
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
 * @ingroup AUX_FUNC
 * @{
 */

#ifndef AUX_FUNC_H_
#define AUX_FUNC_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"
#include "config.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Public Functions ----------------------------------------------------------- */
/** @defgroup AUX_FUNC_Public_Functions CONFIG Public Functions
 * @{
 */

/* ------------ LEDS ------------ */
void redLedOn(void);
void greenLedOn(void);




/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif /* AUX_FUNC_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
