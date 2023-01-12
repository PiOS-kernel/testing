#include <stdint.h>
#include <stdbool.h>
#include "../pios-kernel/kernel/kernel.h"
#include "../includes/tools.h"
#include "../pios-kernel/kernel/exceptions.h"

extern EventHandle test_completed_event;

typedef struct SharedData {
    uint32_t data;
    bool consumed;
} SharedData;


void idle_task_code(void* _) {
    bool test_result = true;
    event_post(test_completed_event, &test_result);
    while(1);
}


void non_idle_task(EventHandle event) {
    event_wait(test_completed_event);
    task_exit();
}


void test_idle_task() {
    EventHandle event = new_event(0);

    // create the tasks
    create_task((void(*)(void*)) non_idle_task, (void*) event, 0, NULL);
}