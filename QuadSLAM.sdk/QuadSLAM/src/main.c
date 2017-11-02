
/* Standard includes. */
#include <stdio.h>
#include <limits.h>
#include <platform.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Xilinx includes. */
#include "xparameters.h"
#include "xscutimer.h"
#include "xscugic.h"
#include "xil_exception.h"

#include "xaxivdma.h"
#include "xvtc.h"
#include "vtc.h"
#include "vdma.h"
#include "xdebug.h"
#include "xil_cache_l.h"
#include "xpseudo_asm_gcc.h"
#include "xgpio.h"
#include "xvprocss.h"

#include "uart.h"
#include "led.h"
#include "stereo_camera.h"
#include "vprocsub.h"

extern XScuGic xInterruptController;
extern void vPortInstallFreeRTOSVectorTable( void );

extern XGpio xGpio1, xGpio2;
extern XVprocSs vprocss;

static void prvSetupHardware( void );
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationTickHook( void );

// FreeRTOS Tasks
static void init_task( void *parameters );

int main( void )
{
	prvSetupHardware();

	xTaskCreate( init_task, "init_task", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
	vTaskStartScheduler();

	for( ;; );
	return 0;
}

static void init_task(void *parameters)
{
	int status;

	uint32_t vid_locked, clk_locked;

	UART_usb_init();
	LED_init();
	VDMA_init();
	VPSS_init();
	VTC_init();
	STEREO_CAMERA_init();


	while(1)
	{
		clk_locked = XGpio_DiscreteRead(&xGpio1, 1);
		vid_locked = XGpio_DiscreteRead(&xGpio1, 2);

		if (clk_locked)
			LED_set(0, LED_ON);
		else
			LED_set(0, LED_OFF);

		if (vid_locked)
			LED_set(1, LED_ON);
		else
			LED_set(1, LED_OFF);

		status = XGpio_DiscreteRead(&xGpio2, 1);

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


