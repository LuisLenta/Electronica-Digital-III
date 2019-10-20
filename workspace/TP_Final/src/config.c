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
#include "lpc17xx_uart.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */
#ifdef __BUILD_WITH_EXAMPLE__
#include "lpc17xx_libcfg.h"
#else
#include "lpc17xx_libcfg_default.h"
#endif /* __BUILD_WITH_EXAMPLE__ */


#ifdef _CONFIG

uint8_t TABLA [2] = { 'A', 'B' };
/* Public Functions ----------------------------------------------------------- */
/** @addtogroup CONFIG_Public_Functions
 * @{
 */

/* CONFIG ------------------------------------------------------------------------------ */

/*********************************************************************//**
 * @brief       Configuration for all modules to TP_FINAL on LPC1769 rev. D
 * @param 		None
 * @return 		None
 **********************************************************************/
void CONFIG_Init(void)
{
	CONFIG_pinselConfig();
	CONFIG_extiConfig();
	CONFIG_gpioConfig();
	CONFIG_uart();
//	CONFIG_gpdmaConfig();
//	CONFIG_dacConfig();
}

/*********************************************************************//*
 * @brief    	    Configuration for PINSEL
 * @param[in]		None
 * @return 			None
 ***********************************************************************/
void CONFIG_pinselConfig(void)
{
	PINSEL_CFG_Type pincfg;

	/*-------------- Configuration for UART0 TX pin P0.2 --------------*/
	pincfg.Portnum = PINSEL_PORT_0;				// Port 0
	pincfg.Pinnum = PINSEL_PIN_2;				// Pin 2
	pincfg.Pinmode = PINSEL_PINMODE_TRISTATE;	// Without resistance
	pincfg.OpenDrain = PINSEL_PINMODE_NORMAL;	// Without OpenDran
	pincfg.Funcnum = PINSEL_FUNC_1;				// UART0

	PINSEL_ConfigPin(&pincfg);					// Load configuration

	/*-------------- Configuration for UART0 RX pin P0.3 --------------*/
	pincfg.Pinnum = PINSEL_PIN_3;				// Pin 3

	PINSEL_ConfigPin(&pincfg);					// Load configuration
	// Only change pin number

	/*-------------- Configuration for DAC AOUT pin P0.26 -------------*/
	pincfg.Pinnum = PINSEL_PIN_26;				// Pin 26
	pincfg.Pinmode = PINSEL_PINMODE_PULLUP;		// Pull-Up
	pincfg.Funcnum = PINSEL_FUNC_2;				// AOUT

	PINSEL_ConfigPin(&pincfg);					// Load configuration

	/*--------------- Configuration for REDLED pin P0.22 --------------*/
	pincfg.Pinnum = REDLED;						// Pin 22
	pincfg.Funcnum = PINSEL_FUNC_0;				// GPIO

	PINSEL_ConfigPin(&pincfg);					// Load configuration

	/*-------------- Configuration for analogic pin P0.23 -------------*/
	pincfg.Pinnum = PINSEL_PIN_23;				// Pin 23 (ADC0[0])
	pincfg.Funcnum = PINSEL_FUNC_1;				// ADC
	pincfg.Pinmode = PINSEL_PINMODE_TRISTATE;	// Tristate

	PINSEL_ConfigPin(&pincfg);					// Load configuration

	/*--------------- Configuration for EINT0 pin P2.10 ---------------*/
	pincfg.Portnum = PINSEL_PORT_2; 			// Port 2
	pincfg.Pinnum = PINSEL_PIN_10;				// Pin 10
	pincfg.Funcnum = PINSEL_FUNC_1;				// EINT0
	pincfg.Pinmode = PINSEL_PINMODE_PULLUP;		// Pull-Up

	PINSEL_ConfigPin(&pincfg);					// Load configuration

	/*--------------- Configuration for EINT1 pin P2.11 ---------------*/
	pincfg.Pinnum = PINSEL_PIN_11;				// Pin 11
	pincfg.Funcnum = PINSEL_FUNC_1;				// EINT1

	PINSEL_ConfigPin(&pincfg);					// Load configuration

	/*-------------- Configuration for GREENLED pin P3.25 -------------*/
	pincfg.Portnum = PINSEL_PORT_3; 			// Port 3
	pincfg.Pinnum = GREENLED;					// Pin 25
	pincfg.Funcnum = PINSEL_FUNC_0;				// GPIO

	PINSEL_ConfigPin(&pincfg);					// Load configuration
}

