/*
 * tiny_pcm_spi.c
 *
 * Created: 07.02.2014 21:38:14
 *  Author: Foxel
 */ 

#define F_CPU 4800000UL
//#define LAST_TRACK_HIDDEN_COUNT 7

#include <avr/io.h>
#include "spi.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include <stdlib.h>

#define PCM_width       2

void init(void);
void sleep(void);
void start_pcm(void);
void setup_seed(void);

unsigned char sample_count = PCM_width;
unsigned char pcmBuffer = 0x00;

#ifdef LAST_TRACK_HIDDEN_COUNT
uint8_t EEMEM eemem_resets_count;
#endif

int main(void)
{
	if (((MCUSR >> BORF) & 0x01) == 1) {
		MCUSR = 0;
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();
		return 0;
	}
	
	setup_seed();
	spi_init();
	init();
	
	start_pcm();
	pcmBuffer = spi_receive();

	//sleep();

	sei();

	while(1);
}

ISR(TIM0_OVF_vect)
{
	cli();
	sample_count--;
	if (sample_count == 0)
	{
		
		OCR0B = pcmBuffer >> 1;
		//OCR0B = ((pcmBuffer-128)*2+128) & 0xff;
		sample_count = PCM_width;
		pcmBuffer = spi_receive();
		if (pcmBuffer == 0xff) {
			sleep();
			start_pcm();
		}
	}
	sei();
}

void init(void) {
	CLKPR = _BV(CLKPCE);
	CLKPR = 0x00;

	DDRB |= 0x02;
		
	/* COM0B1 + WGM00 */
	TCCR0A = 0x21;
	/*  CS00 */
	TCCR0B = _BV(CS00) | _BV(WGM02);
		
	OCR0A = 0x7F; //128 steps PWM
	OCR0B = 0x00;
		
	TCNT0  = 0;
	TIMSK0 = _BV(TOIE0);
}

void start_pcm(void) {
	spi_stop();
	spi_start();
	spi_send(0x03);
	spi_send(0x00);
	spi_send(0x00);
	spi_send(0x00);
	// if there is a list of sounds
	if (spi_receive() == 0xff) {
		unsigned char tracks_count = spi_receive();
		char pos;

		#ifdef LAST_TRACK_HIDDEN_COUNT
		tracks_count--;
		unsigned char resets_count = eeprom_read_byte(&eemem_resets_count);
		eeprom_write_byte(&eemem_resets_count, resets_count+1);
		// char pos = rand() % tracks_count;
		if (resets_count == LAST_TRACK_HIDDEN_COUNT) {
			// if resets count exceeded the last (hidden) track is played
			pos = tracks_count;
		} else {
			// else the one of non-hidden tracks is seleted randomly
			pos = rand() % tracks_count;
		}
		#else
		pos = rand() % tracks_count;
		#endif
		
		while (pos > 0) {
			spi_receive();
			spi_receive();
			spi_receive();
			pos--;
		}
		
		unsigned char addrh = spi_receive();
		unsigned char addrm = spi_receive();
		unsigned char addrl = spi_receive();

		spi_stop();
		spi_start();
		spi_send(0x03);
		spi_send(addrh);
		spi_send(addrm);
		spi_send(addrl);
	}
}

void sleep(void) {
	#ifdef LAST_TRACK_HIDDEN_COUNT
	eeprom_write_byte(&eemem_resets_count, 0x00);
	#endif
	
	spi_stop();
	DDRB = 0x00;
	PORTB |= 0x10;
	
	//DDRB = 0x10;
	//PORTB = 0x10;
	TIMSK0 = 0x00;
	TCCR0A = 0x00;
	TCCR0B = 0x00;

	cli();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	//sleep_bod_disable();
	sleep_mode();
	init();
}

void setup_seed(void)
{
	unsigned char oldADMUX = ADMUX;
	ADMUX |=  _BV(MUX0); //choose ADC2 on PB4
	ADCSRA |= _BV(ADPS2) |_BV(ADPS1) |_BV(ADPS0); //set prescaler to max value, 128
	
	ADCSRA |= _BV(ADEN); //enable the ADC
	ADCSRA |= _BV(ADSC);//start conversion
	
	while (ADCSRA & _BV(ADSC)); //wait until the hardware clears the flag. Note semicolon!
	
	unsigned int seed = ADCL;
	
	ADCSRA |= _BV(ADSC);//start conversion
	
	while (ADCSRA & _BV(ADSC)); //wait again note semicolon!
	
	seed = (seed << 8) + ADCL;
		
	srand(seed);
	
	ADCSRA &= ~_BV(ADEN); //disable ADC

	ADMUX = oldADMUX;
}
