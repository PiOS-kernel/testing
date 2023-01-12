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

void producer_task(SharedData* data) {
    for (int i=0; i<10; ++i) {
        // waits for the consumer task to consume data
        while (!data->consumed)
            yield();

        data->data = i;
        data->consumed = false;
    }
    
    task_exit();
}

void consumer_task(SharedData* data) {
    uint32_t test_result = 0;

    for (int i=0; i<10; ++i) {
        // waits for the producer task to publish data
        while (data->consumed)
            yield();

        if (data->data != i) {
            event_post(test_completed_event, &test_result);
            task_exit();
        }
        data->consumed = true;
    }

    // The data necessary for the test is deallocated
    mem_free((uint8_t*) data, sizeof(SharedData));

    // Publish the test result
    test_result = 1;
    event_post(test_completed_event, &test_result);
    
    task_exit();
}

void test_producer_consumer() {
    SharedData* data = (SharedData*) alloc(sizeof(SharedData));
    data->consumed = true;
    data->data = 0;

    // create the tasks
    create_task((void(*)(void*)) producer_task, (void*)data, 0, NULL);
    create_task((void(*)(void*)) consumer_task, (void*)data, 0, NULL);
}