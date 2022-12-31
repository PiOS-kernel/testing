#include <stdint.h>
#include <stdbool.h>
#include "../pios-kernel/kernel/kernel.h"
#include "../includes/tools.h"
#include "../pios-kernel/kernel/exceptions.h"

extern bool test_completed;
extern bool test_result;

typedef struct SharedData {
    uint32_t data;
    bool consumed;
} SharedData;

void producer_task(SharedData* data) {
    for (int i=0; i<10; ++i) {
        // waits for the consumer task to consume data
        while (!data->consumed)
            PendSVTrigger(); // yield

        data->data = i;
        data->consumed = false;
    }
    
    exit();
}

void consumer_task(SharedData* data) {
    for (int i=0; i<10; ++i) {
        // waits for the producer task to publish data
        while (data->consumed)
            PendSVTrigger(); // yield

        if (data->data != i) {
            test_result = false;
            test_completed = true;
            exit();
        }
        data->consumed = true;
    }

    // Publish the test result
    test_result = true;
    test_completed = true;
    
    exit();
}

void test_producer_consumer() {
    SharedData data = {0, true};
    // create the tasks
    kcreate_task((void(*)(void*)) producer_task, (void*)&data, 0);
    kcreate_task((void(*)(void*)) consumer_task, (void*)&data, 0);

    // The runner is stuck here until the test is completed
    while(!test_completed)
        PendSVTrigger();
}