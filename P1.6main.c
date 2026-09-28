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

void MenuDisplay(Menu *menu, unsigned char select)
{
    LCDWriteCommand(0x01);   // Xoa LCD truoc khi in menu moi

    PrintL(menu->Title, 0, 0);
    PrintL(menu->List1, 1, 0);
    PrintL(menu->List2, 2, 0);
    PrintL(menu->List3, 3, 0);

    PrintL(">", select, 0);
}

int main(void)
{
    unsigned char select = 1;
    Menu *menu;

    MCUCR |= 0x80;
    XMCRA = 0;
    DDRG = 0x1F;

    menu = &MainMenu;

    LCDInit();
    MenuDisplay(menu, select);

    while (1)
    {
        ReadKey;

        if (!Buttons.DOWN)
        {
            while (!Buttons.DOWN)
            {
                ReadKey;
            }

            select = (select == 3) ? 1 : select + 1;
            MenuDisplay(menu, select);
        }

        if (!Buttons.UP)
        {
            while (!Buttons.UP)
            {
                ReadKey;
            }

            select = (select == 1) ? 3 : select - 1;
            MenuDisplay(menu, select);
        }

        if (!Buttons.RIGHT)
        {
            while (!Buttons.RIGHT)
            {
                ReadKey;
            }

            switch (select)
            {
                case 1:
                    menu = (menu->Menulist1 == NULL) ? menu : menu->Menulist1;
                    break;

                case 2:
                    menu = (menu->Menulist2 == NULL) ? menu : menu->Menulist2;
                    break;

                case 3:
                    menu = (menu->Menulist3 == NULL) ? menu : menu->Menulist3;
                    break;
            }

            select = 1;
            MenuDisplay(menu, select);
        }

        if (!Buttons.LEFT)
        {
            while (!Buttons.LEFT)
            {
                ReadKey;
            }

            menu = (menu->pre == NULL) ? menu : menu->pre;
            select = 1;
            MenuDisplay(menu, select);
        }
    }

    return 0;
}
