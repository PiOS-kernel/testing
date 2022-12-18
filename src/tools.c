#include "../includes/tools.h"
#include <stdint.h>

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