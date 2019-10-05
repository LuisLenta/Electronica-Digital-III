/*
===============================================================================
 Name        : TeoricoTareaClase2.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "core_cm3.h"
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here
#define loadTimer 1000000
typedef enum LED
{
	LEDRojo = 1,
	LEDAzul = 2,
	LEDVerde = 3
} LED_Type;

// TODO: insert other definitions and declarations here
void turnOnLED (LED_Type LED);
void turnOffLED (LED_Type LED);
void initTest (void);

static int btnPress;
int main(void) {

    // TODO: insert code here

	btnPress = 0; // Cantidad de veces que se preciona el boton

	/**
	 * Configuracion de LEDS
	 */
	LPC_GPIO0->FIODIR |= (1<<22); //Seteo el P0.22 como salida para prender LED Rojo
	LPC_GPIO3->FIODIR |= (1<<26); //Seteo P3.26 como salida para prender LED Azul
	LPC_GPIO3->FIODIR |= (1<<25); //Seteo P3.25 como salida para prender LED Verde

	/**
	 * Configuracion de boton
	 */
	/* Configuración del pin2[10] en modo EINT0*/
	LPC_PINCON-> PINSEL4 |= (1<<20);
	LPC_PINCON->PINSEL4 &= ~(1<<21);

	/* Configuración de resistencias internas */
		LPC_PINCON->PINMODE4 &= ~(1 << 20); // POR DEFECTO ES PULL-UP (00)
		LPC_PINCON->PINMODE4 &= ~(1 << 21);

	/**
	 * Configuracion de interrupciones
	 */
	/* Configuración de modo interrupción */
	LPC_SC->EXTMODE |= (1 << 0); // SETEO EL MODO COMO FLANCO EXTMODE[0]

	/* Configuración de tipo de flanco */
	LPC_SC->EXTPOLAR |= (1 << 0); // SETEO COMO FLANCO DE SUBIDA EXTPOLAR[0]

	/* Configuración de prioridades */
	NVIC_SetPriority(EINT0_IRQn, 10);
	//NVIC_SetPriority(SysTick_IRQn,1);  //Prioridad maxima para el Systick

	/* Habilitación de interrupción */
	NVIC_EnableIRQ(EINT0_IRQn);

	SysTick_Config(loadTimer);  //Aca se habilitad la interrupción del Systick y se configura.

	initTest();

	// Enter an infinite loop, just incrementing a counter
    while(1) {
        switch(btnPress){
      /*  case -1: //En caso de error titila el LED Rojo
        	turnOnLED(LEDRojo);
        	while(LPC_GPIO0->FIOPIN & (1 << 22)) {} // Espero que se apague el led Rojo
        	break;*/
        case 2:
        	//Si toco el boton dentro de los 500ms apago los LED.
    		turnOffLED(LEDRojo);
    		turnOffLED(LEDAzul);
    		turnOffLED(LEDVerde);
    		break;
        case 3:
        	turnOnLED(LEDVerde);
        	break;
        case 4:
        	turnOnLED(LEDRojo);
        	break;
        case 5:
        	turnOnLED(LEDAzul);
        	while(LPC_GPIO3->FIOPIN & (1 << 26)) {} // Espero que se apague el led Azul
        	//Cuando se apaga el azul, vuelvo a titilar los LEDs dos veces y termino el programa
        	turnOffLED(LEDRojo);
        	turnOffLED(LEDAzul);
        	turnOffLED(LEDVerde);
        	//Como se que en la interrupcion se apagan todos los LEDs, testeo solamente que uno se alla apagado
        	while(LPC_GPIO0->FIOPIN & (1 << 22)) {} // Espero que se apague el led Rojo
        	//Prendo un vez mas
        	turnOffLED(LEDRojo);
        	turnOffLED(LEDAzul);
        	turnOffLED(LEDVerde);
        	while(LPC_GPIO0->FIOPIN & (1 << 22)) {} // Espero que se apague el led Rojo
        	btnPress++; //Aumento el valor de boton precionado para que no entre mas en ningun lado y termine el programa
        default:
        	break;
        }

    }
    return 0 ;
}
void EINT0_IRQHandler(void) {

	btnPress += 1;
	/*Si es la primera vez que pulsan el boton, enciendo los LEDs*/
	if(btnPress == 1){
		turnOnLED(LEDRojo);
		turnOnLED(LEDAzul);
		turnOnLED(LEDVerde);
	}
	LPC_SC->EXTINT = 1<<0;
}

void SysTick_Handler (void) {
	static int i = 100;
	i--;
	static int a = 500;
	a--;

	switch(btnPress){
	case -1: //Si pasaron los 500ms sigo apagando todos los LEDs, solo que voy a prender solamente el rojo en el
			//while del programa principal.
		if(i==0){
			i=100;
			if(LPC_GPIO0->FIOPIN & (1 << 22))
				turnOffLED(LEDRojo);
			else
				turnOnLED(LEDRojo);
		}
		break;
	case 3: //Tercer pulsado de boton. Verde unico LED prendido
	case 4: //Cuarta pulsada de boton. Rojo unico LED prendido
	case 0: //Primeros 100ms para el testeo de LEDs
		if (i == 0) {
			/*Apaga los LED luego de 100 ms*/
			i = 100;
			a = 500;
			turnOffLED(LEDRojo);
			turnOffLED(LEDAzul);
			turnOffLED(LEDVerde);
		}
		break;
	case 1:
		if(a == 0){
			/*Si se toco una vez el boton y pasaron 500ms, apago los LED*/
			i=100;
			turnOffLED(LEDRojo);
			turnOffLED(LEDAzul);
			turnOffLED(LEDVerde);
			btnPress = -1;
		}
		break;

	case 5: //Quinta pulsada de boton. Azul unico LED prendido.
		if(i == 0){
			i = 50; //Pongo a 50ms, para el parpadeo final de todos los LED.
			turnOffLED(LEDRojo);
			turnOffLED(LEDAzul);
			turnOffLED(LEDVerde);
		}

	}
}

/*Enciende el LED especificado por parametro*/
void turnOnLED (LED_Type LED){

	if(LED == 1)LPC_GPIO0->FIOSET |= (1<<22);      	//LED Rojo
	else if (LED == 2)LPC_GPIO3->FIOSET |= (1<<26);	//LED Azul
	else if(LED == 3)LPC_GPIO3->FIOSET |= (1<<25);	//LED Verde

}
/*Apaga el LED especificado por parametro*/
void turnOffLED (LED_Type LED){

		if(LED == 1)LPC_GPIO0->FIOCLR = (1<<22);      	//LED Rojo
		else if (LED == 2)LPC_GPIO3->FIOCLR = (1<<26);	//LED Azul
		else if(LED == 3)LPC_GPIO3->FIOCLR = (1<<25);	//LED Verde
}
/* Rutina de testeo de LEDs al inicio del programa*/
void initTest(void) {

		turnOnLED(LEDRojo);
		while(LPC_GPIO0->FIOPIN & (1 << 22)) {} // Espero que se apague el led Rojo
		turnOnLED(LEDAzul);
		while(LPC_GPIO3->FIOPIN & (1 << 26)) {} // Espero que se apague el led Azul
		turnOnLED(LEDVerde);
		while(LPC_GPIO3->FIOPIN & (1 << 25)) {} // Espero que se apague el led Verde
}

