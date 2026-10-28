#include <xc.h>
#include "userdef.h"

#define F_CPU 1000000UL
#include <util/delay.h>

static void LCDPulseEnable(void)
{
    LCDE_H;
    _delay_us(2);
    LCDE_L;
}

static void LCDWriteCommand(unsigned char command)
{
    LCDRS_L;
    LCD_INS = command;
    _delay_us(2);
    LCDPulseEnable();

    if ((command == 0x01) || (command == 0x02))
    {
        _delay_ms(2);
    }
    else
    {
        _delay_us(50);
    }
}

static void LCDWriteData(unsigned char data)
{
    LCDRS_H;
    LCD_DATA = data;
    _delay_us(2);
    LCDPulseEnable();
    _delay_us(50);
}

void LCDInit(void)
{
    LCDE_L;
    LCDRS_L;
    _delay_ms(20);

    LCDWriteCommand(0x38);
    _delay_ms(5);
    LCDWriteCommand(0x38);
    _delay_us(200);
    LCDWriteCommand(0x38);

    LCDWriteCommand(0x0C);
    LCDWriteCommand(0x06);
    LCDWriteCommand(0x01);
}

void PrintL(const char *str, unsigned char line, unsigned char col)
{
    static const unsigned char line_address[4] =
    {
        0x80, 0xC0, 0x94, 0xD4
    };

    if ((line > 3) || (col > 19))
    {
        return;
    }

    LCDWriteCommand(line_address[line] + col);

    while (*str != '\0')
    {
        LCDWriteData((unsigned char)*str++);
    }
}

int main(void)
{
    MCUCR |= 0x80;
    DDRG = 0x1F;

    LCDInit();

    PrintL("hello", 0, 0);
    PrintL("embedded system", 3, 5);

    while (1)
    {
    }

    return 0;
}
