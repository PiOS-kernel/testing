#include "../includes/_common.h"
#include <stdint.h>

//main - va avanti all'infinito
int main ( void )
{    
    //serial_print("before\n");
    //__asm__("SVC #01\n\t");
    serial_print("after\n");

    while(1);
    return(0);
}

void HardFaultISR()
{
	while(1);
}