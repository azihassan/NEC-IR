rm *.o *.elf *.hex

avr-gcc -c \
        -std=gnu99 \
        -Os \
        -mcall-prologues \
        -g \
        -Wall \
        -ffunction-sections -fdata-sections \
        -mmcu=atmega328p \
        -DF_CPU=16000000L \
        -I/usr/include/simavr \
        main.c -o main.o &&

avr-gcc -Os \
        -mmcu=atmega328p \
        -mcall-prologues \
        -DF_CPU=16000000L \
        -g \
        -ffunction-sections \
        -fdata-sections \
        -I/usr/include/simavr \
        -Wl,--gc-sections \
        main.o -o main.elf &&

avr-objcopy -O ihex -R .eeprom main.elf main.hex &&
echo 1

#avr-g++ -c -g -Os -w -std=gnu++11 -E -CC -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10812 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR -I/home/hassan/Téléchargements/arduino-1.8.12/hardware/arduino/avr/cores/arduino -I/home/hassan/Téléchargements/arduino-1.8.12/hardware/arduino/avr/variants/eightanaloginputs /tmp/arduino_build_782916/sketch/Blink.ino.cpp -o /dev/null

avrdude -v -patmega328p -carduino -P/dev/ttyUSB3 -b57600 -U flash:w:main.hex:i
