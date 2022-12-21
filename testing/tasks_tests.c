#include "tests.h"
#include "../pios-kernel/kernel/task/task.h"
#include "../pios-kernel/kernel/heap/malloc.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "../includes/tools.h"

bool test_queue_empty() {
    Queue q;
    Queue_init(&q);
    ASSERT(empty(&q));
    ASSERT(count_tasks(&q) == 0);
    return true;
}

bool test_enqueue() {
    Queue q;
    Queue_init(&q);
    for (int i=0; i<5; i++) {
        TaskTCB* task = (TaskTCB*) malloc(sizeof(TaskTCB));

        enqueue(&q, task);
        ASSERT(!empty(&q));
        ASSERT(count_tasks(&q) == i + 1);
    }
    ASSERT(count_tasks(&q) == 5);

    // Memory claimed by the tasks TCBs is freed
    for (int i=0; i<5; i++) {
        TaskTCB* task = dequeue(&q);
        free(task);
    }

    ASSERT(available_space(&HEAP) == HEAP_SIZE);
    return true;
}

bool test_dequeue() {
    Queue q;
    Queue_init(&q);
    for (int i=0; i<5; i++) {
        TaskTCB* task = (TaskTCB*) malloc(sizeof(TaskTCB));
        TaskTCB_init(task, i);

        enqueue(&q, task);
        ASSERT(!empty(&q));
        ASSERT(count_tasks(&q) == i + 1);
    }
    for (int i=0; i<5; i++) {
        TaskTCB* task = dequeue(&q);
        ASSERT(task->priority == i);
        free(task);
    }
    
    ASSERT(empty(&q));
    ASSERT(available_space(&HEAP) == HEAP_SIZE);
    return true;
}

bool test_stack_push() {
    TaskTCB task;
    TaskTCB_init(&task, 0);
    uint8_t buffer[5] = {1, 2, 3, 4, 5};
    uint8_t* old_stp = task.stp;
    stack_push(&task, buffer, 5);

    for (int i=0; i<5; i++) {
        ASSERT(task.stp[i] == buffer[i]);
    }

    ASSERT(task.stp == old_stp - 5);
    return true;
}

bool test_stack_start() {
    TaskTCB task;
    TaskTCB_init(&task, 0);
    uint8_t* start = stack_start(&task);
    ASSERT(start == task.stack);
    return true;
}

bool test_stack_end() {
    TaskTCB task;
    TaskTCB_init(&task, 0);
    uint8_t* end = stack_end(&task);
    ASSERT(end == task.stack + STACK_SIZE);
    return true;
}