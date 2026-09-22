#ifndef XC_HEADER_TEMPLATE_H
#define XC_HEADER_TEMPLATE_H
#include <xc.h> // include processor files each processor file is guarded.
#include<stddef.h>
#ifdef __cplusplus
extern "C" {
#endif /*_ cplusplus */
// TODO If C++ is being used, regular C code needs function names to have C
// linkage so the functions can be used by the c code.
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
