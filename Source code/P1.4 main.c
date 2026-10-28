#include <xc.h>
#include "userdef.h"
#define F_CPU 1000000
#include<util/delay.h>
void main(void)
{
unsigned char select = 1;
Menu *menu;   
MCUCR |=0x80;
XMCRA = 0;
DDRG =0x1f;

/* Chỉ bật relay RL2 */
//Relays.relayall = 0x00;
//Relays.relay.RL2 = 1;
//RELAY_ACTIVATION;

/* Bật RL0, RL2, RL4, RL6 */
//Relays.relayall = 0x55;
//RELAY_ACTIVATION;

/* Bật đồng thời RL0 và RL5 */
Relays.relayall = 0x00;
Relays.relay.RL0 = 1;
Relays.relay.RL5 = 1;
RELAY_ACTIVATION;

return 0;

}
