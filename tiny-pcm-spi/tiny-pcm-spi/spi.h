/*
 * IncFile1.h
 *
 * Created: 07.02.2014 21:38:33
 *  Author: Foxel
 */ 

#include <avr/io.h>
#include <util/delay.h>

#ifndef INCFILE1_H_
#define INCFILE1_H_

#define SETSCK     PORTB |= 0x04
#define CLRSCK     PORTB &= 0xFB
#define SETMOSI    PORTB |= 0x01
#define CLRMOSI    PORTB &= 0xFE
#define SETCS      PORTB &= 0xEF
#define CLRCS      PORTB |= 0x10
#define GETMISO    (PINB >> 3) & 0x01

#define SPIDELAY  // no delay
#define SPIDELAY2 _delay_us(1)

void spi_init(void);
void spi_start(void);
void spi_stop(void);
unsigned char spi_receive(void);
void spi_send(unsigned char byte);
unsigned char spi_transmit(unsigned char byte);


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


#endif /* INCFILE1_H_ */