/*********************************************************************//*
 * @brief    	    Configuration for EXTI
 * @param[in]		None
 * @return 			None
 ***********************************************************************/
void CONFIG_extiConfig(void)
{
	EXTI_InitTypeDef exti_cfg;

	/*-------------------- Configuration for EINT0 --------------------*/
	exti_cfg.EXTI_Line = EXTI_EINT0;
	exti_cfg.EXTI_Mode = EXTI_MODE_LEVEL_SENSITIVE;	// Low level
	exti_cfg.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;
	EXTI_Config(&exti_cfg);							// Load configuration
	NVIC_EnableIRQ(EINT0_IRQn);						// Enable interruption

	/*-------------------- Configuration for EINT1 --------------------*/
	exti_cfg.EXTI_Line = EXTI_EINT1;
	EXTI_Config(&exti_cfg);							// Load configuration
	NVIC_EnableIRQ(EINT1_IRQn);						// Enable interruption
}

/*********************************************************************//*
 * @brief    	    Configuration for GPIO
 * @param[in]		None
 * @return 			None
 ***********************************************************************/
void CONFIG_gpioConfig(void)
{
	/*------------------- Configuration for UART0 TX ------------------*/
	GPIO_SetDir(PINSEL_PORT_0, (1 << TX), OUTPUT);
	/*------------------- Configuration for UART0 RX ------------------*/
	GPIO_SetDir(PINSEL_PORT_0, (1 << RX), INPUT);
	/*-------------------- Configuration for AOUT ---------------------*/
	GPIO_SetDir(PINSEL_PORT_0, (1 << AOUT), OUTPUT);
	/*--------------------- Configuration for ADC ---------------------*/
	GPIO_SetDir(PINSEL_PORT_0, (1 << ADC), INPUT);
	/*-------------------- Configuration for REDLED -------------------*/
	GPIO_SetDir(PINSEL_PORT_0, (1 << REDLED), OUTPUT);
	/*-------------------- Configuration for EINT0 --------------------*/
	GPIO_SetDir(PINSEL_PORT_2, (1 << EINT0), INPUT);
	/*-------------------- Configuration for EINT1 --------------------*/
	GPIO_SetDir(PINSEL_PORT_2, (1 << EINT1), INPUT);
	/*------------------- Configuration for GREENLED ------------------*/
	GPIO_SetDir(PINSEL_PORT_3, (1 << GREENLED), OUTPUT);
}

/*********************************************************************//*
 * @brief    	    Configuration for UART
 * @param[in]		None
 * @return 			None
 ***********************************************************************/
void CONFIG_uart(void)
{
	/* Initialize UART Configuration parameter structure to default state:
	 * 		- Baudrate = 9600bps
	 * 		- 8 data bit
	 * 		- 1 Stop bit
	 * 		- None parity
	*/
	UART_CFG_Type uartcfg;
	UART_ConfigStructInit(&uartcfg);				// Load configuration
	UART_Init(LPC_UART0, &uartcfg);					// UART init

	/* Initialize FIFOConfigStruct to default state: (except DMAMode)
	 *		- FIFO_DMAMode = ENABLE
	 *      - FIFO_Level = UART_FIFO_TRGLEV0
	 *      - FIFO_ResetRxBuf = ENABLE
	 *      - FIFO_ResetTxBuf = ENABLE
	 *      - FIFO_State = ENABLE
	 */
	UART_FIFO_CFG_Type fifocfg;
	fifocfg.FIFO_DMAMode = ENABLE;					// Enabled DMA Mode
	UART_FIFOConfigStructInit(&fifocfg);			// Load configuration
	UART_FIFOConfig(LPC_UART0, &fifocfg);			// FIFI init

	UART_TxCmd(LPC_UART0, ENABLE);					// UART TX enable
	UART_IntConfig(LPC_UART0, UART_INTCFG_THRE, ENABLE);
	NVIC_EnableIRQ(UART0_IRQn);						// Interruption enable
}

/*********************************************************************//*
 * @brief    	    Configuration for GPDMA
 * @param[in]		None
 * @return 			None
 ***********************************************************************/
