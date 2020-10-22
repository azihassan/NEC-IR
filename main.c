#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/avr_mcu_section.h>

//11000001 11000111 11000000 00111111
AVR_MCU(F_CPU, "atmega328");

const struct avr_mmcu_vcd_trace_t _mytrace[]  _MMCU_ = {
    { AVR_MCU_VCD_SYMBOL("PB5"), .mask = (1 << PB5), .what = (void*)&PORTB, },
};

void modulate(int pin)
{
    for(int i = 0; i < 21; i++)
    {
        PORTB |= (1 << pin);
        _delay_us(6.575);
        PORTB &= ~(1 << pin);
        _delay_us(19.725);
    }
}

void mark(int pin)
{
    modulate(pin);
    _delay_us(1687.5);
}

void space(int pin)
{
    modulate(pin);
    _delay_us(562.5);
}

void init_message(int pin)
{
    /*
    for(int i = 0; i < 342; i++)
    {
        PORTB |= (1 << pin);
        _delay_us(13.15);
        PORTB &= ~(1 << pin);
        _delay_us(13.15);
    }*/
    //_delay_us(9000);
    for(int i = 0; i < 17; i++)
    {
        modulate(pin);
    }
    PORTB &= ~(1 << pin);
    _delay_us(4500);
}

void finish_message(int pin)
{
    modulate(pin);
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

void send_message(int pin, uint8_t address, uint8_t command)
{
    init_message(pin);
    send_byte(pin, address);
    send_byte(pin, ~address);
    send_byte(pin, command);
    send_byte(pin, ~command);
    finish_message(pin);
}

int main(void)
{
    //DDRB |= (1 << PB5) | (1 << PB0);

    //02:58:50.957 -> P=NEC  A=0xFE01 C=0x10
    //02:58:50.990 -> P=NEC  A=0xFE01 C=0x10 R
    //02:59:07.750 -> P=NEC  A=0xFE01 C=0x1
    //1001

    //send_byte(PB5, 0b11111111);
    //mark(PB5);
    //mark(PB5);
    //space(PB5);
    //mark(PB5);
    send_message(PB5, 0xff, 0xff);
    sleep_cpu();
    return 0;
}
