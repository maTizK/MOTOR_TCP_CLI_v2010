/*! \file modbus_mk.h 
 *  \brief modbus protocol functions and motor control header file 
 */

#include "main.h"
#include "tcpCLI.h"

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
	
	uint16_t speed; //!< motor speed
	uint16_t param1; //!< parameter ???  
	uint16_t maxRPM ; //!< maximum RPM
	uint16_t upRamp ; //!< up ramp time
	uint16_t downRamp ; //!< down ramp time
};



uint16_t input_Register[10];


void copy_inputregisters(uint16_t *);

/*! \fn void init_USARTx(void)
 *  \brief initialize USART pheriph 
 * 
 */
void init_USARTx(void);


/*! \fn void USART_puts(  uint8_t *, int)
 *  \param *s - pointer to data to send 
 *  \param nb - size of data
 */
void USART_puts(  uint8_t *, int);

/*! \fn uint16_t crc16(uint8_t *, uint16_t )
 *  \brief calculation of checksum 
 *  \param *buffer - pointer to buffer 
 *  \param buffer_length - buffer length 
 */ 
uint16_t crc16(uint8_t *, uint16_t );

/*! \fn void write_read_modbus( uint8_t * , uint8_t * , int, int )
 *  \brief write and read data to modbus line 
 *  \param *req - data to send 
 *  \param *rsp - received data 
 *  \param write_len - size of data to send 
 *  \param read_len - size of dataa to read
 */
void write_read_modbus( uint8_t * , uint8_t * , int, int );


/*! \fn void modbus_RIB( int16_t , int, uint8_t * )
 *  \brief read input bits on motor side
 *  \param address - motor register address 
 *  \param nb - number of bits to read
 *  \param *dst - destination buffer of read data
 */
void modbus_RIB( int16_t , int, uint8_t * );


/*! \fn void modbus_WIB( uint16_t , int , uint8_t * )
 *  \brief write bits to register
 *  \param address - register address 
 *  \param nb - number of bits to send 
 *  \param *src - source buffer of bits to send 
 */
void modbus_WIB( uint16_t , int , uint8_t * );


/*! \fn void modbus_WR( int , int , const uint16_t *)
 *  \brief write to register of motor 
 *  \param address - register address 
 *  \param nb  - size of data to send 
 *  \param *src - pointer to data to send 
 */
void modbus_WR( int , int , const uint16_t *);

/*! \fn void modbus_RR( int , int , uint16_t *)
 *  \brief read from motor register 
 *  \param address - register address 
 *  \param nb - size of data to receieve 
 *  \param *src - receieved data 
 */
void modbus_RR( int , int , uint16_t *);


void setSpeed_task(void *);
void motorHeartBit_task(void *);

