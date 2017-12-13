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

#ifndef serialalt_h
#define serialalt_h

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/sleep.h>
#include "Arduino.h"

#define false 0
#define true 1
#define __END 13

extern volatile uint8_t* ArrData;
extern volatile char p_rxBuffer;
extern volatile char rxStart0;
extern volatile char rxStart1;
extern void (*otherFunction)();

class SerialAlt
{
  public:
    SerialAlt();
    void init_usart(int _baud);
	void write(uint8_t Data);
	void begin(int _baud,int length);
	void begin(int _baud);
	void printf(const char *format, ...);
	void print(char* str);
	void println(char* str);
	void print(int num);
	void println(int num);
	void attach(void (*otherFunction)());
	int rxBuffer;
	uint8_t recByte;
	uint8_t key;
	bool useKey;
  private:
};

extern SerialAlt serialX;
void arrayOfData();
#endif
