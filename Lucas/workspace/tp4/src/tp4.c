#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

/* Library includes. */
#include "stdlib.h"
#include "uart.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


//---------------------------------------------------------------------------
#define mainPASS_STATUS_MESSAGE "All tasks are executing without error."

#define STACK_SIZE	((unsigned short)80)

typedef struct
{
	 uint8_t temperature;
	 char *data;
} Data_t;

const char *name = "int_ext"; //usage in trace
static char *pcStatusMessage = mainPASS_STATUS_MESSAGE;

// Declare Semaphore, Queue and trace for INT
QueueHandle_t xQueue;
SemaphoreHandle_t xSemaphore;
QueueSetMemberHandle_t xActivatedMember;
traceHandle eint3Handle;

//This task, generates a random numeber simulating a temperature sensor. And send it to queue
void task_sensor (void *pvParameters) {

	portTickType xNextWakeTime;
	uint8_t temperature;

	xNextWakeTime = xTaskGetTickCount (); //tiempo en el que la tarea dejo el estado bloqueado por ultima vez
	//se utiliza para calcular el tiempo que la tarea deberia dejar el estado bloqueado.

	for (;;) {
		//segundo parametro, es el tiempo del periodo
		vTaskDelayUntil (&xNextWakeTime, 500 / portTICK_PERIOD_MS); //the exact tick count value at which the
		//calling task should be moved from the Blocked state into the Ready state.
		//Hecho para que no se ejecute solo esta tarea, es decir, permite que otras tareas con
		//menor prioridad se ejecuten.
		//cuando el delay finaliza, el planificador agarra esta tarea, si es la de mayor prioridad
		//donde desea que su tarea se ejecute periódicamente con una frecuencia fija

		temperature = (rand () % 80);	// returns a pseudo-random integer between 0 and RAND_MAX
		Data_t temp_data = {temperature, NULL};
		xQueueSend (xQueue, &temp_data, 0); //mete datos al final de la cola. El 0
		//establece la cantidad de tiempo maximo que la tarea permanece en el estado bloqueado.
		//desbloqueara otra tarea si los datos en la cola no estan listos en ese tiempo
	}
}

//active it from the external interrupt. Send random String to queue.
void task_data (void *pvParameters) {

	Data_t data;

	for (;;) {
		//The Task takes the semaphore and waits to be free it in the external interrupt (button)
		xSemaphoreTake (xSemaphore, portMAX_DELAY);

		switch (rand() % 4) {
			case 0:
				data.data = "abc";
				break;
			case 1:
				data.data = "abcdef";
				break;
			case 2:
				data.data = "abcdefghi";
				break;
			case 3:
				data.data = "abcdefghijkl";
				break;
			default:
				data.data = "abcdefghijklmno";
				break;
			}

		xQueueSendToBack (xQueue, &data, 0); //mete datos al final de la cola. El 0
		//establece la cantidad de tiempo maximo que la tarea permanece en el estado bloqueado
		//desbloqueara otra tarea si los datos en la cola no estan listos en ese tiempo


	}
}

//Receive data from queue and send it to UART
void task_send (void *pvParameters) {

	Data_t receivedValue;
	BaseType_t xStatus;

	//Para poder seleccionar la siguiente tarea para ejecutar, el planificador mismo debe ejecutar al
	//final de cada segmento de tiempo. Una interrupción periódica, llamada 'tick interrupt'

	const TickType_t xTimeInTicks = pdMS_TO_TICKS(1000);//converts a time specified 
	//in milliseconds into a time specified in ticks.


	//The ‘tick count’ value is the total number of tick interrupts that have occurred 
	//since the scheduler was started.

	for (;;) {

		xStatus = xQueueReceive (xQueue, &receivedValue, xTimeInTicks);

		if (xStatus == pdPASS) {
			if (receivedValue.data == NULL)
				//sendByte (receivedValue.temperature);
				UART3_Printf("Temperatura: %d \n\r",(int)receivedValue.temperature);
			else
				//sendString (receivedValue.data);
				UART3_Printf("%s \n\r",receivedValue.data);
		}
	}
}

void Config (void) {

	/* configure UART module*/
	// P0.0 as TX and P0.1 as RX
	UART3_Init(9600);  /* Initialize UART at 9600 baud rate */
	LPC_UART3 -> IER = 1;

	NVIC_SetPriority (21, 5); //priority 5 for external interrupt (button)
	LPC_GPIOINT -> IO2IntEnF = (1 << 10); //falling edge
	NVIC_EnableIRQ (21);	//enable interrupt
}

