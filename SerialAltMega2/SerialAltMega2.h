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

#ifndef SerialAlt2_h
#define SerialAlt2_h

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/sleep.h>
#include "Arduino.h"

#define false 0
#define true 1
#define __END 13

extern volatile uint8_t* ArrData2;
extern volatile char p_rxBuffer2;
extern volatile char rxStart02;
extern volatile char rxStart12;
extern void (*otherFunction2)();

class SerialAlt2
{
  public:
    SerialAlt2();
    void init_usart(int _baud);
	void write(uint8_t Data);
	void begin(int _baud,int length);
	void begin(int _baud);
	void printf(const char *format, ...);
	void print(char* str);
	void println(char* str);
	void print(int num);
	void println(int num);
	void attach(void (*otherFunction2)());
	int rxBuffer;
	uint8_t recByte;
	uint8_t key;
	bool useKey;
  private:
};

extern SerialAlt2 serialX2;
void arrayOfData2();
#endif
