/*
===============================================================================
 Name        : PracticoClase6_ADC.c
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

// TODO: insert other include files here
#include "lpc17xx_adc.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"
// TODO: insert other definitions and declarations here
#define INPUT		(uint32_t) 0
#define OUTPUT		(uint32_t) 1

void init(void);
void config_GPIO(void);
void config_TMR0(void);
void config_ADC(void);
void config_DAC(void);
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
