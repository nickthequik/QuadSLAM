
/* Standard includes. */
#include <stdio.h>
#include <limits.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Xilinx includes. */
#include "platform.h"
#include "xparameters.h"
#include "xscutimer.h"
#include "xscugic.h"
#include "xil_exception.h"

#define mainQUEUE_SEND_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define	mainQUEUE_RECEIVE_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define mainQUEUE_SEND_FREQUENCY_MS			( 2000 / portTICK_PERIOD_MS )
#define mainQUEUE_LENGTH					( 1 )
#define mainTASK_LED						( 0 )

extern XScuGic xInterruptController;

static void prvSetupHardware( void );
/*
 * The Xilinx projects use a BSP that do not allow the start up code to be
 * altered easily.  Therefore the vector table used by FreeRTOS is defined in
 * FreeRTOS_asm_vectors.S, which is part of this project.  Switch to use the
 * FreeRTOS vector table.
 */
extern void vPortInstallFreeRTOSVectorTable( void );

void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationTickHook( void );

// FreeRTOS Tasks
static void prvQueueReceiveTask( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );

static QueueHandle_t xQueue;
uint32_t xBlinkRate;

#include "xgpio.h"

#define partstNUM_LEDS			( 1 )
#define partstDIRECTION_OUTPUT	( 1 )
#define partstOUTPUT_ENABLED	( 1 )
#define partstLED_OUTPUT		( 1 )
#define partstLED_INPUT			( 2 )
#define partstGPIO_OUTPUTS      ( 0 )
#define partstGPIO_INPUTS       ( 0xF )

XGpio xGpio;
UBaseType_t led_state = 0;

void vParTestInitialise( void )
{
	XGpio_Config *pxConfigPtr;
	BaseType_t xStatus;

	/* Initialise the GPIO driver. */
	pxConfigPtr = XGpio_LookupConfig(XPAR_AXI_GPIO_0_DEVICE_ID);
	xStatus = XGpio_CfgInitialize( &xGpio, pxConfigPtr, pxConfigPtr->BaseAddress );
	configASSERT( xStatus == XST_SUCCESS );
	( void ) xStatus; /* Remove compiler warning if configASSERT() is not defined. */

	//XGpio_SetDataDirection(&xGpio, partstLED_INPUT, partstGPIO_INPUTS);
	XGpio_SetDataDirection(&xGpio, partstLED_OUTPUT, partstGPIO_OUTPUTS);
	XGpio_DiscreteWrite (&xGpio, partstLED_OUTPUT, 0);
}

void vParTestSetLED(UBaseType_t uxLED, BaseType_t xValue)
{
	uint32_t i;
	UBaseType_t uxMask = 1, uxOnMask = 0, uxOffMask = 0;

	for (i = 0; i < 4; i++)
	{
		if (uxLED & uxMask)
		{
			if (xValue & uxMask)
				uxOnMask |= uxMask;
			else
				uxOffMask |= uxMask;
		}
		uxMask = uxMask << 1;
	}
	XGpio_DiscreteSet(&xGpio, partstLED_OUTPUT, uxOnMask);
	XGpio_DiscreteClear(&xGpio, partstLED_OUTPUT, uxOffMask);
}

void vParTestToggleLED(UBaseType_t uxLED)
{
	uint32_t sw_state = 0xFFFF;

	//sw_state = XGpio_DiscreteRead(&xGpio, partstLED_INPUT);

	led_state ^= uxLED;
	led_state &= sw_state;

	XGpio_DiscreteWrite(&xGpio, partstLED_OUTPUT, led_state);
}

int main( void )
{
	prvSetupHardware();

	/* Create the queue. */
	xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof(uint32_t) );

	if( xQueue != NULL )
	{
		// Start the two tasks as described in the comments at the top of this file.
		xTaskCreate( prvQueueReceiveTask, "Rx", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_RECEIVE_TASK_PRIORITY, NULL );
		xTaskCreate( prvQueueSendTask, "Tx", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK_PRIORITY, NULL );

		// Start the tasks and timer running.
		vTaskStartScheduler();
	}
	for( ;; );
	return 0;
}

static void prvQueueSendTask( void *pvParameters )
{
	TickType_t xNextWakeTime;
	uint32_t ulValueToSend;
	uint32_t blink_rates[5] = {250, 125, 83, 63, 50};
	uint8_t i = 0;

	( void ) pvParameters;
	xNextWakeTime = xTaskGetTickCount();

	for( ;; )
	{
		ulValueToSend = blink_rates[i++];
		xQueueSend( xQueue, &ulValueToSend, 0U );

		if (i == 5)
			i = 0;

		vTaskDelayUntil( &xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS );
	}
}

static void prvQueueReceiveTask( void *pvParameters )
{
	TickType_t xLastWakeTime;
	uint32_t ulReceivedValue;

	( void ) pvParameters;

	vParTestInitialise();
	xLastWakeTime = xTaskGetTickCount();

	while(1)
	{
		xQueueReceive(xQueue, &ulReceivedValue, 0);

		vParTestToggleLED(0xFFFF);
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(ulReceivedValue));
	}
}

