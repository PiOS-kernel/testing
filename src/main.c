#include "../includes/_common.h"
#include <stdint.h>
#include "../testing/tests.h"
#include "../pios-kernel/kernel/kernel.h"

int shared = 0;
MCB* my_mutex;
void task1(void *arg);
void task2(void *arg);
void task3(void *arg);
void task4(void *arg);
extern void PendSVTrigger();

void busy_wait(uint32_t n){
    for(int i = 0; i < n; i++);
}

void job(){
    shared++;
    serial_print("S->");
    serial_print_int(shared);
    serial_print("\n");
    //busy waiting
    //busy_wait(0xFFFFFF);
    //serial_print("Job done\n");
} 


void main ( void )
{    
    my_mutex = semaphore_init(2);
    create_task(task1, 0, 6);
    
}

void task1(void *arg)
{
    serial_print("T1 inside\n");
    char number[10];
    synch_wait(my_mutex);
    job();
    create_task(task2, 0, 5);
    PendSVTrigger();
    while(1);
    

}
void task2(void *arg)
{
    serial_print("T2 inside\n");
    char number[10];
    synch_wait(my_mutex);
    job();
    create_task(task3, 0, 4);
    PendSVTrigger();
    // when waken up, post the mutex
    synch_post(my_mutex);
    PendSVTrigger();
    while(1);

}
void task3(void *arg)
{
    serial_print("T3 inside\n");
    char number[10];
    synch_wait(my_mutex);
    job();
    //create_task(task4, 0, 1);
    PendSVTrigger();
    while(1);
    

}
void task4(void *arg)
{
    serial_print("T3 inside\n");
    char number[10];
    synch_wait(my_mutex);
    job();
    PendSVTrigger();
    while(1);

}

