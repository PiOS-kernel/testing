#include "../includes/getChar.h"
void PUT32 ( unsigned int, unsigned int );
#define UART0BASE 0x4000C000
int notmain ( void )
{
    unsigned int rx;
    for(rx=0;rx<8;rx++)
    {
        //PUT32(UART0BASE+0x00,0x30+(rx&7));
        PUT32(UART0BASE+0x00,getCharAHex());
    }
    //printf("Hello world!\n");
    return(0);
}
int Myexit(){
    PUT32(UART0BASE+0x00,'\n');
    PUT32(UART0BASE+0x00,'x');
    return(0);
}