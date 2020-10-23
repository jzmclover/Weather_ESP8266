#include "delay.h"
void delay_my_ms (u16 times)
{    
    u16 i=0;      
    while(times--)
    { 
        i=6000;
        while(i--) ;    
    }
}
