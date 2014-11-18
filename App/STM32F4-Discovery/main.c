/*
    FreeRTOS V7.5.3 - Copyright (C) 2013 Real Time Engineers Ltd.
    All rights reserved
 */

/*!	\file main.c
 * 	\brief Main file with main functions for MOTOR TCP CLI application 
 */


// includes 

#include "main.h"
#include "spi.h"
#include "W5200.h"
#include "modbus_mk.h"
#include "tcpCLI.h"



///* Priorities for the demo application tasks. */
#define mainFLASH_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )
#define mainQUEUE_POLL_PRIORITY				( tskIDLE_PRIORITY + 2UL )
#define mainSEM_TEST_PRIORITY				( tskIDLE_PRIORITY + 1UL )
#define mainBLOCK_Q_PRIORITY				( tskIDLE_PRIORITY + 2UL )
#define mainCREATOR_TASK_PRIORITY			( tskIDLE_PRIORITY + 3UL )
#define mainFLOP_TASK_PRIORITY				( tskIDLE_PRIORITY )


/* The period after which the check timer will expire, in ms, provided no errors
have been reported by any of the standard demo tasks.  ms are converted to the
equivalent in ticks using the portTICK_RATE_MS constant. */
#define mainCHECK_TIMER_PERIOD_MS			( 3000UL / portTICK_RATE_MS )

/* The period at which the check timer will expire, in ms, if an error has been
reported in one of the standard demo tasks.  ms are converted to the equivalent
in ticks using the portTICK_RATE_MS constant. */
#define mainERROR_CHECK_TIMER_PERIOD_MS 	( 200UL / portTICK_RATE_MS )

/* Set mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY to 1 to create a simple demo.
Set mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY to 0 to create a much more
comprehensive test application.  See the comments at the top of this file, and
the documentation page on the http://www.FreeRTOS.org web site for more
information. */
#define mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY		0

/*-----------------------------------------------------------*/

/** setup hardware for system */ 
static void prvSetupHardware( void );

/*
 * The check timer callback function, as described at the top of this file.
 */
static void prvCheckTimerCallback( xTimerHandle xTimer );

/*
 * Configure the interrupts used to test the interrupt nesting depth as
 * described at the top of this file.
 */
static void prvSetupNestedFPUInterruptsTest( void );

/*
 * Register check tasks, and the tasks used to write over and check the contents
 * of the FPU registers, as described at the top of this file.  The nature of
 * these files necessitates that they are written in an assembly file.
 */
extern void vRegTest1Task( void *pvParameters );
extern void vRegTest2Task( void *pvParameters );
extern void vRegTestClearFlopRegistersToParameterValue( unsigned long ulValue );
extern unsigned long ulRegTestCheckFlopRegistersContainParameterValue( unsigned long ulValue );

/*
 * The task that is synchronised with the button interrupt.  This is done just
 * to demonstrate how to write interrupt service routines, and how to
 * synchronise a task with an interrupt.
 */
static void prvButtonTestTask( void *pvParameters );

/*
 * This file can be used to create either a simple LED flasher example, or a
 * comprehensive test/demo application - depending on the setting of the
 * mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY constant defined above.  If
 * mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY is set to 1, then the following
 * function will create a lot of additional tasks and a software timer.  If
 * mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY is set to 0, then the following
 * function will do nothing.
 */
static void prvOptionallyCreateComprehensveTestApplication( void );

/*-----------------------------------------------------------*/

/* The following two variables are used to communicate the status of the
register check tasks to the check software timer.  If the variables keep
incrementing, then the register check tasks has not discovered any errors.  If
a variable stops incrementing, then an error has been found. */
volatile unsigned long ulRegTest1LoopCounter = 0UL, ulRegTest2LoopCounter = 0UL;

/* The following variables are used to verify that the interrupt nesting depth
is as intended.  ulFPUInterruptNesting is incremented on entry to an interrupt
that uses the FPU, and decremented on exit of the same interrupt.
ulMaxFPUInterruptNesting latches the highest value reached by
ulFPUInterruptNesting.  These variables have no other purpose. */
volatile unsigned long ulFPUInterruptNesting = 0UL, ulMaxFPUInterruptNesting = 0UL;
/* The semaphore used to demonstrate a task being synchronised with an
interrupt. */
static xSemaphoreHandle xTestSemaphore = NULL;



/*-----------------------------------------------------------*/

