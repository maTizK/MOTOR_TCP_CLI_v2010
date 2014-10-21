#include "main.h"
#include "FreeRTOS_CLI.h"
#include "W5200.h"

// prototypes of CLI functions 


// motor commands with 1 parameter 
portBASE_TYPE prvMotorCommand1 ( 	int8_t *pcWriteBuffer, 
						size_t xWriteBufferLen, 
						const int8_t *pcCommandString);

// motor commands with 2 parameters 
portBASE_TYPE prvMotorCommand2 ( 	int8_t *pcWriteBuffer, 
						size_t xWriteBufferLen, 
						const int8_t *pcCommandString);

// set speed comand definition 1 parameter 
static const CLI_Command_Definition_t xMotorCommand1 = 
{
	"motor",
	"motor  [options] [parameters] : Controls motor speed, rpm, etc., .. \r\n",
	prvMotorCommand1,
	1
};

// set speed comand definition 2 parameters 
static const CLI_Command_Definition_t xMotorCommand2 = 
{
	"motor",
	"motor  [options] [parameters] : Controls motor speed, rpm, etc., .. \r\n",
	prvMotorCommand2,
	1
};
