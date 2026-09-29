// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef XC_HEADER_TEMPLATE_H
#define XC_HEADER_TEMPLATE_H
#include <xc.h> // include processor files each processor file is guarded.
#include<stddef.h>
#ifdef __cplusplus
extern "C" {
#endif /*_ cplusplus */
#ifdef __cplusplus
}
#endif /*_cplusplus */
#define EXMEM_ADD 0x1100
#define porta (unsigned char *) 0x3B
#define EXMEM *(volatile unsigned short *) (EXMEM_ADD+0)

//address table
#define XMEM_CS0 0
#define XMEM_CS1 1
#define XMEM_CS2 2
#define XMEM_CS3 3
#define XMEM_CS4 4  
#define XMEM_CS5 5
#define XMEM_CS6 6
#define XMEM_CS7 7
#define XMEM_CS8 8
#define XMEM_CS9 9
#define XMEM_CS10 10
#define XMEM_CS11 11
#define XMEM_CS12 12
#define XMEM_CS13 13
#define XMEM_CS14 14
#define XMEM_CS15 15
#define XMEM_CS16 16
#define XMEM_CS17 17
#define XMEM_CS18 18
#define XMEM_CS19 19

#define LCDE_H (PORTG |= (1<<3))
#define LCDE_L (PORTG &= ~(1<<3))
#define LCDRS_H (PORTG |= (1<<4))
#define LCDRS_L (PORTG &= ~(1<<4))
//define external device
#define LED0 *(volatile unsigned char *) (EXMEM_ADD+XMEM_CS0)
#define LED1 *(volatile unsigned char *) (EXMEM_ADD+XMEM_CS1)
#define LED2 *(volatile unsigned char *) (EXMEM_ADD+XMEM_CS2)
#define LED3 *(volatile unsigned char *) (EXMEM_ADD+XMEM_CS3)
#define LED7D1 *(volatile unsigned char *) (EXMEM_ADD+XMEM_CS4)
#define LED7D2 *(volatile unsigned char *) (EXMEM_ADD+XMEM_CS5)
#define LED7D3 *(volatile unsigned char *) (EXMEM_ADD+XMEM_CS6)
#define LED7D4 *(volatile unsigned char *) (EXMEM_ADD+XMEM_CS7)
#define LED7IDCTR *(volatile unsigned char *) (EXMEM_ADD+XMEM_CS8)
#define LED7IDDATE *(volatile unsigned char *) (EXMEM_ADD+XMEM_CS9)
#define RELAYMAP *(volatile unsigned char *) (EXMEM_ADD+XMEM_CS10)
#define LCD_DATA *(volatile unsigned char *) (EXMEM_ADD+XMEM_CS11)
#define LCD_INS LCD_DATA
#define MOTORDC *(volatile unsigned char *) (EXMEM_ADD+XMEM_CS13)
#define BUTTON *(volatile unsigned char *) (EXMEM_ADD+XMEM_CS14)
#define RELAY_ACTIVATION (*(volatile unsigned char *) (EXMEM_ADD+XMEM_CS10)= *(unsigned char *) (&Relays))
#define MOTOR_ACTIVATION (*(volatile unsigned char *) (EXMEM_ADD+XMEM_CS13)= *(unsigned char *) (&Motors))
#define ReadKey (*(unsigned char *) (&Buttons) = *(volatile unsigned char *) (EXMEM_ADD+XMEM_CS14))

// Bảng mã LED 7 đoạn: 0, 1, ..., 9, A, B, C, D, E, F
unsigned char SegCode[] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};

// DS18B20 nối với PB0
#define DS18B20 0
#define OneWire_High (PORTB |= (1 << DS18B20))
#define OneWire_Low  (PORTB &= ~(1 << DS18B20))
#define OneWire_Master (DDRB |= (1 << DS18B20))
#define OneWire_Free   (DDRB &= ~(1 << DS18B20))
#define OneWire_State  (PINB & (1 << DS18B20))

//define struct
struct bits {
unsigned char RL0 : 1 ;
unsigned char RL1 : 1 ;
unsigned char RL2 : 1 ;
unsigned char RL3 : 1 ;
unsigned char RL4 : 1 ;
unsigned char RL5 : 1 ;
unsigned char RL6 : 1 ;
unsigned char RL7 : 1 ;
};
#define STOP 0
#define FORWARD 1
#define REWARD 2
union RL {
unsigned char relayall;
struct bits relay;
};
union RL Relays;
struct Motor {
unsigned char DC1: 2;
unsigned char DC2: 2;
unsigned char DCFree: 4;
};
struct Motor Motors;
struct Button{
    
unsigned char LEFT: 1;
unsigned char RIGHT: 1;
unsigned char UP: 1;
unsigned char DOWN: 1 ;
unsigned char GO: 1;
unsigned char BTN1: 1;
unsigned char BTN2: 1;
unsigned char BTN3 : 1;
};
struct Button Buttons;