int main(void)
{
	/*<! Configure the hardware ready to run the test. */
	prvSetupHardware();;
	
	// ============now register CLI commands ===================
	
	FreeRTOS_CLIRegisterCommand( &xMotorCommand );
       	FreeRTOS_CLIRegisterCommand( &xTaskStatsCommand);		

	// =============== register CLI commands ===================
	int sr = sizeof(QueueTelegram);
	sr = sizeof (QueueTelegram *);
	// create queues 
	QSpd_handle = xQueueCreate(2, sizeof(QueueTelegram));


/*------------------added by Matic Knap 24 Jun 2014 ---------------------------------*/

	// echo server task 
	xTaskCreate(set_macTask, "TCPsrv", configMINIMAL_STACK_SIZE*14, 
			NULL, mainFLASH_TASK_PRIORITY , &set_macTaskHandle);
	
	// run motor task 
	xTaskCreate(motorControl_task, "motor", configMINIMAL_STACK_SIZE*19,
		       	NULL, mainFLASH_TASK_PRIORITY, &motorHBHandle);

	// set motor task 
	xTaskCreate(motorHeartBeat_task, "motorHB", configMINIMAL_STACK_SIZE*5,		       				
			NULL, mainFLASH_TASK_PRIORITY , &motorHeartBeatHandle);
	

/*------------------added by Matic Knap 24 Jun 2014 ---------------------------------*/


	/* The following function will only create more tasks and timers if
	mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY is set to 0 (at the top of this
	file).  See the comments at the top of this file for more information. */
	//prvOptionallyCreateComprehensveTestApplication();

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for( ;; );
}


/*! 	\fn static void prvSetupHardware(void) 
 *	\brief Sets up hardware
 * 	
 */
void prvSetupHardware( void )
{
	/* Setup STM32 system (clock, PLL and Flash configuration) */
	SystemInit();

	/* Ensure all priority bits are assigned as preemption priority bits. */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	//init_SPIx();
	init_SPIx();	
	
	// init USARTx 
	init_USARTx();

	/*
	 * Create task - initialize Wiznet
	 * It creates task for Wiznet initialization and at end delete it. 
	 * Function : 		init_W5200 from W5200.c file 
	 * Stack size :		5 times minimial stack size 
	 * Task priority :	main flash task priority + 1 
	 * Parameters 	 :	no parameters (NULL)
	 */  
	xTaskCreate(init_W5200, "init_W5200", configMINIMAL_STACK_SIZE*5, 
			NULL, mainFLASH_TASK_PRIORITY + 1 , NULL);
	
	
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	#if ( mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY == 0 )
	{
		/* Just to verify that the interrupt nesting behaves as expected,
		increment ulFPUInterruptNesting on entry, and decrement it on exit. */
		ulFPUInterruptNesting++;

		/* Fill the FPU registers with 0. */
		//vRegTestClearFlopRegistersToParameterValue( 0UL );

		/* Trigger a timer 2 interrupt, which will fill the registers with a
		different value and itself trigger a timer 3 interrupt.  Note that the
		timers are not actually used.  The timer 2 and 3 interrupt vectors are
		just used for convenience. */
		NVIC_SetPendingIRQ( TIM2_IRQn );

		/* Ensure that, after returning from the nested interrupts, all the FPU
		registers contain the value to which they were set by the tick hook
		function. */
		//configASSERT( ulRegTestCheckFlopRegistersContainParameterValue( 0UL ) );

		ulFPUInterruptNesting--;
	}
	#endif
}
/*-----------------------------------------------------------*/

static void prvSetupNestedFPUInterruptsTest( void )
{
NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the TIM2 interrupt in the NVIC.  The timer itself is not used,
	just its interrupt vector to force nesting from software.  TIM2 must have
	a lower priority than TIM3, and both must have priorities above
	configMAX_SYSCALL_INTERRUPT_PRIORITY. */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY - 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );

	/* Enable the TIM3 interrupt in the NVIC.  The timer itself is not used,
	just its interrupt vector to force nesting from software.  TIM2 must have
	a lower priority than TIM3, and both must have priorities above
	configMAX_SYSCALL_INTERRUPT_PRIORITY. */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY - 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );
}
/*-----------------------------------------------------------*/

void TIM3_IRQHandler( void )
{
	/* Just to verify that the interrupt nesting behaves as expected, increment
	ulFPUInterruptNesting on entry, and decrement it on exit. */
	ulFPUInterruptNesting++;

	/* This is the highest priority interrupt in the chain of forced nesting
	interrupts, so latch the maximum value reached by ulFPUInterruptNesting.
	This is done purely to allow verification that the nesting depth reaches
	that intended. */
	if( ulFPUInterruptNesting > ulMaxFPUInterruptNesting )
	{
		ulMaxFPUInterruptNesting = ulFPUInterruptNesting;
	}

	/* Fill the FPU registers with 99 to overwrite the values written by
	TIM2_IRQHandler(). */
	//vRegTestClearFlopRegistersToParameterValue( 99UL );

	ulFPUInterruptNesting--;
}
/*-----------------------------------------------------------*/

void TIM2_IRQHandler( void )
{
	/* Just to verify that the interrupt nesting behaves as expected, increment
	ulFPUInterruptNesting on entry, and decrement it on exit. */
	ulFPUInterruptNesting++;

	/* Fill the FPU registers with 1. */
	//vRegTestClearFlopRegistersToParameterValue( 1UL );

	/* Trigger a timer 3 interrupt, which will fill the registers with a
	different value. */
	NVIC_SetPendingIRQ( TIM3_IRQn );

	/* Ensure that, after returning from the nesting interrupt, all the FPU
	registers contain the value to which they were set by this interrupt
	function. */
	//configASSERT( ulRegTestCheckFlopRegistersContainParameterValue( 1UL ) );

	ulFPUInterruptNesting--;
}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	unction, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
///*-----------------------------------------------------------*/
void assert_failed(uint8_t* file, uint32_t line){

}
