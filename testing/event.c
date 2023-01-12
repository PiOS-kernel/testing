#include "tests.h"
#include "../pios-kernel/kernel/kernel.h"
#include <stdint.h>

extern EventHandle test_completed_event;

void events_test_task1(EventHandle event) {
    // Task 1 waits on event 1
    event_wait(event);

    uint32_t payload;
    get_event_msg(event, &payload);

    // The task expects the message payload to be 10.
    if (payload != 10) {
        event_post(event, &payload);
        task_exit();
    }

    for (int i=1; i<=10; ++i) {
        payload += 1;
        // The scheduler is called, but this task should be the only one
        // in the ready queue.
        yield();
    }

    event_post(event, &payload);
    task_exit();
}

void events_test_task2(EventHandle event) {
    uint32_t msg = 0;
    for (int i=1; i<=10; ++i) {
        msg += 1;
        // The scheduler is called, but this task should be the only one
        // in the ready queue.
        yield();
    }

    // Task 2 sends the message to task 1, and the waits to be woken
    // up by the event.
    event_post(event, &msg);
    event_wait(event);

    // The task expects the message payload to be 20.
    get_event_msg(event, &msg);

    // The memory allocated for the event object is reclaimed.
    delete_event(event);

    if (msg != 20) msg = 0;
    else msg = 1;

    event_post(test_completed_event, &msg);

    task_exit();
}

void test_events() {
    EventHandle event = NEW_EVENT(uint32_t);

    create_task((void(*)(void*)) events_test_task1, (void*)event, 0, NULL);
    create_task((void(*)(void*)) events_test_task2, (void*)event, 0, NULL);
}