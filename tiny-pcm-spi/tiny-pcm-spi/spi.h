/*
 * IncFile1.h
 *
 * Created: 07.02.2014 21:38:33
 *  Author: Foxel
 */ 

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

#endif /* INCFILE1_H_ */
