CC = avr-gcc
DUDE = avrdude
SOURCES = main.c
TTY = /dev/ttyUSB3
MCU = atmega328p
HEX = main.hex


compile: $(SOURCES)
	$(CC) -c \
		-std=gnu99 \
		-Os \
		-mcall-prologues \
		-g \
		-Wall \
		-ffunction-sections -fdata-sections \
		-mmcu=$(MCU) \
		-DF_CPU=16000000L \
		-I/usr/include/simavr \
		$(SOURCES) -o main.o;

	$(CC) -Os \
		-mmcu=$(MCU) \
		-mcall-prologues \
		-DF_CPU=16000000L \
		-g \
		-ffunction-sections \
		-fdata-sections \
		-I/usr/include/simavr \
		-Wl,--gc-sections \
		main.o -o main.elf;

	avr-objcopy -O ihex -R .eeprom main.elf $(HEX);

compile-debug: $(SOURCES)
	$(CC) -c \
		-std=gnu99 \
		-Os \
		-mcall-prologues \
		-g \
		-Wall \
		-ffunction-sections -fdata-sections \
		-mmcu=$(MCU) \
		-DF_CPU=16000000L \
		-Wl,--undefined=_mmcu,--section-start=.mmcu=0x910000 \
		$(SOURCES) -o main.o;

	$(CC) -Os \
		-mmcu=$(MCU) \
		-mcall-prologues \
		-DF_CPU=16000000L \
		-g \
		-ffunction-sections \
		-fdata-sections \
		-Wl,--gc-sections \
		-Wl,--undefined=_mmcu,--section-start=.mmcu=0x910000 \
		main.o -o main.elf;


upload: $(HEX)
	$(DUDE) -v -p$(MCU) -carduino -P$(TTY) -b57600 -U flash:w:main.hex:i