#define ON   1
#define OFF  0

#define Device_Relay  0
#define Device_Motor1 1 
#define Device_Motor2 2
#define Device_LED    3

void ActuatorsActivation (char Devices, char Status)
{
switch (Devices) {
case Device_Relay: if (Status) Relays.relayall = 0xff;
else Relays.relayall = 0x00;
RELAY_ACTIVATION;
break;
case Device_Motor1: Motors. DC1 = Status; MOTOR_ACTIVATION;
break;
case Device_Motor2: Motors. DC2 = Status; MOTOR_ACTIVATION;
break;
case Device_LED: if (Status==ON) LED0 =0xff;
else LED0 = 0x00;
break;
}
}




typedef struct Linker {
char MenuID ;    
struct Linker *pre;
char Title [20];
char List1 [20];
struct Linker *Menulist1; void (*ActivationON) (char, char);
char List2 [20];
struct Linker *Menulist2; void (*ActivationOFF) (char, char);
char List3 [20];
struct Linker *Menulist3;
} Menu ;
Menu MainMenu, SensorMenu, ActuatorMenu, SettingMenu, TempMenu, HumidityMenu, DateTimeMenu,  MotorMenu, RelayMenu,
 LedMenu, Motor1Menu, Motor2Menu;

Menu MainMenu = {
0,
NULL,
"   MAIN MENU   ",
"   Sensors     " ,&SensorMenu,NULL,
"   Actuator    ",&ActuatorMenu,NULL,
"   Setting     ",&SettingMenu,
};

Menu SensorMenu = {

0,
    &MainMenu,
"   SENSORS       ",
"   Temperature   ",&TempMenu,NULL,
"   Humidity      ",&HumidityMenu,NULL,
"   Date & Time   ",&DateTimeMenu,
};

Menu TempMenu = {
    
    0,
&SensorMenu,
"   Temperature   ",
"   ON            ",NULL,NULL,
"   OFF           ",NULL,NULL,
"   SET UP        ",NULL,
};

Menu HumidityMenu = {
    0,
&SensorMenu,
"   Humidity      ",
"   ON            ",NULL,NULL,
"   OFF           ",NULL,NULL,
"   SET UP        ",NULL,
};

Menu DateTimeMenu = {
0,
&SensorMenu,
"   Date & Time   ",
"   ON            ",NULL,NULL,
"   OFF           ",NULL,NULL,
"   SET UP        ",NULL,
};

Menu ActuatorMenu = {
    0,
&MainMenu,
"   ACTUATORS   ",
"   Motors      ",&MotorMenu,NULL,
"   Relays      ",&RelayMenu,NULL,
"   Leds     ",&LedMenu,
};

Menu MotorMenu = {
    0,
&ActuatorMenu,
"   MOTORS      ",
"   Motor 1     ",&Motor1Menu,NULL,
"   Motor 2     ",&Motor2Menu,NULL,
"   None        ",NULL,
};
 
Menu Motor1Menu = {
    Device_Motor1,
&MotorMenu,
"   Motor 1       ",
"   ON            ",NULL,&ActuatorsActivation,
"   OFF           ",NULL,&ActuatorsActivation,
"   SET UP        ",NULL,
};

Menu Motor2Menu = {
    Device_Motor2,
&MotorMenu,
"   Motor 2       ",
"   ON            ",NULL,&ActuatorsActivation,
"   OFF           ",NULL,&ActuatorsActivation,
"   SET UP        ",NULL,
};


Menu RelayMenu = {
    Device_Relay,
&ActuatorMenu,
"   Relays        ",
"   ON            ",NULL,&ActuatorsActivation,
"   OFF           ",NULL,&ActuatorsActivation,
"   SET UP        ",NULL,
};

Menu LedMenu = {
    Device_LED,
&ActuatorMenu,
"   Leds          ",
"   ON            ",NULL,&ActuatorsActivation,
"   OFF           ",NULL,&ActuatorsActivation,
"   SET UP        ",NULL,
};


Menu SettingMenu = {
0,
&MainMenu,
"   SETTINGS    ",
"   Display     ",NULL,NULL,
"   Sound       ",NULL,NULL,
"   Network     ",NULL,
};

#endif /* XC_HEADER_TEMPLATE_H */
