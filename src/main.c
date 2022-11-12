#include "../includes/getChar.h"
#include <stdint.h>
void PUT32 ( unsigned int, unsigned int );
#define UART0BASE 0x4000C000

void serial_print(char* string_to_print);

//main - va avanti all'infinito
int main ( void )
{
    int myConstant = 65;
    serial_print("Hello world!");
    char c[2] = {myConstant, '\0'};
    serial_print(c);
    myConstant = 66;
    c[0] = myConstant;
    serial_print(c);
    return(0);
}

void serial_print(char* string_to_print){
    int i = 0;
    uint8_t c;
    i = 0;
    c = string_to_print[i];
    while(c != '\0'){
        PUT32(UART0BASE, c);
        ++i;
        c = string_to_print[i];
    }
    PUT32(UART0BASE, '\n');
    PUT32(UART0BASE, '\r');

}