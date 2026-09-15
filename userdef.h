// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef XC_HEADER_TEMPLATE_H
#define XC_HEADER_TEMPLATE_H
#include <xc.h> // include processor files each processor file is guarded.
#include<stddef.h>
#ifdef __cplusplus
extern "C" (
#endif /*_ cplusplus */
// TODO If C++ is being used, regular C code needs function names to have C
// linkage so the functions can be used by the c code.
#ifdef __cplusplus
}
#endif /*_cplusplus */
#endif /* XC HEADER TEMPLATE H */

#define EXMEM_ADD 0x1100
#define porta (unsigned char *) 0x3B
#define EXMEM * (unsigned short *) (EXMEM_ADD+0)

//address table
#define CS0 0
#define CS1 1
#define CS2 2
#define CS3 3
#define CS4 4
#define CS5 5
#define CS6 6
#define CS7 7
#define CS8 8
#define CS9 9
#define CS10 10
#define CS11 11
#define CS12 12 
#define CS13 13
#define CS14 14
#define CS15 15
#define CS16 16
#define CS17 17
#define CS18 18
#define CS19 19

#define LCDE_H (PORTG |= (1<<3))
#define LCDE_L (PORTG &= ~(1<<3))
//define external device
#define LED0 *(unsigned char *) (EXMEM_ADD+CS0)
#define LED1 *(unsigned char *) (EXMEM_ADD+CS1)
#define LED2 *(unsigned char *) (EXMEM_ADD+CS2)
#define LED3 *(unsigned char *) (EXMEM_ADD+CS3)
#define LED7D1 *(unsigned char *) (EXMEM_ADD+CS4)
#define LED7D2 *(unsigned char *) (EXMEM_ADD+CS5)
#define LED7D3 *(unsigned char *) (EXMEM_ADD+CS6)
#define LED7D4 *(unsigned char *) (EXMEM_ADD+CS7)
#define LED7IDCTR *(unsigned char *) (EXMEM_ADD+CS8)
#define LED7IDDATE *(unsigned char *) (EXMEM_ADD+CS9)
#define RELAYMAP *(unsigned char *) (EXMEM_ADD+CS10)
#define LCD_DATA *(unsigned char *)(EXMEM_ADD + CS11)
#define LCD_INS  *(unsigned char *)(EXMEM_ADD + CS12)
#define MOTORDC *(unsigned char *) (EXMEM_ADD+CS13)
#define BUTTON *(unsigned char *) (EXMEM_ADD+CS14)
#define RELAY_ACTIVATION (*(volatile unsigned char *) (EXMEM_ADD+CS10)= *(unsigned char *) (&Relays))
#define MOTOR_ACTIVATION (*(volatile unsigned char *) (EXMEM_ADD+CS13)= *(unsigned char *) (&Motors))
#define ReadKey (*(unsigned char *) (&Buttons) = *(unsigned char *) (EXMEM_ADD+CS14))
//define struct
struct bits {
unsigned char RLO : 1 ;
unsigned char RL1 : 1 ;
unsigned char RL2 : 1 ;
unsigned char RL3 : 1 ;
unsigned char RL4 : 1 ;
unsigned char RLS : 1 ;
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
