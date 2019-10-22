/*******************************************************************************
*   $FILE:  main.c
*   Atmel Corporation:  http://www.atmel.com \n
*   Support email:  www.atmel.com/design-support/
******************************************************************************/

/*  License
*   Copyright (c) 2010, Atmel Corporation All rights reserved.
*
*   Redistribution and use in source and binary forms, with or without
*   modification, are permitted provided that the following conditions are met:
*
*   1. Redistributions of source code must retain the above copyright notice,
*   this list of conditions and the following disclaimer.
*
*   2. Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
*   3. The name of ATMEL may not be used to endorse or promote products derived
*   from this software without specific prior written permission.
*
*   THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
*   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
*   SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
*   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
*   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
*   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*----------------------------------------------------------------------------
                            compiler information
----------------------------------------------------------------------------*/

#define F_CPU 8000000UL

/*----------------------------------------------------------------------------
                                include files
----------------------------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <util/delay.h>


#define __delay_cycles(n)     __builtin_avr_delay_cycles(n)
#define __enable_interrupt()  sei()

#include "touch_api.h"
#include "touch.h"
/*----------------------------------------------------------------------------
                            manifest constants
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
                                    macros
----------------------------------------------------------------------------*/

#define GET_SENSOR_STATE(SENSOR_NUMBER) qt_measure_data.qt_touch_status.sensor_states[(SENSOR_NUMBER/8)] & (1 << (SENSOR_NUMBER % 8))
#define GET_ROTOR_SLIDER_POSITION(ROTOR_SLIDER_NUMBER) qt_measure_data.qt_touch_status.rotor_slider_values[ROTOR_SLIDER_NUMBER]

#define TRUE		1
#define FALSE		0
#define ADC_VREF_TYPE 0x60

#define Read_RIGHTUP		ADC_read(2)
#define Read_RIGHTDOWN		ADC_read(0)
#define Read_LEFTUP			ADC_read(3)
#define Read_LEFTDOWN		ADC_read(1)
#define ON			1
#define OFF			0

#define LEDright_reset()		DDRB &= ~0x01
#define LEDright_set()	DDRB |= 0x01

/*----------------------------------------------------------------------------
                            type definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
                                prototypes
----------------------------------------------------------------------------*/
extern void touch_measure(void);
extern void touch_init( void );
extern void init_system( void );
extern void init_timer_isr(void);
extern void set_timer_period(uint16_t);
/*----------------------------------------------------------------------------
                            Structure Declarations
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
                                    macros
----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
                                global variables
----------------------------------------------------------------------------*/
/* Timer period in msec. */
uint16_t qt_measurement_period_msec = QT_MEASUREMENT_PERIOD_MS;
uint16_t time_ms_inc=0;

uint8_t touch_sense_old;
uint8_t touch_sense_new;
uint8_t touch_value;
uint8_t touch_cnt;
int touch_level;

uint8_t UpRight_sense_old, UpLeft_sense_old, DownRight_sense_old, DownLeft_sense_old ;
uint8_t UpRight_sense_new, UpLeft_sense_new, DownRight_sense_new, DownLeft_sense_new ;

uint8_t up_sensed, down_sensed, touch_sensed;
uint8_t SwitchStatus;
uint8_t rx_cnt;
uint8_t pR_Lenght, pR_CMD0, pR_CMD1, pR_Data[5];
uint8_t Wheel_value;

typedef struct {
	uint8_t Lenght;
	uint8_t CMD1;
	uint8_t CMD0;
	uint8_t Data[5];
	uint8_t FCS;
} UART_Packet;

UART_Packet R_packet, T_packet;

/*----------------------------------------------------------------------------
                                extern variables
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
                                static variables
----------------------------------------------------------------------------*/

/* flag set by timer ISR when it's time to measure touch */
volatile uint8_t time_to_measure_touch = 0u;

/* current time, set by timer ISR */
volatile uint16_t current_time_ms_touch = 0u;




