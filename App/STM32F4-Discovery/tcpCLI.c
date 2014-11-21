#include "tcpCLI.h"


static 	portTickType xDelay = portMAX_DELAY;//3000 / portTICK_RATE_MS;

QueueTelegram telegramS, telegramR; 

/* default telegram */
/*
telegram.data[0] = 1000;
telegram.data[1] = 0;	
telegram.data[2] = 2250;
telegram.data[3] = 10;	
telegram.data[4] = 10;	
telegram.size = 5;
telegram.Qcmd = SETDATA;		
*/



int handleVariable_set (	
				int8_t *pcWriteBuffer, 
				size_t xWriteBufferLen,
				uint8_t * Param, 
				int xParamLength,
				xQueueHandle Qhandle,
				uint8_t * Value,
				int xValueLength,
				int socket )
{


	//================================================================================//
	//		CASE PARAMETER speed [value]
	//================================================================================//

	if ( !strncmp ( Param, "speed", 5))
	{	
	
	
		
		// now convert parameter to proper value and check if it is in range 
        	Value[xValueLength] = '\0'; 	
		uint16_t s1 = atoi ( Value ); 
		// if speed is in range
		if(s1 < 1 ||  s1 > 100 )
		{
			// send error via TCP
			sprintf(pcWriteBuffer, "Error: speed is out of range [1,100]p : %d\n\n\0", s1);
		//	pcWriteBuffer[11+ 3] = "\0"; 
			
			////send( socket_0, buf, 11+3, 0);

	       		return pdFALSE; 	
		}	
	
		// convert to correct value ( * 100 ) 
		s1 *= 100; 
		
		// test i
		telegramS.data[1] = 0;	
		telegramS.data[2] = 2250;
		telegramS.data[3] = 10;	
		telegramS.data[4] = 10;	
		telegramS.size = 5;
		telegramS.Qcmd = SETDATA;	
		// test /
		// setup telegram 
		telegramS.Qcmd = SETDATA;
		telegramS.size = 5;
		telegramS.data[0] = s1;
		
	
		// send value to setSpeed_task via Queue 
		if ( xQueueSend ( QSpd_handle, (void *)&telegramS, xDelay ) == pdPASS )
		{	
					
			if (  xQueueReceive ( QSpd_handle, &telegramR, xDelay)== pdPASS)
			{
				if ( telegramR.Qcmd == SUCCSESS) 
				{	
					sprintf(pcWriteBuffer, "Speed succsesfully set.\n\n");
					xWriteBufferLen = 25; 	
				//	//send( socket, buf, len, 0);


					return pdPASS;

				}
				else
				{
					sprintf(pcWriteBuffer, "MODBUS ERROR !!!.\n\n");
			       		xWriteBufferLen = 19; 	
//					//send( socket, buf, len, 0);

					return pdFALSE;


				}
						
			}
			else
			{
					// send to Queue was unsuccsessful
				// send error via TCP 
		
				sprintf(pcWriteBuffer, "Error recieving response!\n\n");
	 			xWriteBufferLen = 27; 	
				//send( socket_0, buf, len, 0);

				return pdFALSE; 	
			}


		}
		else
		{
			// send to Queue was unsuccsessful
			// send error via TCP 
		
			sprintf(pcWriteBuffer, "Error sending Queue!\n\n");
	 		xWriteBufferLen = 22; 	
			//send( socket, buf, len, 0);

			return pdFALSE; 	
		}
	}
	
	//================================================================================//
	//		CASE PARAMETER upramp [value]
	//================================================================================//

	if ( !strncmp ( Param, "upramp", 6))
	{	
	
	
		
		// now convert parameter to proper value and check if it is in range 
        	Value[xValueLength] = '\0'; 	
		uint16_t s1 = atoi ( Value ); 
		// if speed is in range
		if(s1 < 15 ||  s1 > 300 )
		{
			// send error via TCP
			//
			xWriteBufferLen = 100; 
		//	uint8_t buf [len]; //= "Error: speed is out of range!\n\n");
			sprintf(pcWriteBuffer, "Error: speed is out of range [15,300]p : %d\n\n\0", s1);
			//send( socket_0, buf, len, 0);

	       		return pdFALSE; 	
		}	
	
		telegramS.data[3] = s1; 
		telegramS.size = 5; 
		telegramS.Qcmd = SETDATA;
		
	
		// send value to setSpeed_task via Queue 
		if ( xQueueSend ( QSpd_handle, (void *)&telegramS, xDelay ) == pdPASS )
		{	
					
			if (  xQueueReceive ( QSpd_handle, &telegramR, xDelay))
			{
				if ( telegramR.Qcmd == SUCCSESS) 
				{	
					sprintf(pcWriteBuffer, "Up ramp succsesfully set.\n\n");
					xWriteBufferLen = 27; 	
					//send( socket, buf, len, 0);


					return pdPASS;

				}
				else
				{
					sprintf(pcWriteBuffer, "MODBUS ERROR !!!.\n\n");
			       		xWriteBufferLen = 19; 	
					//send( socket, buf, len, 0);

					return pdFALSE;


				}
						
			}

		}
		else
		{
			// send to Queue was unsuccsessful
			// send error via TCP 
		
			sprintf(pcWriteBuffer, "Error sending Queue!\n\n");
	 		xWriteBufferLen = 22; 	
			//send( socket, buf, len, 0);

			return pdFALSE; 	
		}
	}

	//================================================================================//
	//		CASE PARAMETER downramp [value]
	//================================================================================//

	if ( !strncmp ( Param, "downramp", 8))
	{	
	
	
		
				return 0; 

	}
			
}

