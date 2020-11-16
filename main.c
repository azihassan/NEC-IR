#define F_CPU 16000000L
#include "codes.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/avr_mcu_section.h>

AVR_MCU(F_CPU, "atmega328");

const struct avr_mmcu_vcd_trace_t _mytrace[]  _MMCU_ = {
    { AVR_MCU_VCD_SYMBOL("PB5"), .mask = (1 << PB5), .what = (void*)&PORTB, },
};

void modulate(int pin, int microseconds)
{
    for(int i = 0; i < microseconds / 26; i++)
    {
        PORTB |= (1 << pin);
        _delay_us(13);
        PORTB &= ~(1 << pin);
        _delay_us(13);
    }
}

void mark(int pin)
{
    modulate(pin, 560);
    _delay_us(1680);
}

void space(int pin)
{
    modulate(pin, 560);
    _delay_us(560);
}

void init_message(int pin)
{
    modulate(pin, 9000);
    PORTB &= ~(1 << pin);
    _delay_us(4500);
}

void finish_message(int pin)
{
    modulate(pin, 560);
    PORTB &= ~(1 << pin);
}

void repeat(int pin)
{
    PORTB |= (1 << pin);
    _delay_us(9000);

    PORTB &= ~(1 << pin);
    _delay_us(2250);

    PORTB |= (1 << pin);
    _delay_us(560);
    PORTB &= ~(1 << pin);
}

void send_byte(int pin, uint8_t value)
{
    for(int i = 0; i < 8; i++)
    {
        if(value & 1)
        {
            mark(pin);
        }
        else
        {
            space(pin);
        }
        value >>= 1;
    }
}

void send_message(int pin, uint16_t address, uint8_t command)
{
    init_message(pin);
    send_byte(pin, address & 0xff);
    send_byte(pin, (address & 0xff00) >> 8);
    send_byte(pin, command);
    send_byte(pin, ~command);
    finish_message(pin);
}

int main(void)
{
    send_message(PB5, ADDRESS, POWER);
    _delay_ms(8000);
    send_message(PB5, ADDRESS, SOURCE);
    _delay_ms(2000);
    send_message(PB5, ADDRESS, UP);
    _delay_ms(2000);
    send_message(PB5, ADDRESS, OK);
    _delay_ms(1000);
    send_message(PB5, ADDRESS, OK);
    _delay_ms(1000);
    send_message(PB5, ADDRESS, DOWN);
    _delay_ms(1000);
    send_message(PB5, ADDRESS, OK);
    _delay_ms(1000);
    send_message(PB5, ADDRESS, UP);
    _delay_ms(1000);
    send_message(PB5, ADDRESS, OK);

    sleep_cpu();

    return 0;
}