void CONFIG_gpdmaConfig(void)
{
	/*------------------- Initialize GPDMA controller ------------------*/
	GPDMA_Init();
	GPDMA_Channel_CFG_Type GPDMACfg;

	/*--------------------- Setting GPDMA interrupt --------------------*/

	NVIC_DisableIRQ (DMA_IRQn);						// Disable interrupt for DMA
	//NVIC_SetPriority(DMA_IRQn, ((0x01<<3)|0x01));	// preemption = 1, sub-priority = 1

	/*---------------- Setup GPDMA channel 0 (Mem to TX) ---------------*/
	GPDMACfg.ChannelNum = 0;						// Channel 0
	GPDMACfg.SrcMemAddr = (uint32_t)TABLA;		// Source memory
	GPDMACfg.DstMemAddr = 0;						// Destination memory - don't care
	GPDMACfg.TransferSize = sizeof(TABLA);		// Transfer size
	GPDMACfg.TransferWidth = 0;						// Transfer width - don't care
	GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_M2P;	// Transfer type
	GPDMACfg.SrcConn = 0;							// Source connection - don't care
	GPDMACfg.DstConn = GPDMA_CONN_UART0_Tx;			// Destination connection (TX)
	GPDMACfg.DMALLI = 0;							// Linker List Item - unused
	GPDMA_Setup(&GPDMACfg);							// Setup channel 0 with given parameter

	/*---------------- Setup GPDMA channel 1 (Rx to Mem) ---------------*/
	GPDMACfg.ChannelNum = 1;						// Channel 1
	GPDMACfg.SrcMemAddr = 0;						// Source memory - don't care
	GPDMACfg.DstMemAddr = (uint32_t)TABLA;		// Destination memory
	GPDMACfg.TransferSize = sizeof(TABLA);			// Transfer size
	GPDMACfg.TransferWidth = 0;						// Transfer width - don't care
	GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_P2M;	// Transfer type
	GPDMACfg.SrcConn = GPDMA_CONN_UART0_Rx;			// Source connection (RX)
	GPDMACfg.DstConn = 0;							// Destination connection - don't care
	GPDMACfg.DMALLI = 0;							// Linker List Item - unused
	GPDMA_Setup(&GPDMACfg);							// Setup channel 1 with given parameter

	/*--------------- Setup GPDMA channel 2 (ADC to Mem) ---------------*/
	GPDMACfg.ChannelNum = 2;						// Channel 1
	GPDMACfg.SrcMemAddr = 0;						// Source memory - don't care
	GPDMACfg.DstMemAddr = (uint32_t)TABLA;		// Destination memory
	GPDMACfg.TransferSize = sizeof(TABLA);			// Transfer size
	GPDMACfg.TransferWidth = 0;						// Transfer width - don't care
	GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_P2M;	// Transfer type
	GPDMACfg.SrcConn = GPDMA_CONN_ADC;				// Source connection (ADC)
	GPDMACfg.DstConn = 0;							// Destination connection - don't care
	GPDMACfg.DMALLI = 0;							// Linker List Item - unused
	GPDMA_Setup(&GPDMACfg);							// Setup channel 1 with given parameter

	/*--------------- Setup GPDMA channel 3 (Mem to DAC) ---------------*/
	GPDMACfg.ChannelNum = 3;						// Channel 0
	GPDMACfg.SrcMemAddr = (uint32_t)TABLA;		// Source memory
	GPDMACfg.DstMemAddr = 0;						// Destination memory - don't care
	GPDMACfg.TransferSize = sizeof(TABLA);		// Transfer size
	GPDMACfg.TransferWidth = 0;						// Transfer width - don't care
	GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_M2P;	// Transfer type
	GPDMACfg.SrcConn = 0;							// Source connection - don't care
	GPDMACfg.DstConn = GPDMA_CONN_DAC;				// Destination connection (DAC)
	GPDMACfg.DMALLI = 0;							// Linker List Item - unused
	GPDMA_Setup(&GPDMACfg);							// Setup channel 0 with given parameter

//	Channel0_TC = 0; 								// Reset terminal counter
//	Channel0_Err = 0;								// Reset Error counter

	//NVIC_EnableIRQ (DMA_IRQn);						// Enable interrupt for DMA
	GPDMA_ChannelCmd(0, ENABLE);					// Disable GPDMA Channel 0 (M2Tx)
	GPDMA_ChannelCmd(1, DISABLE);					// Disable GPDMA Channel 1 (Rx2M)
	GPDMA_ChannelCmd(2, DISABLE);					// Disable GPDMA Channel 2 (ADC2M)
	GPDMA_ChannelCmd(3, DISABLE);					// Disable GPDMA Channel 3 (M2DAC)
}


/**
 * @}
 */

#endif /* _CONFIG */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