int handleVariable_get (	
				int8_t *pcWriteBuffer, 
				size_t xWriteBufferLen,
				uint8_t * Param, 
				int xParamLength,
				xQueueHandle Qhandle,
				int socket )
{


	//================================================================================//
	//		CASE PARAMETER speed [value]
	//================================================================================//

	if ( !strncmp ( Param, "speed", 5))
	{	
	
		telegramS.Qcmd = GETDATA; 
		
	
		// send value to setSpeed_task via Queue 
		if ( xQueueSend ( QSpd_handle, (void *)&telegramS, xDelay ) == pdPASS )
		{	
					
			if (  xQueueReceive ( QSpd_handle, &telegramR, xDelay)== pdPASS)
			{
				if ( telegramR.Qcmd == SUCCSESS) 
				{	
					sprintf(pcWriteBuffer, "Speed is  %2d.%2dp \n\n\0", 
					telegramR.data[3]/100,
					telegramR.data[3] % 100);
				
					//send( socket, buf, len, 0);


					return pdPASS;

				}
				else
				{
					sprintf(pcWriteBuffer, "MODBUS ERROR !!!.\n\n");
			       		xWriteBufferLen = 19; 	
					//send( socket, buf, len, 0);

					return pdFALSE;


				}
						
			}
			else
			{
					// send to Queue was unsuccsessful
				// send error via TCP 
		
				sprintf(pcWriteBuffer, "Error recieving response!\n\n");
	 			xWriteBufferLen = 27; 	
				//send( socket_0, buf, len, 0);

				return pdFALSE; 	
			}


		}
		else
		{
			// send to Queue was unsuccsessful
			// send error via TCP 
		
			sprintf(pcWriteBuffer, "Error sending Queue!\n\n");
	 		xWriteBufferLen = 22; 	
			//send( socket, buf, len, 0);

			return pdFALSE; 	
		}
	}
	
	//================================================================================//
	//		CASE PARAMETER upramp [value]
	//================================================================================//

	if ( !strncmp ( Param, "upramp", 6))
	{	
	
	
		telegramS.Qcmd = GETDATA;
			
	
		// send value to setSpeed_task via Queue 
		if ( xQueueSend ( QSpd_handle, (void *)&telegramS, xDelay ) == pdPASS )
		{	
					
			if (  xQueueReceive ( QSpd_handle, &telegramR, xDelay))
			{
				if ( telegramR.Qcmd == SUCCSESS) 
				{	
					sprintf(pcWriteBuffer, "Up ramp is %d seconds\n\n\0", 
							telegramR.data[3]);
					
					xWriteBufferLen = 25; 	
					//send( socket, buf, len, 0);


					return pdPASS;

				}
				else
				{
					sprintf(pcWriteBuffer, "MODBUS ERROR !!!.\n\n");
			       		xWriteBufferLen = 19; 	
					//send( socket, buf, len, 0);

					return pdFALSE;


				}
						
			}

		}
		else
		{
			// send to Queue was unsuccsessful
			// send error via TCP 
		
			sprintf(pcWriteBuffer, "Error sending Queue!\n\n");
	 		xWriteBufferLen = 22; 	
			//send( socket, buf, len, 0);

			return pdFALSE; 	
		}
	}

	//================================================================================//
	//		CASE PARAMETER downramp [value]
	//================================================================================//

	if ( !strncmp ( Param, "downramp", 8))
	{	
	
	
				
	
	

			return 0; 

	}




}






