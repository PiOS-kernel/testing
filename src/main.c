#include "../includes/_common.h"
#include <stdint.h>
#include "tests/tests.h"

//main - va avanti all'infinito
int main ( void )
{    
    test_runner();
    while(1);
    return(0);
}



void HardFaultISR()
{
    serial_print("HardFault, baby!\n");
	while(1);
}