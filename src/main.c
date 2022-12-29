#include "../includes/_common.h"
#include <stdint.h>
#include "../testing/tests.h"
#include "../pios-kernel/kernel/kernel.h"

int shared = 0;
MCB* my_mutex;
void task1(void *arg);
void task2(void *arg);
extern void PendSVTrigger();

//main - va avanti all'infinito
void main ( void )
{    
    my_mutex = mutex_init();
    create_task(task1, 0, 0);
    create_task(task2, 0, 0);
}

void task1(void *arg)
{
    char number[10];
    while(1)
    {
        mutex_wait(my_mutex);
        shared++;
        if(shared > 10){
            while(1);
        }
        itoa(shared, number);
        serial_print("Task1: shared = ");
        serial_print(number);
        serial_print("\n");
        mutex_post(my_mutex);
        PendSVTrigger();
    }
}
void task2(void *arg)
{
    char number[10];
    while(1)
    {
        mutex_wait(my_mutex);
        shared++;
        itoa(shared, number);
        serial_print("Task2: shared = ");
        serial_print(number);
        serial_print("\n");
        mutex_post(my_mutex);
        PendSVTrigger();
    }
}

