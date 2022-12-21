#include "tests.h"
#include "../pios-kernel/kernel/kernel.h"
#include "../includes/tools.h"

void mock_task(void *arg)
{
    serial_print("\nIt's me ");
    serial_println((char *)arg);
    
    while(1);
}

bool test_create_task() {
    char* name = "Mario!";
    create_task(mock_task, (void*) name, 0);

    ASSERT(count_tasks(&WAITING_QUEUES[0]) == 1);
    TaskTCB* created_task = dequeue(&WAITING_QUEUES[0]);
    ASSERT(created_task != NULL);

    // The first element on the task's stack should be the
    // pointer to its arguments
    uint32_t* stack_top = (uint32_t*) created_task->stp;
    ASSERT(*stack_top == (uint32_t) name);

    // The following 12 words should be 0-filled registers
    for (int i=0; i<12; i++) {
        stack_top += 1;
        ASSERT(*stack_top == 0);
    }

    // Finally we should find the link register pointing to the task's entry point
    stack_top += 1;
    ASSERT(*stack_top == (uint32_t) mock_task);
    free(created_task);
    return true;
}

bool test_task_switch() {
    char* name = "Mario!";
    kcreate_task(mock_task, (void*) name, 0);

    task_switch();
    asm volatile ("pop {pc}\n");

    return true;
}