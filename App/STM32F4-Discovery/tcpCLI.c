#include "tcpCLI.h"



portBASE_TYPE prvMotorCommand1 ( 	int8_t *pcWriteBuffer, 
						size_t xWriteBufferLen, 
						const int8_t *pcCommandString)
{
	/* globals: 
*/

	return pdFALSE;










}

// prototypes of CLI functions 

portBASE_TYPE prvMotorCommand2 ( 	int8_t *pcWriteBuffer, 
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

	int8_t *Option, *Param;
	int xOptionLength, xParamLength; 
	
	// get option from command line 

	Option = FreeRTOS_CLIGetParameter( pcCommandString, // command string 
					  1,  		   // first parameter
				  	  &xOptionLength // parameter string length
					  
					 );
	// get parameter from command line 
	
	Param = FreeRTOS_CLIGetParameter( pcCommandString, // command string 
					  2,  		   // 2nd parameter
				  	  &xParamLength // parameter string length
					  
					 );





	if( !strncmp( Option, "speed", 5) ) 
	{
				// now convert parameter to proper value and check if it is in range 
        		Param[xParamLength-2] = '\0'; 	
			int s1 = atoi ( Param ); 
			// if speed is in range
			if(s1 < 10 ||  s1 > 100 )
			{
				// send error via TCP
				//
				int len = strlen ( Param ) + 11 ; 
				uint8_t buf [len]; //= "Error: speed is out of range!\n\n";
				sprintf(buf, "Errror : %d\n\n", s1);
				buf[11+ 3] = "\0"; 
				send( socket_0, buf, len, 0);

	       			return pdFALSE; 	
			}	
	
			// convert to correct value ( * 100 ) 
			s1 *= 100; 

			// resume task set speed 
//			vTaskResume ( setSpeedHandle ) ; 

			// send value to setSpeed_task via Queue 
			if ( !xQueueSend ( QSpd_handle, &s1, 500 ) )
			{	
				uint8_t * buf = "Speed succsesfully set.\n\n";
			       	int len = 25; 	
				send( socket_0, buf, len, 0);

				return pdPASS;
			}
			else
			{
				// send to Queue was unsuccsessful
			        // send error via TCP 
		
				uint8_t * buf = "Error sending Queue!\n\n";
	 		      	int len = 22; 	
				send( socket_0, buf, len, 0);

				return pdFALSE; 	
			}



			// end of speed paramete



	}


	return pdFALSE;










}


