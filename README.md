attiny-spi-pcm
==============

PCM sample player (ATTINY + SPI flash)

Flashing with avrdude on windows
--------------------------------

`avrdude -p t13 -P COM11 -c avrisp -b 19200 -U flash:w:".\tiny-pcm-spi\tiny-pcm-spi\Release\tiny-pcm-spi.hex":i`
`avrdude -p t13 -P COM11 -c avrisp -b 19200 -U lfuse:w:0x69:m -U hfuse:w:0xfd:m`