void ADC_init()
{
	// ADC initialization
	// ADC Clock frequency: 1000.000 kHz
	// ADC Voltage Reference: AVCC pin
	// ADC Auto Trigger Source: ADC Stopped
	// Only the 8 most significant bits of
	// the AD conversion result are used
	// Digital input buffers on ADC0: On, ADC1: On, ADC2: On, ADC3: On
	// ADC4: On, ADC5: On
	DIDR0=0x00;
	ADMUX=ADC_VREF_TYPE & 0xff;
	ADCSRA=0x83;
}

void GPIO_init(){
	// Input/Output Ports initialization
	// Port B initialization
	// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=Out Func0=Out
	// State7=T State6=T State5=T State4=T State3=T State2=T State1=0 State0=0
	PORTB=0x05;
	DDRB=0x00;
}


void UART_init(){
	
	// USART initialization
	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// USART Receiver: On
	// USART Transmitter: On
	// USART0 Mode: Asynchronous
	// USART Baud Rate: 38400
	UCSR0A=0x00;
	UCSR0B=0x98;
	UCSR0C=0x06;
	UBRR0H=0x00;
	UBRR0L=0x0C;
}

void TIMER1_init()
{
	// Timer/Counter 1 initialization
	// Clock source: System Clock
	// Clock value: 3.906 kHz
	// Mode: CTC top=OCR1A
	// OC1A output: Discon.
	// OC1B output: Discon.
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer1 Overflow Interrupt: Off
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: On
	// Compare B Match Interrupt: Off
	TCCR1A=0x00;
	TCCR1B=0x0D;
	TCNT1H=0x00;
	TCNT1L=0x00;
	ICR1H=0x00;
	ICR1L=0x00;
	OCR1AH=0x0F;
	OCR1AL=0x42;
	OCR1BH=0x00;
	OCR1BL=0x00;
	
	// Timer/Counter 1 Interrupt(s) initialization
	TIMSK1=0x02;
}

void TIMER2_init()
{
	// Timer/Counter 2 initialization
	// Clock source: System Clock
	// Clock value: 7.813 kHz
	// Mode: CTC top=OCR2A
	// OC2A output: Disconnected
	// OC2B output: Disconnected
	ASSR=0x00;
	TCCR2A=0x02;
	TCCR2B=0x07;
	TCNT2=0x00;
	OCR2A=0xEF;		//interrupt about each 30ms: (1/7813)x239=0.0305
	OCR2B=0x00;
	
	// Timer/Counter 2 Interrupt(s) initialization
	TIMSK2=0x02;

}


uint8_t ADC_read(uint8_t adc_ch)
{
	ADMUX=adc_ch | (ADC_VREF_TYPE & 0xff);
	// Delay needed for the stabilization of the ADC input voltage
	_delay_us(10);
	// Start the AD conversion
	ADCSRA|=0x40;
	// Wait for the AD conversion to complete
	while ((ADCSRA & 0x10)==0);
	ADCSRA|=0x10;
	return ADCH;
}


void TransmitByte( uint8_t data )
{
	while ( !(UCSR0A  & (1<<UDRE0)) );		/* Wait for empty transmit buffer */
	
	UDR0 = data; 							/* Start transmitting */
}

ISR(TIMER1_COMPA_vect)
{
	up_sensed = FALSE;
	down_sensed = FALSE;
	touch_sensed = FALSE;
	touch_cnt = 0x00;
	touch_level = 0x00;
	
	TCCR1B=0x00;	//timer1 stop
	DDRB &= ~0x04;	//LED2 OFF
}

