#include "tests.h"
#include "../pios-kernel/kernel/kernel.h"

extern EventHandle test_completed_event;

void kill_test_task1(EventHandle event);
void kill_test_task2(EventHandle event);

void kill_test_task1(EventHandle event) {
    TaskHandle task2 = create_task((void(*)(void*)) kill_test_task2, event, 0);
    event_wait(event);

    uint32_t counter;
    get_event_msg(event, (void*) &counter);
    
    bool test_result = false;
    if (counter != 10) {
        event_post(test_completed_event, (void*) &test_result);
        kill(task2);
        exit();
    }

    // Task2 is killed
    kill(task2);
    // Task1 tries to wake up task 2. If kill succeded, Task1 should be
    // the only READY task and the scheduler should immediately return 
    // control to it.
    event_post(event, (void*) &counter);
    yield();

    test_result = counter == 10;
    event_post(test_completed_event, (void*) &test_result);
    exit();
}

void kill_test_task2(EventHandle event) {
    uint32_t counter = 0;
    for(; counter < 10; counter++);
    
    // Task1 is woken up and control is passed on to it
    event_post(event, (void*) &counter);
    event_wait(event);

    // This section should be unreachable
    get_event_msg(event, (void*) &counter);
    counter += 10;

    while(1);
}

void test_kill() {
    EventHandle event = NEW_EVENT(uint32_t);
    create_task((void(*)(void*)) kill_test_task1, event, 0);
}