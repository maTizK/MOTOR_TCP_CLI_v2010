#include "tcpCLI.h"


static 	portTickType xDelay = 100 / portTICK_RATE_MS;






int handleVariable_set (	QueueTelegram telegram,
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
        	Value[xValueLength-2] = '\0'; 	
		uint16_t s1 = atoi ( Value ); 
		// if speed is in range
		if(s1 < 10 ||  s1 > 100 )
		{
			// send error via TCP
			sprintf(pcWriteBuffer, "Error: speed is out of range [1,100]p : %d\n\n\0", s1);
		//	pcWriteBuffer[11+ 3] = "\0"; 
			
			////send( socket_0, buf, 11+3, 0);

	       		return pdFALSE; 	
		}	
	
		// convert to correct value ( * 100 ) 
		s1 *= 100; 
		
		
		// setup telegram 
		
		telegram.data[0] = s1;
		telegram.data[1] = 0;	
		telegram.data[2] = 2250;
		telegram.data[3] = 10;	
		telegram.data[4] = 10;	
		telegram.size = 5;
		telegram.Qcmd = SETDATA;
		
	
		// send value to setSpeed_task via Queue 
		if ( xQueueSend ( QSpd_handle, (void *)&telegram, xDelay ) == pdPASS )
		{	
					
			if (  xQueueReceive ( QSpd_handle, &telegram, xDelay)== pdPASS)
			{
				if ( telegram.Qcmd = SUCCSESS) 
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
        	Value[xValueLength-2] = '\0'; 	
		uint16_t s1 = atoi ( Value ); 
		// if speed is in range
		if(s1 < 10 ||  s1 > 100 )
		{
			// send error via TCP
			//
			xWriteBufferLen = 100; 
		//	uint8_t buf [len]; //= "Error: speed is out of range!\n\n");
			sprintf(pcWriteBuffer, "Error: speed is out of range [1,100]p : %d\n\n\0", s1);
			//send( socket_0, buf, len, 0);

	       		return pdFALSE; 	
		}	
	
		// convert to correct value ( * 100 ) 
		s1 *= 100; 
		
		
		// setup telegram 
		
		telegram.data[0] = s1; 
		telegram.size = 1; 
		telegram.Qcmd = SETDATA;
		
	
		// send value to setSpeed_task via Queue 
		if ( xQueueSend ( QSpd_handle, (void *)&telegram, xDelay ) == pdPASS )
		{	
					
			if (  xQueueReceive ( QSpd_handle, &telegram, xDelay))
			{
				if ( telegram.Qcmd = SUCCSESS) 
				{	
					sprintf(pcWriteBuffer, "Speed succsesfully set.\n\n");
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
	
	
		
		// now convert parameter to proper value and check if it is in range 
        	Value[xValueLength-2] = '\0'; 	
		uint16_t s1 = atoi ( Value ); 
		// if speed is in range
		if(s1 < 10 ||  s1 > 100 )
		{
			// send error via TCP
			//
			xWriteBufferLen = strlen ( Value ) + 11 ; 
			//uint8_t buf [len]; //= "Error: speed is out of range!\n\n");
			sprintf(pcWriteBuffer, "Errror : %d\n\n", s1);
			pcWriteBuffer[11+ 3] = "\0"; 
			//send( socket_0, buf, len, 0);

	       		return pdFALSE; 	
		}	
	
		// convert to correct value ( * 100 ) 
		s1 *= 100; 
		
		
		// setup telegram 
		
		telegram.data[0] = s1; 
		telegram.size = 1; 
		telegram.Qcmd = SETDATA;
		
	
		// send value to setSpeed_task via Queue 
		if ( xQueueSend ( QSpd_handle, (void *)&telegram, xDelay ) == pdPASS )
		{	
					
			if (  xQueueReceive ( QSpd_handle, &telegram, xDelay))
			{
				if ( telegram.Qcmd = SUCCSESS) 
				{	
					sprintf(pcWriteBuffer, "Speed succsesfully set.\n\n");
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

	strcpy(pcWriteBuffer, xMotorCommand.pcHelpString);

			return 0; 

}
			


int handleVariable_get (	QueueTelegram telegram,
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
	
		telegram.Qcmd = GETDATA; 
		
	
		// send value to setSpeed_task via Queue 
		if ( xQueueSend ( QSpd_handle, (void *)&telegram, xDelay ) == pdPASS )
		{	
					
			if (  xQueueReceive ( QSpd_handle, &telegram, xDelay)== pdPASS)
			{
				if ( telegram.Qcmd = SUCCSESS) 
				{	
					sprintf(pcWriteBuffer, "Speed is  %2d.%2dp \n\n\0", 
					telegram.data[3]/100,
					telegram.data[3] % 100);
				
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
	
	
		
			
	
		// send value to setSpeed_task via Queue 
		if ( xQueueSend ( QSpd_handle, (void *)&telegram, xDelay ) == pdPASS )
		{	
					
			if (  xQueueReceive ( QSpd_handle, &telegram, xDelay))
			{
				if ( telegram.Qcmd = SUCCSESS) 
				{	
					sprintf(pcWriteBuffer, "Speed succsesfully set.\n\n");
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
	
	
				
	
		// send value to setSpeed_task via Queue 
		if ( xQueueSend ( QSpd_handle, (void *)&telegram, xDelay ) == pdPASS )
		{	
					
			if (  xQueueReceive ( QSpd_handle, &telegram, xDelay))
			{
				if ( telegram.Qcmd = SUCCSESS) 
				{	
					sprintf(pcWriteBuffer, "Speed succsesfully set.\n\n");
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


	strcpy(pcWriteBuffer, xMotorCommand.pcHelpString);


			return 0; 

}











// prototypes of CLI functions 

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
	QueueTelegram telegram; 
	
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

		if (Value == NULL)
		{
			strcpy(pcWriteBuffer, xMotorCommand.pcHelpString);

			return pdFALSE; 

		}
		
		if ( handleVariable_set ( telegram, 
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

		if (Param == NULL) return pdFALSE; 

		
		if ( handleVariable_get ( telegram,
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

		telegram.Qcmd = STOP; 
		telegram.size = 0; 
			// send value to setSpeed_task via Queue 
		if ( xQueueSend ( QSpd_handle, (void *)&telegram, xDelay ) )
		{	
					
			if (  xQueueReceive ( QSpd_handle, &telegram, xDelay))
			{
				if ( telegram.Qcmd = SUCCSESS) 
				{	
					sprintf(pcWriteBuffer ,"Motor succsesfully stopped.\n\n");
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
		telegram.Qcmd = START; 
		telegram.size = 0; 
			// send value to setSpeed_task via Queue 
		if ( xQueueSend ( QSpd_handle, (void *)&telegram, xDelay ) == pdPASS )
		{	
					
			if (  xQueueReceive ( QSpd_handle, &telegram, xDelay) == pdPASS)
			{
				if ( telegram.Qcmd = SUCCSESS) 
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


