/*
 * IncFile1.h
 *
 * Created: 07.02.2014 21:38:33
 *  Author: Foxel
 */ 

#include "spi.h"
#include <avr/io.h>
#include <util/delay.h>

void spi_init(void) {
	// pins 0, 2, 4 as output, 3 as input
	DDRB = (DDRB & 0xF7) | 0x15;
	spi_stop();
}

void spi_send(unsigned char byte) {
	spi_transmit(byte);
}

unsigned char spi_receive(void) {
	return spi_transmit(0x00);
}

unsigned char spi_transmit(unsigned char byte) {
	unsigned char bit;
	
	for (bit = 0; bit < 8; bit++) {
		if (byte & 0x80) {
			SETMOSI;
		} else {
			CLRMOSI;
		}			
		byte <<= 1;
		
		SPIDELAY;
		SETSCK;
		SPIDELAY;
		
		byte |= GETMISO;
		CLRSCK;
	}
	
	return byte;
}


void spi_start(void) {
	CLRSCK;
	CLRMOSI;
	CLRCS;
	SPIDELAY;
	SETCS;
}

void spi_stop(void) {
	CLRSCK;
	CLRMOSI;
	CLRCS;
}

