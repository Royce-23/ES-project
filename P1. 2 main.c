#include <stddef.h>
#include "userdef.h"
#ifndef F_CPU
#define F_CPU 1000000
#endif
#include <util/delay.h>

unsigned long led = 0x55555555; // 01010101010101010101010101010101
int i;

//////TEST//////
//int main(void) {
//    DDRA = 0XFF;  //chọn chiều xuất
//    *(volatile unsigned char*)0x3B = 0X0F; //55 means 010101, trỏ ra đ/c 0x3b một giá trị gán giá trị 0x55    

// ///P1.2a///DỊCH 32 LED//////
// int main (void)  {
//    MCUCR |= 0x80; //cho phép truy cập vào ô nhớ ngoài
//    for (i=0; i<32; i++)
//    {
//        led = (unsigned long)1<<i;
//        LED0 = led&0x000000FF;
//        LED1 = (led>>8)&0x000000FF;
//        LED2 = (led>>16)&0x000000FF;
//        LED3 = (led>>24)&0x000000FF;  
//    _delay_ms(50);
//    }
//    while(1);
//    return 0;
// }

///P1.2b///DỊCH 32 LED VÒNG LẶP//////
void led_display(unsigned long led)
{   LED0 = led & 0x000000FF;
   LED1 = (led >> 8) & 0x000000FF;
   LED2 = (led >> 16) & 0x000000FF;
   LED3 = (led >> 24) & 0x000000FF; }
int main(void) {
   MCUCR |= 0x80;   // External memory wait states ON, 1 wait state
   while (1)
   {
       for (i = 0; i < 32; i++)
       {
           led = (unsigned long)1 << i;
           led_display(led);
           _delay_ms(50);
       }
   }
}