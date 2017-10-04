MCU=attiny13
F_CPU=4800000
CFLAGS=-std=c99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
TARGET=tiny-pcm-spi
SRCS=tiny-pcm-spi.c spi.c
HEADERS=spi.h
FLASH_PORT=/dev/ttyACM0

all: ${TARGET}.hex

${TARGET}.bin: ${SRCS} ${HEADERS}
	avr-gcc ${CFLAGS} -o ${TARGET}.bin ${SRCS}

${TARGET}.hex: ${TARGET}.bin
	avr-objcopy -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex
	avr-size ${TARGET}.hex

flash:
	avrdude -p ${MCU} -P ${FLASH_PORT} -c avrisp -b 19200 -U lfuse:w:0x69:m -U hfuse:w:0xfd:m
	avrdude -p ${MCU} -P ${FLASH_PORT} -c avrisp -b 19200 -U flash:w:"${TARGET}.hex":i

clean:
	rm -f ${TARGET}.bin ${TARGET}.hex