// Extern interrupt Handler
void EINT3_IRQHandler (void) {

	vTraceStoreISRBegin (eint3Handle);//comienza trace

	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;	//No realizar cambio de contexto dentro de la interrupcion
	LPC_GPIOINT -> IO2IntClr |= (1 << 10); //lowered the flag
	xSemaphoreGiveFromISR (xSemaphore, &xHigherPriorityTaskWoken); //Give a semaphore. Si sale en 1, hay una tarea
	//esperando el semaforo. Ejecutara la de la prioridad mas alta
	portYIELD_FROM_ISR (xHigherPriorityTaskWoken); //si xHigherPriorityTaskWoken es falso, entonces no hay cambio
	//de contexto, es decir, no hay cambio de tarea

	vTraceStoreISREnd (xHigherPriorityTaskWoken);//finaliza trace
}

/*-----------------------------------------------------------*/

void vApplicationTickHook (void) {
	return;
}

/*-----------------------------------------------------------*/

char *pcGetTaskStatusMessage (void) {
	/* Not bothered about a critical section here. */
	return pcStatusMessage;
}

/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook (TaskHandle_t pxTask, char *pcTaskName) {
	/* This function will get called if a task overflows its stack. */
	(void) pxTask;
	(void) pcTaskName;

	for (;;);
}

void vConfigureTimerForRunTimeStats (void) {

	const unsigned long TCR_COUNT_RESET = 2, CTCR_CTM_TIMER = 0x00,
						TCR_COUNT_ENABLE = 0x01;

	/* This function configures a timer that is used as the time base when
	collecting run time statistical information - basically the percentage
	of CPU time that each task is utilising.  It is called automatically when
	the scheduler is started (assuming configGENERATE_RUN_TIME_STATS is set
	to 1). */

	/* Power up and feed the timer. */
	LPC_SC -> PCONP |= 0x02UL;
	LPC_SC -> PCLKSEL0 = (LPC_SC -> PCLKSEL0 & (~(0x3 << 2))) | (0x01 << 2);

	/* Reset Timer 0 */
	LPC_TIM0 -> TCR = TCR_COUNT_RESET;

	/* Just count up. */
	LPC_TIM0 -> CTCR = CTCR_CTM_TIMER;

	/* Prescale to a frequency that is good enough to get a decent resolution,
	but not too fast so as to overflow all the time. */
	LPC_TIM0 -> PR = (configCPU_CLOCK_HZ / 10000UL) - 1UL;

	/* Start the counter. */
	LPC_TIM0 -> TCR = TCR_COUNT_ENABLE;
}


int main (void) {

	Config();
	vTraceEnable (TRC_INIT);

	//init queue, semaphore and trace handle
	xQueue = xQueueCreate (8, sizeof (Data_t)); //cantidad de elementos y el tamaño de cada elemento
	xSemaphore = xSemaphoreCreateBinary ();
	eint3Handle = xTraceSetISRProperties(name, 5);

	/* Crear las tareas
		parametros
	 * 1-puntero a la funcion de la tarea
	 * 2-nombre que solo sirve para que el humano la identifique
	 * 3-tamaño de la pila de cada tarea. El valor especifica el número de palabras que puede contener la pila,
	 * no el numero de bytes. ej:si el ancho de la pila es de 4bytes:  100 * 4 = 400bytes
	 * 4-valor que se pasa a la tarea.
	 * 5-Define la prioridad a la que se ejecutará la tarea. Las prioridades pueden ser asignado de 0,
	 * que es la prioridad más baja, a (configMAX_PRIORITIES - 1), que es la prioridad más alta.
	 * El valor de la prioridad se puede cambiar luego de crear la tarea con vTaskPrioritySet ().
	 * 6-identificador de tarea
	 *
	 * retorna FAIL si la memoria de la pila en ram es insuficiente.

	 *una tarea se puede eliminar con vTaskDelete().
	 */
    xTaskCreate (task_sensor, "temperature", STACK_SIZE, (void *) NULL, 3, NULL);
    xTaskCreate (task_data, "Data_string", STACK_SIZE, (void *) NULL, 2, NULL);
    xTaskCreate (task_send, "Send_UART", STACK_SIZE, (void *) NULL, 1, NULL);

	vTraceEnable (TRC_START);

    /* Start the scheduler so the tasks start executing. */
	vTaskStartScheduler(); //planificador
	//el scheduler toma siempre la tarea de mayor prioridad

	/* Will only get here if there was insufficient memory to create the idle
    task.  The idle task is created within vTaskStartScheduler(). */
	for (;;);
}
