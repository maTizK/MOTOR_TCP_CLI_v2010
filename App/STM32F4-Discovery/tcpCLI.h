
#ifndef _TCP_CLI_H_
#define _TCP_CLI_H_

#include "main.h"
#include "FreeRTOS_CLI.h"
#include "W5200.h"
// prototypes of CLI functions 


// motor commands with 1 parameter 
portBASE_TYPE prvMotorCommand ( 	int8_t *pcWriteBuffer, 
						size_t xWriteBufferLen, 
						const int8_t *pcCommandString);


// set speed comand definition 1 parameter 
static const CLI_Command_Definition_t xMotorCommand = 
{
	"motor",
	"motor  [options] [parameters] : Controls motor speed, rpm, etc., .. \r\n",
	prvMotorCommand,
	-1
};

/*!	\enum QueueCommand 
 * 	\brief Queue telegram command
 *
 * 	Queue telegram command for tasks commmunication
 */
typedef enum
{
	SETDATA,	/**< enum DATA if sending data. */ 
	GETDATA, //!< enum GETDATA if getting data */ 
	IDLE,	/**< enum IDLE if task has to go to idle mode. */ 
	DELETE,	/**< enum DELETE if task has to delete itself. */
	START, //!< enum START if task has to start motor
	STOP,  //!< enum STOP if task has to stop motor 
	SUCCSESS, //!< enum SUCCSESS if command has been succssessfully receieved
	ERROR_MODBUS //!< enum ERROR if error has occured
		
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
	uint16_t data[10]; /**< data of telegram */ 

}QueueTelegram; 

#endif
