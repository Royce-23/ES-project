#include <stddef.h>
#include "userdef.h"
#ifndef F_CPU
#define F_CPU 1000000
#endif
#include <util/delay.h>
#include <avr/io.h>

unsigned long led = 0x55555555; // 01010101010101010101010101010101
int i;

///P1.3////////////////LED7Segment///////////////////// 
void led_display(unsigned long led) {
    LED0 = led & 0x000000FF;
    LED1 = (led >> 8) & 0x000000FF;
    LED2 = (led >> 16) & 0x000000FF;
    LED3 = (led >> 24) & 0x000000FF; }

///P1.3a///LED 7 ĐOẠN (phương pháp nối trực tiếp)//////
void SevenSegDisplay(unsigned int value) {
    LED7D1 = SegCode[value%10];               //số hàng đơn vị
    LED7D2 = SegCode[(value/10)%10];         //số hàng chục
    LED7D3 = SegCode[(value/100)%10%10];    //số hàng trăm
    LED7D4 = SegCode[(value/1000)%10%10];  //số hàng nghìn
}

///P1.3b///LED 7 ĐOẠN (phương pháp quét)//////
void SevenSegDisplaySCAN(unsigned int value) {
    LED7IDDATE = SegCode[value%10];  
    LED7IDCTR = 0b11111110;    //mở LED hàng đơn vị
    _delay_ms(50);
    LED7IDCTR = 0xff;    //tắt LED hàng đơn vị
    LED7IDDATE = SegCode[(value/10)%10];  
    LED7IDCTR = 0b11111101;    //mở LED hàng chục
    _delay_ms(50);
    LED7IDCTR = 0xff;    //tắt LED hàng chục
    LED7IDDATE = SegCode[(value/100)%10%10];   
    LED7IDCTR = 0b11111011;    //mở LED hàng trăm
    _delay_ms(50);
    LED7IDCTR = 0xff;    //tắt LED hàng trăm
    LED7IDDATE = SegCode[(value/1000)%10%10];  
    LED7IDCTR = 0b11110111;    //mở LED hàng nghìn
    _delay_ms(50);
    LED7IDCTR = 0xff;          //tắt LED hàng nghìn
}

int main(void){
    MCUCR |= 0x80;   // External memory wait states ON, 1 wait state

// ////TEST//////
//    DDRB = 0xFF;
//    p = 0x1100;
//    *p = 0x55;

SevenSegDisplay(3214);    //hiển thị số 3214 lên LED 7 đoạn (PP Trực tiếp)

while (1) SevenSegDisplaySCAN(3214);    //hiển thị số 3214 lên LED 7 đoạn (PP Quét)
                                       //cho while(1) vào để lặp

}
