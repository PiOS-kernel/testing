#include "../includes/_common.h"

void Service_Call_1(){
    serial_print("Service call 1");
}

extern void Service_Call_2(){
    serial_print("Service call 2");
}

extern void Service_Call_Default(){
    serial_print("Service call Default");
}