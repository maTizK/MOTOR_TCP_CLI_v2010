/*! \file main.h
 *  \brief A main header file.
*/



/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* FreeRTOS includes */ 
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
/* Pheripals includes */ 

#include "stm32f4xx.h" 
#include "stm32f4xx_conf.h"
#include <stm32f4xx.h>
#include <stm32f4xx_usart.h>
#include <stm32f4xx_spi.h>
#include <stm32f4xx_dma.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>

/*! 	\var xSemaphoreHandle xSemaphoreDMASPI
 * 	\brief Semaphore handle for DMA SPI pheriphal
 */

xSemaphoreHandle xSemaphoreDMASPI;

/*!	\var static unsigned portBASE_TYPE xHigherPriorityTaskWoken
 * 	\brief Indicates if higher priority has been woken
 */

static unsigned portBASE_TYPE xHigherPriorityTaskWoken;

/*!	\var xSemaphoreHandle xSmphrUSART; 
	\brief Sempahore handle for USART port 
*/

xSemaphoreHandle xSmphrUSART; 

/*!	\var xTaskHandle setSpeedHandle
* 	\brief Task handles for setspeed
*/
xTaskHandle setSpeedHandle,

	    
/*!	\var xTaskHandle motorHBHandle
	\brief Task handles for motor heart bit tasks

*/

	    	 motorHBHandle;



/*!	\var xQueueHandle QSpd_handle	\brief Queue handles
*/


xQueueHandle QSpd_handle, 
	     
/*!	\var xQueueHandle QStatus_handle
	\brief Queue handles
*/
	     
	     
	     QStatus_handle;

/*!	\var int socket_0
 * 	\brief Socket 0 descriptor
 */	      
int socket_0;

/*!	\enum QueueCommand 
 * 	\brief Queue telegram command
 *
 * 	Queue telegram command for tasks commmunication
 */
typedef enum
{
	DATA,	/**< enum DATA if sending data. */ 
	IDLE,	/**< enum IDLE if task has to go to idle mode. */ 
	DELETE	/**< enum DELETE if task has to delete itself. */

} QueueCommand;

/*!	\struct QueueTelegram struct
 * 	\brief Telegram communication betwen task
 *
 * 	Telegram communication between tasks in specific format 
 */
typedef struct
{
	QueueCommand Qcmd; /**< QueueCommand what type of telegram we received */
	size_t size; 	   /**< size of data transmited */ 
	uint8_t data[100]; /**< data of telegram */ 

}QueueTelegram; 
