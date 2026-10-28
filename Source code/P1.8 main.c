#define F_CPU 1000000UL
#include <xc.h>
#include "userdef.h"
#include <util/delay.h>


static void LCDPulseEnable(void)
{
    LCDE_H;
    _delay_us(2);
    LCDE_L;
}

/* Four common-anode seven-segment digits, driven through CS4..CS7. */
static void SegDisplayD(uint16_t value)
{
    if (value > 9999U) value = 9999U;
    LED7D1 = SegCode[value % 10U];
    LED7D2 = SegCode[(value / 10U) % 10U];
    LED7D3 = SegCode[(value / 100U) % 10U];
    LED7D4 = SegCode[(value / 1000U) % 10U];
}

static void LCDWriteCommand(unsigned char command)
{
    LCDRS_L;
    LCD_INS = command;
    _delay_us(2);
    LCDPulseEnable();

    if ((command == 0x01) || (command == 0x02)) {
        _delay_ms(2);
    } else {
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

void PrintL (const char *str, unsigned char line, unsigned char col) 
{
    static const unsigned char line_address[4] = {0x80, 0xC0, 0x94, 0xD4};
    if ((line > 3) || (col > 19)) {
        return;
    }

    LCDWriteCommand(line_address[line] + col);
    while (*str != '\0') {
        LCDWriteData((unsigned char)*str++);
    }
}
void MenuDisplay (Menu *menu, unsigned char select)
{
LCDWriteCommand(0x01);
PrintL (menu->Title ,0,0);
PrintL (menu->List1 ,1,0);
PrintL (menu->List2 ,2,0);
PrintL (menu->List3 ,3,0);
PrintL(">", select, 0);
}

static void PrintTemperature(int16_t temperature_q4)
{
    char text[17];
    uint16_t magnitude;
    uint16_t integer_part;
    uint8_t decimal_part;
    uint8_t pos = 0U;

    if (temperature_q4 < 0) {
        text[pos++] = '-';
        magnitude = (uint16_t)(-(temperature_q4 + 1)) + 1U;
    } else {
        magnitude = (uint16_t)temperature_q4;
    }

    integer_part = magnitude >> 4;
    decimal_part = (uint8_t)(((magnitude & 0x0FU) * 100U + 8U) >> 4);
    if (integer_part >= 100U) text[pos++] = (char)('0' + (integer_part / 100U));
    if (integer_part >= 10U) text[pos++] = (char)('0' + ((integer_part / 10U) % 10U));
    text[pos++] = (char)('0' + (integer_part % 10U));
    text[pos++] = '.';
    text[pos++] = (char)('0' + (decimal_part / 10U));
    text[pos++] = (char)('0' + (decimal_part % 10U));
    text[pos++] = ' ';
    text[pos++] = 'C';
    text[pos] = '\0';
    PrintL(text, 2, 5);
}

/* Temperature screen: press LEFT to return to the Sensor menu. */
static void ShowTemperatureScreen(void)
{
    int16_t temperature_q4;

    LCDWriteCommand(0x01);
    PrintL("DS18B20 1-WIRE", 0, 2);
    PrintL("Temperature:", 1, 2);

    while (1) {
        if (DS18B20_ReadTemperatureQ4(&temperature_q4) != 0U) {
            PrintL("                ", 2, 0);
            PrintTemperature(temperature_q4);
            /* 25.06 C is shown as 0025 on the four seven-segment digits. */
            SegDisplayD((uint16_t)((temperature_q4 < 0) ?
                        -(temperature_q4 / 16) : (temperature_q4 / 16)));
            PrintL("LEFT: Back      ", 3, 0);
        } else {
            PrintL("Sensor not found", 2, 1);
            PrintL("Check PB0/pullup", 3, 1);
        }

        ReadKey;
        if (!Buttons.LEFT) {
            while (!Buttons.LEFT) ReadKey;
            break;
        }
    }
}

int main(void)
{
unsigned char select = 1;
Menu *menu;
MCUCR |=0x80;
XMCRA = 0;
DDRG =0x1f;
OneWire_High;
OneWire_Free;
menu = &MainMenu;
LCDInit();
MenuDisplay (menu, select);

 while (1)
{
 ReadKey;
 if (!Buttons.DOWN)
 {
 while (!Buttons.DOWN) ReadKey;
 select = (select==3)?1:select+1;
 MenuDisplay (menu, select);
 }
 if (!Buttons.UP)
 {
 while (!Buttons.UP) ReadKey;
 select = (select==1)?3:select-1;
 MenuDisplay (menu, select);
 }

 if (!Buttons.RIGHT)
 {
 while(!Buttons.RIGHT) ReadKey;
 switch (select)
 {
 case 1: menu= (menu->Menulist1 == NULL)? menu: menu->Menulist1;break;
 case 2: menu= (menu->Menulist2 == NULL)? menu: menu->Menulist2;break;
 case 3: menu= (menu->Menulist3 == NULL)? menu: menu->Menulist3;break;
 }
 select = 1;
 MenuDisplay (menu, select);
 } 
 if (!Buttons.LEFT)
 {
 while(!Buttons.LEFT) ReadKey;
 menu= (menu->pre == NULL)? menu: menu->pre;
 select = 1;
 MenuDisplay (menu, select);
 }
 if (!Buttons.GO)
{
    while(!Buttons.GO) ReadKey;
    switch (select) {
    case 1:
        if (menu == &TempMenu) {
            ShowTemperatureScreen();
            MenuDisplay(menu, select);
        }
        if (menu->ActivationON != NULL) {
            menu->ActivationON(menu->MenuID, ON);
        }
        break;

    case 2:
        if (menu->ActivationOFF != NULL) {
            menu->ActivationOFF(menu->MenuID, OFF);
        }
        break;
  }

}
}
    return 0;

}
