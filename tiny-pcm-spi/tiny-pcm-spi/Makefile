all: tiny-pcm-spi.hex

tiny-pcm-spi.o: tiny-pcm-spi.c spi.h
	avr-gcc -funsigned-char -funsigned-bitfields -Os -fpack-struct -fshort-enums -Wall -c -std=gnu99 -mmcu=attiny13a -o tiny-pcm-spi.o tiny-pcm-spi.c

tiny-pcm-spi.hex: tiny-pcm-spi.o
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "tiny-pcm-spi.o" "tiny-pcm-spi.hex"

clean:
	rm -f tiny-pcm-spi.o tiny-pcm-spi.hex