ISR(TIMER2_COMPA_vect)
{
	int UpRight_diff, DownRight_diff, UpLeft_diff, DownLeft_diff, touch_sense_diff;			
	
	touch_sense_new = GET_ROTOR_SLIDER_POSITION(0);		
	touch_sense_diff = (int)touch_sense_new - (int)touch_sense_old;	
	touch_measure();
	
 	if (abs(touch_sense_diff) > 5 )
 	{
		if (touch_sensed == TRUE)
		{
			touch_cnt++;
			if (touch_sense_new > touch_sense_old)
			{
				touch_level++;
				//TransmitByte(0x00);
			}
			else if (touch_sense_new < touch_sense_old)
			{
				touch_level--;
				//TransmitByte(0xFF);
			}
										
			if (touch_cnt == 4)						//each 120ms a command will send to ZigBee core
			{
				touch_cnt = 0;
				if (touch_level < 0)				//detect that if most of touches was Positive
					T_packet.Data[0] = 0x02;  
				else if (touch_level > 0)			//detect that if most of touches was Negative
					T_packet.Data[0] = 0x01;
								
				touch_level = 0x00;
				
				//TransmitByte(T_packet.Data[0]);
				//TransmitByte(T_packet.Data[1]);
				T_packet.CMD0 = 0x20;
				T_packet.CMD1 = 0xF0;
				T_packet.Lenght = 0x01;
				send_packet();
			}
		}
		
		touch_sensed = TRUE;
		TCCR1B=0x0D;				//timer1 start			
		DDRB |= 0x04;				//LED2 ON		 
 		touch_sense_old = touch_sense_new;		  		
 	}
		
	UpRight_sense_old = UpRight_sense_new;
	UpRight_sense_new = Read_RIGHTUP;
	_delay_us(10);
	UpLeft_sense_old = UpLeft_sense_new;
	UpLeft_sense_new = Read_LEFTUP;
	_delay_us(10);
	DownRight_sense_old = DownRight_sense_new;
	DownRight_sense_new = Read_RIGHTDOWN;
	_delay_us(10);
	DownLeft_sense_old = DownLeft_sense_new;
	DownLeft_sense_new = Read_LEFTDOWN;
	_delay_us(10);
	
	
	UpRight_diff = (int)UpRight_sense_new - (int)UpRight_sense_old;
	DownRight_diff = (int)DownRight_sense_new - (int)DownRight_sense_old;
	UpLeft_diff = (int)UpLeft_sense_new - (int)UpLeft_sense_old;
	DownLeft_diff = (int)DownLeft_sense_new - (int)DownLeft_sense_old;	
	
	if (DownRight_diff > 1 || DownLeft_diff > 1 )
	{
		if (up_sensed == TRUE)
		{
			up_sensed =FALSE;
			TCCR1A=0x00;TCCR1B=0x00;	//timer1 stop			
			
			if (SwitchStatus == OFF)
			{			
				SwitchStatus = ON;
				LEDright_set();
							
				T_packet.Lenght = 1;
				T_packet.CMD0 = 0x20;
				T_packet.CMD1 = 0xD0;
				T_packet.Data[0] = 0x01;
				send_packet();
			}
		}
		else if (DownRight_diff > 1 && DownLeft_diff > 1)
		{
			down_sensed = TRUE;
			TCCR1B=0x0D;				//timer1 start					
		}
	}

	if (UpRight_diff > 1 || UpLeft_diff > 1 )
	{			
		if (down_sensed == TRUE)
		{
			down_sensed =FALSE;
			TCCR1A=0x00;TCCR1B=0x00;	//timer1 stop
			if (SwitchStatus == ON)
			{				
				SwitchStatus = OFF;		
				LEDright_reset();	
			
				T_packet.Lenght = 1;
				T_packet.CMD0 = 0x20;
				T_packet.CMD1 = 0xD1;
				T_packet.Data[0] = 0x00;
				send_packet();
			}
		}			
		else if(UpRight_diff > 1 && UpLeft_diff > 1)
		{		
			up_sensed = TRUE;
			TCCR1B=0x0D;				//timer1 start
		}
	}
		
}

