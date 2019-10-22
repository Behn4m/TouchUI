#include "touch.h"

#if defined(_DEBUG_INTERFACE_) && defined(QDEBUG_TWI)

#include <avr/io.h>
#include <avr/interrupt.h>
#include "TWI_Master.h"
#include "QDebugTransport.h"

#if defined(QDEBUG_SPI)
#warning "TWI driver should not be included if SPI is defined as debug interface"
#endif


static unsigned char TWI_addr;    					// Transceiver buffer
static unsigned int TWI_msgSize;                   	// Number of bytes to be transmitted.

union TWI_statusReg TWI_statusReg = {0};            // TWI_statusReg is defined in TWI_Master.h

/****************************************************************************
Call this function to set up the TWI master to its initial standby state.
Remember to enable interrupts from the main application after initializing the TWI.
****************************************************************************/
void TWI_Master_Init(void)
{
  TWBR = TWI_BAUDSETTING;                           // Set bit rate register (Baudrate). Defined in header file.
// TWSR = TWI_TWPS;                                 // Not used. Driver presumes prescaler to be 00.
  TWDR = 0xFF;                                      // Default content = SDA released.
  TWCR = (1<<TWEN)|                                 // Enable TWI-interface and release TWI pins.
         (0<<TWIE)|(0<<TWINT)|                      // Disable Interupt.
         (0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // No Signal requests.
         (0<<TWWC);                                 //
}    
    
/****************************************************************************
Call this function to test if the TWI_ISR is busy transmitting.
****************************************************************************/
unsigned char TWI_Transceiver_Busy( void )
{
  return ( TWCR & (1<<TWIE) );                  // IF TWI Interrupt is enabled then the Transceiver is busy
}


void TWI_Send_Message( void )
{
  TWI_msgSize = ((TX_Buffer[1]<<8)+TX_Buffer[2]);// Number of data to transmit.
  
  TWI_addr  =(TWISLAVE_ADDR<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT); // Store slave address with R/W setting.
  
  TWI_statusReg.all = 0;      

  TWCR = (1<<TWEN)|                             // TWI Interface enabled.
         (1<<TWIE)|(1<<TWINT)|                  // Enable TWI Interupt and clear the flag.
         (0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|       // Initiate a START condition.
         (0<<TWWC);                             //

  while ( TWI_Transceiver_Busy() );             // Wait until TWI is ready for next transmission.
}

void TWI_Retrieve_Message( void )
{
  	unsigned int i;
  
  	TWI_addr  = (TWISLAVE_ADDR<<TWI_ADR_BITS) | (TRUE<<TWI_READ_BIT); 
	
  	TWI_statusReg.all = 0;      

  	TWCR = 	(1<<TWEN)|                             // TWI Interface enabled.
         	(1<<TWIE)|(1<<TWINT)|                  // Enable TWI Interupt and clear the flag.
         	(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|       // Initiate a START condition.
         	(0<<TWWC);                             //

  	while ( TWI_Transceiver_Busy() );             	// Wait until TWI is ready for next transmission.  
	
	if( ! TWI_statusReg.lastTransOK )               // Last transmission completed successfully.              
		for ( i=0; i<=TWI_msgSize; i++ )           	// Copy data from Transceiver buffer.
		  	RX_Buffer[i] = 0;						// Clear data if transmission not successful
}


// ********** Interrupt Handlers ********** //
/****************************************************************************
This function is the Interrupt Service Routine (ISR), and called when the TWI interrupt is triggered;
that is whenever a TWI event has occurred. This function should not be called directly from the main
application.
TWINT is not cleared automaticly, but must be cleared manually in the ISR.
****************************************************************************/

ISR(TWI_vect)
{
  static unsigned int TWI_bufPtr;
  volatile unsigned char FrameInProgress;
  
  switch (TWSR)
  {
  case TWI_START:             	// START has been transmitted  
    
  case TWI_REP_START:         	// Repeated START has been transmitted
    TWI_bufPtr = 0;         	// Set buffer pointer to the TWI Address location
    TWDR = TWI_addr;
	TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
	break;

  case TWI_MTX_ADR_ACK:       	// SLA+W has been tramsmitted and ACK received
  case TWI_MTX_DATA_ACK:      	// Data byte has been transmitted and ACK received
	
	if (TWI_bufPtr <= TWI_msgSize)
    {
        TWDR = TX_Buffer[TWI_bufPtr++];
		TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
    }
	else                    // Send STOP after last byte
    {
        TWI_statusReg.lastTransOK = TRUE;                 // Set status bits to completed successfully. 
		TWCR = (1<<TWEN)|(0<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC);
	}
    break;
	
  case TWI_MRX_ADR_ACK:       // SLA+R has been transmitted and ACK received

	TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
	break; 
	
  case TWI_MRX_DATA_ACK:      // Data byte has been received and ACK transmitted
    
	FrameInProgress=RxHandler(TWDR);
	
	if (FrameInProgress )                  	// Detect the last byte to NACK it.
	{
		// Send ACK after reception
		TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
	}
	else                    
	{
		//TWI_Protocol_State=TWI_STATE_MSG_START;			// TWI ready to receive new packet
		// Send NACK after reception
		TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
	}    

	break;	
    
  case TWI_MRX_DATA_NACK:     // Data byte has been received and NACK tramsmitted

	RxHandler(TWDR);
	
    TWI_statusReg.lastTransOK = TRUE;                 // Set status bits to completed successfully. 
	// Initiate a STOP condition.
	TWCR = (1<<TWEN)|(0<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC);
	break;      
    
  case TWI_ARB_LOST:          // Arbitration lost
	// Initiate a (RE)START condition.
	TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
	break;
    
  	case TWI_MTX_ADR_NACK:      // SLA+W has been transmitted and NACK received
    case TWI_MRX_ADR_NACK:      // SLA+R has been transmitted and NACK received    
    case TWI_MTX_DATA_NACK:     // Data byte has been tramsmitted and NACK received
//    case TWI_NO_STATE              // No relevant state information available; TWINT = �0�
    
  case TWI_BUS_ERROR:         // Bus error due to an illegal START or STOP condition
    default:     
                                                        // Reset TWI Interface
      TWCR = (1<<TWEN)|                                 // Enable TWI-interface and release TWI pins
             (0<<TWIE)|(0<<TWINT)|                      // Disable Interupt
             (0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // No Signal requests
             (0<<TWWC);                                 //
  }
}
#endif/* _DEBUG_INTERFACE_*/

//**********************************************************************************************************



