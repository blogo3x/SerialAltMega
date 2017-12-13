/**************************************************************************/
/*! 
    @file     serialalt.h
    @author   Iqro Kurniawan (Stechoq)
	
	This is a alternative library of Serial Communication using USART in Arduino
	* This library uses RX interrupt to handle the incoming data, so it's possible 
	* to run an interrupt-driven program.
	* Please note, this library uses Hardware serial, and in case the Arduino has
	* more than one hardware serial channel, it only uses the first serial channel
	* that available in that board
	
	* Tested in Arduino Uno and Arduino Mega
	*  
    v1.0  - First release
*/
/**************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/sleep.h>
#include "SerialAltMega.h"
#include "Arduino.h"

SerialAlt serialX;
volatile uint8_t* ArrData;
volatile char p_rxBuffer=0;
volatile char rxStart0=0;
volatile char rxStart1=0;
void (*otherFunction)();

SerialAlt::SerialAlt(){	
}

void SerialAlt::init_usart(int _baud){	
	uint16_t _ubrr = ((F_CPU / 4 / _baud - 1) / 2);
	UBRR1H=(_ubrr>>8);
	UBRR1L=_ubrr;
	UCSR1A |= (1<<U2X1);
	UCSR1C |= (3<<UCSZ10);
	UCSR1B |= (1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1);
	attach(arrayOfData);
}

void SerialAlt::begin(int _baud,int length){	
	init_usart(_baud);
	key = 0x255;
	
	rxBuffer = length;
	ArrData = (char*)malloc(rxBuffer * sizeof(char));
	
	for(char i=0;i<length;i++)
		ArrData[i]=0;
	
	sei();
}

void SerialAlt::begin(int _baud){
	init_usart(_baud);
	
	key = 0x255;
	
	rxBuffer = 8;
	ArrData = (char*)malloc(rxBuffer * sizeof(char));
	
	for(char i=0;i<rxBuffer;i++)
		ArrData[i]=0;
	
	sei();
}

void SerialAlt::write(uint8_t Data)
{
  while(!(UCSR1A&(1<<UDRE1)));
  UDR1 = Data;
}

void SerialAlt::print(char* str)
{
	while(*str){
		write(*str++);
	}
}

void SerialAlt::println(char* str)
{
	print(str);
	write(10);
}

void SerialAlt::print(int num)
{
	printf("%d",num);
}

void SerialAlt::println(int num)
{
	printf("%d\n",num);
}

void SerialAlt::printf(const char *format, ...)
{
    char s[100];
    va_list arg;
   
    va_start (arg, format);
    vsprintf (s, format, arg);
    va_end (arg);

    print(s);
}

void SerialAlt::attach(void (*func)()) {
  otherFunction = func;
}

ISR (USART1_RX_vect)
{
	serialX.recByte=UDR1;
	(*otherFunction)();
}

void arrayOfData(){	
	if (rxStart0 && rxStart1)
	{
		if (!(UCSR1A & ((1 << FE1) | (1 << DOR1))));
		{
		  if (serialX.recByte != __END)
			ArrData[p_rxBuffer++] = serialX.recByte;

		  if (p_rxBuffer == serialX.rxBuffer || serialX.recByte == __END)
		  {
			p_rxBuffer = 0;
			rxStart0 = 0;
			rxStart1 = 0;
		  }
		}
	}

	if (rxStart0) {
		if (!rxStart1 && serialX.recByte == serialX.key) {
		  rxStart1 = 1;
		}
	}

	if (!rxStart0 && serialX.recByte == serialX.key) {
		rxStart0 = 1;
		rxStart1 = 0;
	}
}