/* ================================================================================================
 * ================================================================================================
 * 		prvMotorCommand 
 * ================================================================================================
 * ==============================================================================================*/

portBASE_TYPE prvMotorCommand ( 	int8_t *pcWriteBuffer, 
						size_t xWriteBufferLen, 
						const int8_t *pcCommandString)
{
	/* globals: 
	 * 	socket 
	 * 	setSpeedHandle
	 * 	QSpd_Handle
	 * 	QStatus_handle
	 * 	
	 **/

	int8_t *Option, *Param, *Value;
	int xOptionLength, xParamLength, xValueLength; 
	//QueueTelegram telegram; 
	
	// get option from command line 

	Option = FreeRTOS_CLIGetParameter( pcCommandString, // command string 
					  1,  		   // first parameter
				  	  &xOptionLength // parameter string length
					  
					 );

	if ( Option == NULL)
	{
		sprintf(pcWriteBuffer, "To few arguments. \n\n\0");
		return pdFALSE;
	}

	if (!strncmp(Option, "help", 4))
	{
		strcpy(pcWriteBuffer, 
			"Usage and parameters\n"
			"options:\n"
			"\tget - get motor parameter value\n"
			"\tset - set motor parameter value\n"
			"\tstart - starts motor\n"
			"\tstop - stops motor\n"
			"parametrs:\n"
			"\tspeed - sets speed\n"
			"\tupramp\n"
			"\tdownramp\n"

			
			"\0"

		      );

		return pdPASS;
	}


    //	vTaskResume(motorHBHandle);


	//================================================================================//
	//		CASE COMMAND SET [parameter name] [value]
	//================================================================================//

	if( !strncmp( Option, "set", 3) ) 
	{

		// get parameter from command line 
	
		Param = FreeRTOS_CLIGetParameter( pcCommandString, // command string 
					  2,  		   // 2nd parameter
				  	  &xParamLength // parameter string length
					  
					 );
		Value = FreeRTOS_CLIGetParameter( pcCommandString, // command string 
					  3,  		   // 2nd parameter
				  	  &xValueLength // parameter string length
					  
					 );
		
		// return pdFALSE if there is no 3rd parameter 

		if (Value==NULL||Param==NULL)
		{
			strcpy(pcWriteBuffer, xMotorCommand.pcHelpString);

			return pdFALSE; 

		}

		
		if ( handleVariable_set ( 
				           pcWriteBuffer, 
					   xWriteBufferLen,
					   Param, 
					   xParamLength, 
					   QSpd_handle, 
					   Value,
					   xValueLength,  
					   socket_0)) return pdPASS;

		
			
		return pdFALSE;

	}
	//================================================================================//
	//		CASE COMMAND GET [parameter name] [value]
	//================================================================================//


	if (!strncmp ( Option, "get", 3))
	{
		// get parameter from command line 
	
		Param = FreeRTOS_CLIGetParameter( pcCommandString, // command string 
					  2,  		   // 2nd parameter
				  	  &xParamLength // parameter string length
					  
					 );
			
		// return pdFALSE if there is no 3rd parameter 

		if(Value==NULL||Param==NULL)
		{
			strcpy(pcWriteBuffer, xMotorCommand.pcHelpString);

			return pdFALSE; 

		}

		
		if ( handleVariable_get ( 
				        pcWriteBuffer, 
					   xWriteBufferLen,
	
				           Param, 
					   xParamLength, 
					   QSpd_handle, 
					   socket_0)) return pdPASS;
		return pdFALSE;


	}
		
	//================================================================================//
	//		CASE COMMAND STOP 
	//================================================================================//
	if (!strncmp ( Option, "stop", 4))
	{

		telegramS.Qcmd = STOP; 
		telegramS.size = 5; 
			// send value to setSpeed_task via Queue 
		if ( xQueueSend ( QSpd_handle, (void *)&telegramS, xDelay ) )
		{	
					
			if (  xQueueReceive ( QSpd_handle, &telegramR, xDelay)== pdPASS)
			{
				if ( telegramR.Qcmd == SUCCSESS) 
				{	
					sprintf(pcWriteBuffer ,"Motor succsesfully stopped.\n\n");
					xWriteBufferLen = 29 ; 	
					//send( socket_0, buf, len, 0);

					
					return pdPASS;

				}
				else
				{
					sprintf(pcWriteBuffer, "MODBUS ERROR !!!.\n\n");
			       		xWriteBufferLen = 19; 	
					//send( socket_0, buf, len, 0);

					return pdFALSE;


				}
			}
			else
			{
					// send to Queue was unsuccsessful
				// send error via TCP 
		
				sprintf(pcWriteBuffer, "Error recieving response!\n\n");
	 			xWriteBufferLen = 27; 	
				//send( socket_0, buf, len, 0);

				return pdFALSE; 	
			}
		}
		else
		{
			
			// send to Queue was unsuccsessful
			// send error via TCP 
		
			sprintf(pcWriteBuffer, "Error sending Queue!\n\n");
	 		xWriteBufferLen = 22; 	
			//send( socket, buf, len, 0);

			return pdFALSE; 	
		}
					
	}
	//================================================================================//
	//		CASE COMMAND START
	//================================================================================//

	if (!strncmp ( Option, "start", 5))
	{
		telegramS.Qcmd = START; 
		telegramS.size = 5; 
			// send value to setSpeed_task via Queue 
		if ( xQueueSend ( QSpd_handle, (void *)&telegramS, xDelay ) == pdPASS )
		{	
		//	vTaskDelay(500/portTICK_RATE_MS);		
			if (  xQueueReceive ( QSpd_handle, &telegramR, xDelay) == pdPASS)
			{
				if ( telegramR.Qcmd == SUCCSESS) 
				{	
					sprintf(pcWriteBuffer , "Motor succsesfully started.\n\n");
					xWriteBufferLen = 29; 	
					//send( socket_0, buf, len, 0);

					
					return pdPASS;

				}
				else
				{
					sprintf(pcWriteBuffer, "MODBUS ERROR !!!.\n\n");
			       		xWriteBufferLen = 19; 	
					//send( socket_0, buf, len, 0);

					return pdFALSE;


				}
			}
			else
			{
					// send to Queue was unsuccsessful
				// send error via TCP 
		
				sprintf(pcWriteBuffer, "Error recieving response!\n\n");
	 			xWriteBufferLen = 27; 	
				//send( socket_0, buf, len, 0);

				return pdFALSE; 	
			}
		}
		else
		{
			
			// send to Queue was unsuccsessful
			// send error via TCP 
		
			sprintf(pcWriteBuffer,"Error sending Queue!\n\n");
	 		xWriteBufferLen = 22; 	
			//send( socket_0, buf, len, 0);

			return pdFALSE; 	
		}


	}


	strcpy(pcWriteBuffer, xMotorCommand.pcHelpString);
		
	return pdFALSE;


}

portBASE_TYPE prvTaskStatsCommand ( 	int8_t *pcWriteBuffer, 
						size_t xWriteBufferLen, 
						const int8_t *pcCommandString)
{
    /* For simplicity, this function assumes the output buffer is large enough
    to hold all the text generated by executing the vTaskList() API function,
    so the xWriteBufferLen parameter is not used. */
    ( void ) xWriteBufferLen;

    /* pcWriteBuffer is used directly as the vTaskList() parameter, so the table
    generated by executing vTaskList() is written directly into the output
    buffer. */

  
    vTaskList( pcWriteBuffer);

 

    /* The entire table was written directly to the output buffer.  Execution
    of this command is complete, so return pdFALSE. */
    return pdFALSE;
}

