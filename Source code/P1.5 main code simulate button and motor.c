#include <xc.h>
#include "userdef.h"

#define F_CPU 1000000UL
#include <util/delay.h>



void main(void)
{
    MCUCR |= 0x80;
    DDRG = 0x0F;


    Motors.DC1 = STOP;
    Motors.DC2 = STOP;
    MOTOR_ACTIVATION;

    while (1)
    {
        ReadKey;

        if (!Buttons.LEFT)
        {
            while (!Buttons.LEFT)
            {
                ReadKey;
            }

            Motors.DC2 = REWARD;
        }

        if (!Buttons.RIGHT)
        {
            while (!Buttons.RIGHT)
            {
                ReadKey;
            }

            Motors.DC2 = FORWARD;
        }

        MOTOR_ACTIVATION;
    }
}