ISR(USART_RX_vect)
{
	uint8_t ReceivedByte;
	ReceivedByte = UDR0;
	//DDRB = 0x00;
	
	if (ReceivedByte == 0xFE && rx_cnt == 0)
		rx_cnt++;
	else if (rx_cnt == 1)
	{
		R_packet.Lenght = ReceivedByte;
		rx_cnt++;
	}
	else if (rx_cnt == 2)
	{
		R_packet.CMD0 = ReceivedByte;
		rx_cnt++;
	}
	else if (rx_cnt == 3)		
	{
		R_packet.CMD1 = ReceivedByte;
		rx_cnt++;
	}
	else if (rx_cnt < 2 + R_packet.Lenght)
	{
		R_packet.Data[rx_cnt - 3] = ReceivedByte;
		rx_cnt++;
	}
	else
	{
		R_packet.FCS = ReceivedByte;				//get fcs byte
		rx_cnt = 0;
		
		uint8_t fcs_calculate = R_packet.Lenght ^ R_packet.CMD0 ^ R_packet.CMD1 ;	//Calculate received packet fcs
		
		for (int j = 0; j < R_packet.Lenght; j++)
			fcs_calculate ^= R_packet.Data[j];
		
		if (R_packet.FCS == fcs_calculate)			
			procces_packet();		
	}
}
		
void procces_packet()
{
	/*assert(!"The method or operation is not implemented.")*/	
	switch (R_packet.CMD0)
	{
		case 0x40:
		switch(R_packet.CMD1)
		{
			case 0x10:
			DDRB &= ~0x01;	//LED1 OFF
			
			break;
			case 0x11:
			DDRB |= 0x01;	//LED1 ON
			break;
			case 0x12:
			//LED1 BLINK
			break;
			
			case 0x20:
			DDRB &= ~0x04;	//LED2 OFF
			break;			
			case 0x21:
			DDRB |= 0x04;	//LED2 ON
			break;
			case 0x22:
			//LED2 BLINK			
			break;
		}
		break;
		
		case 0x60:
		switch(R_packet.CMD1)
		{
			case 0xF0:
			//wheel value update status
			break;
			
			case 0xE0:
			//Light sensor report status
			break;									
		}
		break;
	}
}

void send_packet()
{
	TransmitByte(0xFE);
	TransmitByte(T_packet.Lenght);
	TransmitByte(T_packet.CMD0);
	TransmitByte(T_packet.CMD1);
		
	T_packet.FCS = T_packet.Lenght ^ T_packet.CMD0 ^ T_packet.CMD1;
		
	for (int i=0; i<T_packet.Lenght; i++)
		{
			TransmitByte(T_packet.Data[i]);
			T_packet.FCS ^= T_packet.Data[i];
		}		
	TransmitByte(T_packet.FCS);
}


/*============================================================================
Name    :   main
------------------------------------------------------------------------------
Purpose :   main code entry point
Input   :   n/a
Output  :   n/a
Notes   :
============================================================================*/

int main( void )
{

   /* initialise host app, pins, watchdog, etc */
    init_system();

    /* configure timer ISR to fire regularly */
    init_timer_isr();

	/* Initialize Touch sensors */
	touch_init();
	
	/* Initialise Peripherals */
	UART_init();	_delay_ms(100);		GPIO_init();
	_delay_ms(100);	
	ADC_init();
	
	up_sensed = down_sensed = FALSE;
// 	UpRight_sense_new = Read_RIGHTUP;
// 	_delay_us(10);
// 	UpLeft_sense_new = Read_LEFTUP;
// 	_delay_us(10);
// 	DownRight_sense_new = Read_RIGHTDOWN;
// 	_delay_us(10);
// 	DownLeft_sense_new = Read_LEFTDOWN;
// 	_delay_us(10);
		
		
	_delay_ms(100);	
	TIMER2_init();
	_delay_ms(100);	
	TIMER1_init();
	_delay_ms(100);

    /* enable interrupts */
    __enable_interrupt();

	
    /* loop forever */
    for( ; ; )
    {
        
// 		touch_sense_new = GET_ROTOR_SLIDER_POSITION(0);	
// 		TransmitByte(touch_sense_new);

		
/*		DDRB = 0x01;*/
		
//		TransmitByte(UpLeft_sense_new);
//		TransmitByte(UpRight_sense_new);
// 		TransmitByte(DownLeft_sense_new);
// 		TransmitByte(DownRight_sense_new);
		
		touch_sense_new = GET_ROTOR_SLIDER_POSITION(0);
		//TransmitByte(touch_sense_new);
// 		
// 		
// 		_delay_ms(500);
/*		DDRB = 0x00;*/
		
    /*  Time Non-critical host application code goes here  */
    }
}