static void prvSetupHardware( void )
{
	BaseType_t xStatus;
	XScuGic_Config *pxGICConfig;

	/* Ensure no interrupts execute while the scheduler is in an inconsistent
	state.  Interrupts are automatically enabled when the scheduler is
	started. */
	portDISABLE_INTERRUPTS();

	/* Obtain the configuration of the GIC. */
	pxGICConfig = XScuGic_LookupConfig( XPAR_SCUGIC_SINGLE_DEVICE_ID );

	/* Sanity check the FreeRTOSConfig.h settings are correct for the
	hardware. */
	configASSERT( pxGICConfig );
	configASSERT( pxGICConfig->CpuBaseAddress == ( configINTERRUPT_CONTROLLER_BASE_ADDRESS + configINTERRUPT_CONTROLLER_CPU_INTERFACE_OFFSET ) );
	configASSERT( pxGICConfig->DistBaseAddress == configINTERRUPT_CONTROLLER_BASE_ADDRESS );

	/* Install a default handler for each GIC interrupt. */
	xStatus = XScuGic_CfgInitialize(&xInterruptController, pxGICConfig, pxGICConfig->CpuBaseAddress);
	configASSERT( xStatus == XST_SUCCESS );
	( void ) xStatus; /* Remove compiler warning if configASSERT() is not defined. */

	/* The Xilinx projects use a BSP that do not allow the start up code to be
	altered easily.  Therefore the vector table used by FreeRTOS is defined in
	FreeRTOS_asm_vectors.S, which is part of this project.  Switch to use the
	FreeRTOS vector table. */
	vPortInstallFreeRTOSVectorTable();
}

void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

void vApplicationIdleHook( void )
{
volatile size_t xFreeHeapSpace, xMinimumEverFreeHeapSpace;

	/* This is just a trivial example of an idle hook.  It is called on each
	cycle of the idle task.  It must *NOT* attempt to block.  In this case the
	idle task just queries the amount of FreeRTOS heap that remains.  See the
	memory management section on the http://www.FreeRTOS.org web site for memory
	management options.  If there is a lot of heap memory free then the
	configTOTAL_HEAP_SIZE value in FreeRTOSConfig.h can be reduced to free up
	RAM. */
	xFreeHeapSpace = xPortGetFreeHeapSize();
	xMinimumEverFreeHeapSpace = xPortGetMinimumEverFreeHeapSize();

	/* Remove compiler warning about xFreeHeapSpace being set but never used. */
	( void ) xFreeHeapSpace;
	( void ) xMinimumEverFreeHeapSpace;
}

void vAssertCalled( const char * pcFile, unsigned long ulLine )
{
volatile unsigned long ul = 0;

	( void ) pcFile;
	( void ) ulLine;

	taskENTER_CRITICAL();
	{
		/* Set ul to a non-zero value using the debugger to step out of this
		function. */
		while( ul == 0 )
		{
			portNOP();
		}
	}
	taskEXIT_CRITICAL();
}

void vApplicationTickHook( void )
{
	#if( mainSELECTED_APPLICATION == 1 )
	{
		/* The full demo includes a software timer demo/test that requires
		prodding periodically from the tick interrupt. */
		vTimerPeriodicISRTests();

		/* Call the periodic queue overwrite from ISR demo. */
		vQueueOverwritePeriodicISRDemo();

		/* Call the periodic event group from ISR demo. */
		vPeriodicEventGroupsProcessing();

		/* Use task notifications from an interrupt. */
		xNotifyTaskFromISR();

		/* Use mutexes from interrupts. */
		vInterruptSemaphorePeriodicTest();
	}
	#endif
}

//who the fuck wrote this shit hack below?

void *memcpy( void *pvDest, const void *pvSource, size_t xBytes )
{
/* The compiler used during development seems to err unless these volatiles are
included at -O3 optimisation.  */
volatile unsigned char *pcDest = ( volatile unsigned char * ) pvDest, *pcSource = ( volatile unsigned char * ) pvSource;
size_t x;

	/* Extremely crude standard library implementations in lieu of having a C
	library. */
	if( pvDest != pvSource )
	{
		for( x = 0; x < xBytes; x++ )
		{
			pcDest[ x ] = pcSource[ x ];
		}
	}

	return pvDest;
}

void *memset( void *pvDest, int iValue, size_t xBytes )
{
/* The compiler used during development seems to err unless these volatiles are
included at -O3 optimisation.  */
volatile unsigned char * volatile pcDest = ( volatile unsigned char * volatile ) pvDest;
volatile size_t x;

	/* Extremely crude standard library implementations in lieu of having a C
	library. */
	for( x = 0; x < xBytes; x++ )
	{
		pcDest[ x ] = ( unsigned char ) iValue;
	}

	return pvDest;
}

int memcmp( const void *pvMem1, const void *pvMem2, size_t xBytes )
{
const volatile unsigned char *pucMem1 = pvMem1, *pucMem2 = pvMem2;
volatile size_t x;

	/* Extremely crude standard library implementations in lieu of having a C
	library. */
	for( x = 0; x < xBytes; x++ )
	{
		if( pucMem1[ x ] != pucMem2[ x ] )
		{
			break;
		}
	}

	return xBytes - x;
}

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

	/* Pass out a pointer to the StaticTask_t structure in which the Idle task's
	state will be stored. */
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

	/* Pass out the array that will be used as the Idle task's stack. */
	*ppxIdleTaskStackBuffer = uxIdleTaskStack;

	/* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

	/* Pass out a pointer to the StaticTask_t structure in which the Timer
	task's state will be stored. */
	*ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

	/* Pass out the array that will be used as the Timer task's stack. */
	*ppxTimerTaskStackBuffer = uxTimerTaskStack;

	/* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}


