/*
===============================================================================
 Name        : TeoricoClase9_DMADAC.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_dac.h"

#define RAMPA 				100
#define EXTRA				42
#define NUMERO_MUESTRAS		142
#define FRECUENCIA 			100
#define CLOCK_DAC_MHZ 		25  	//CLK divided by 4

void init (void);
void configDAC(void);
void configGPDMA(void);
void configPINSEL(void);

void crearLista (void);

int main(void) {

    // TODO: insert code here

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}
