#include <stddef.h>
#include "userdef.h"
#ifndef F_CPU
#define F_CPU 1000000
#endif
#include <util/delay.h>

unsigned long led = 0x55555555; // 01010101010101010101010101010101
int i;

void SegDisplayD(unsigned int value) {
    LED7D1 = SegCode[value % 10];               //số hàng đơn vị
    LED7D2 = SegCode[(value / 10) % 10];         //số hàng chục
    LED7D3 = SegCode[(value / 100) % 10];    //số hàng trăm
    LED7D4 = SegCode[(value / 1000) % 10];  //số hàng nghìn
}

///sub_P1.8///
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
PrintL (menu->Title ,0,0);
PrintL (menu->List1 ,1,0);
PrintL (menu->List2 ,2,0);
PrintL (menu->List3 ,3,0);
PrintL(">", select, 0);
}
///P1.8b///One-Wire DS18B20//////
/*One-wire interface*/
char OneWireReset(void) {
    char Presence;
    OneWire_Master;
    OneWire_Low;
    _delay_us(480);
    OneWire_High;
    OneWire_Free;
    _delay_us(80);
    Presence = OneWire_State;
    _delay_us(60);
    return Presence;
}
void OneWireWriteByte(unsigned char Byte) {
    unsigned char i;
    for (i=0; i<8; i++) {
        OneWire_Master;
        if (Byte&0x01) {
            OneWire_Low;
            _delay_us(6);
            OneWire_High;
            OneWire_Free;
            _delay_us(64);
        }
        else {
            OneWire_Low;
            _delay_us(60);
            OneWire_High;
            OneWire_Free;
            _delay_us(10);
        }
        Byte = Byte>>1;
    }
}
unsigned char OneWireReadByte(void) {
    unsigned char i;
    unsigned char Byte = 0;
    for (i=0; i<8; i++) {
        OneWire_Master;
        OneWire_Low;
        _delay_us(3);
        OneWire_High;
        OneWire_Free;
        _delay_us(10);
        Byte >>= 1;
        if (OneWire_State) {
            Byte |= 0x80;
        }
        _delay_us(50);
    }
    return Byte;
}

unsigned int ReadTemp(void) {
    unsigned char data [2];
    signed int rawTemperature;
    if (!OneWireReset()) {
        OneWireWriteByte(0xCC);
        OneWireWriteByte(0x44);
        OneWire_Free;
        _delay_ms(750);
        if (OneWireReset()) {
            return 0;
        }
        OneWireWriteByte(0xCC);
        OneWireWriteByte(0xBE);
        data[0] = OneWireReadByte();
        data[1] = OneWireReadByte();
        rawTemperature = (signed int)(((unsigned int)data[1] << 8) | data[0]);
        if (rawTemperature < 0) {
            return 0;
        }
        return (unsigned int)(rawTemperature / 16);
    }
    else 
       return 0;
}

int main(void) {
    unsigned int Temp;
    // unsigned char data; // Chưa sử dụng trong bài P1.8
    unsigned char select = 1;
    Menu *menu;
    MCUCR |= 0x80;
    XMCRA = 0;
    DDRG |= (1 << 3) | (1 << 4);

    menu = &MainMenu;
    LCDInit();
    PrintL("Test one-wire",0,0);
    
    while (1) {
        Temp = ReadTemp();
        SegDisplayD(Temp);
    }

    while (1);
    MenuDisplay(menu, select);

///sub_P1.8///
    while(1)
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
 MenuDisplay (menu, select);
 } 
 if (!Buttons.LEFT)
 {
 while(!Buttons.LEFT) ReadKey;
 menu= (menu->pre == NULL)? menu: menu->pre;
 MenuDisplay (menu, select);
 }
 if (!Buttons.GO)
{
    while(!Buttons.GO) ReadKey;
    switch (select) {
    case 1:
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
