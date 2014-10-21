#include "main.h"

#define 	USARTx		 	 USART6
#define 	USARTx_CLK	 	 RCC_APB2Periph_USART6

#define 	USARTx_RX_GPIO_PIN	 GPIO_Pin_6
#define 	USARTx_RX_GPIO_PORT	 GPIOC
#define 	USARTx_RX_GPIO_CLK       RCC_AHB1Periph_GPIOC

#define 	USARTx_TX_GPIO_PIN	 GPIO_Pin_7
#define 	USARTx_TX_GPIO_PORT	 GPIOC
#define 	USARTx_TX_GPIO_CLK       RCC_AHB1Periph_GPIOC


#define		USARTx_CS_GPIO_PIN	 GPIO_Pin_8
#define 	USARTx_CS_GPIO_PORT	 GPIOG
#define 	USARTx_CS_GPIO_CLK       RCC_AHB1Periph_GPIOG



#define DE()		USARTx_CS_GPIO_PORT->BSRRH |= GPIO_Pin_8; // chip select  
#define DD()		USARTx_CS_GPIO_PORT->BSRRL |= GPIO_Pin_8; // chip select  


#define MAX_STRLEN 128
#define REQ_MAX_LEN 512
int fd; 
//recived charachters
volatile uint8_t received_string[MAX_STRLEN + 1 ]; 
int rx_length; 
static unsigned portBASE_TYPE xHigherPriorityTaskWoken_usart;
struct Spd_Settings{
	
	uint16_t speed ;
	uint16_t param1 ;
	uint16_t maxRPM ;
	uint16_t upRamp ;
	uint16_t downRamp ;
};



uint16_t input_Register[10];
//xSemaphoreHandle xSmphrUSART; 
//xTaskHandle setSpeedHandle;

void copy_inputregisters(uint16_t *);

void init_USARTx(void);

void USART_puts(  uint8_t *, int);

/* function for  crc calculation */
uint16_t crc16(uint8_t *, uint16_t );

/* write read to modbus */
void write_read_modbus( uint8_t * , uint8_t * , int, int );


/* modbus read input bits */
void modbus_RIB( int16_t , int, uint8_t * );


/* modbus write input bits */
void modbus_WIB( uint16_t , int , uint8_t * );


/* Write the values from the array to the registers of the remote device */
void modbus_WR( int , int , const uint16_t *);

/* Read the values from the array to the registers of the remote device */
void modbus_RR( int , int , uint16_t *);

void setSpeed_task(void *);
void motorHeartBit_task(void *);
