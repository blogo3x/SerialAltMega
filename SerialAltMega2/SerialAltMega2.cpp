/**************************************************************************/
/*! 
    @file     SerialAlt2.h
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
#include "SerialAltMega2.h"
#include "Arduino.h"

SerialAlt2 serialX2;
volatile uint8_t* ArrData2;
volatile char p_rxBuffer2=0;
volatile char rxStart02=0;
volatile char rxStart12=0;
void (*otherFunction2)();

SerialAlt2::SerialAlt2(){	
}

void SerialAlt2::init_usart(int _baud){	
	uint16_t _ubrr = ((F_CPU / 4 / _baud - 1) / 2);
	UBRR2H=(_ubrr>>8);
	UBRR2L=_ubrr;
	UCSR2A |= (1<<U2X2);
	UCSR2C |= (3<<UCSZ20);
	UCSR2B |= (1<<RXEN2)|(1<<TXEN2)|(1<<RXCIE2);
	attach(arrayOfData2);
}

void SerialAlt2::begin(int _baud,int length){	
	init_usart(_baud);
	key = 0x255;
	
	rxBuffer = length;
	ArrData2 = (char*)malloc(rxBuffer * sizeof(char));
	
	for(char i=0;i<length;i++)
		ArrData2[i]=0;
	
	sei();
}

void SerialAlt2::begin(int _baud){
	init_usart(_baud);
	
	key = 0xFF;
	
	rxBuffer = 8;
	ArrData2 = (char*)malloc(rxBuffer * sizeof(char));
	
	for(char i=0;i<rxBuffer;i++)
		ArrData2[i]=0;
	
	sei();
}

void SerialAlt2::write(uint8_t Data)
{
  while(!(UCSR2A&(1<<UDRE2)));
  UDR2 = Data;
}

void SerialAlt2::print(char* str)
{
	while(*str){
		write(*str++);
	}
}

void SerialAlt2::println(char* str)
{
	print(str);
	write(10);
}

void SerialAlt2::print(int num)
{
	printf("%d",num);
}

void SerialAlt2::println(int num)
{
	printf("%d\n",num);
}

void SerialAlt2::printf(const char *format, ...)
{
    char s[100];
    va_list arg;
   
    va_start (arg, format);
    vsprintf (s, format, arg);
    va_end (arg);

    print(s);
}

void SerialAlt2::attach(void (*func)()) {
  otherFunction2 = func;
}

ISR (USART2_RX_vect)
{
	serialX2.recByte=UDR2;
	(*otherFunction2)();
}

void arrayOfData2(){	
	if (rxStart02 && rxStart12)
	{
		if (!(UCSR2A & ((1 << FE2) | (1 << DOR2))));
		{
		  if (serialX2.recByte != __END)
			ArrData2[p_rxBuffer2++] = serialX2.recByte;

		  if (p_rxBuffer2 == serialX2.rxBuffer || serialX2.recByte == __END)
		  {
			p_rxBuffer2 = 0;
			rxStart02 = 0;
			rxStart12 = 0;
		  }
		}
	}

	if (rxStart02) {
		if (!rxStart12 && serialX2.recByte == serialX2.key) {
		  rxStart12 = 1;
		}
	}

	if (!rxStart02 && serialX2.recByte == serialX2.key) {
		rxStart02 = 1;
		rxStart12 = 0;
	}
}
