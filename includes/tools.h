#ifndef __TOOLS_H__
#define __TOOLS_H__

/**
 * Debugging tools
*/

void PUT32 ( unsigned int, unsigned int );
#define UART0BASE 0x4000C000

void serial_print(char* string_to_print);

#endif