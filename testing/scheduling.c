#include <stdint.h>
#include <stdbool.h>
#include "../pios-kernel/kernel/kernel.h"
#include "../includes/tools.h"
#include "integration.h"

typedef struct PublishedData {
    uint32_t data;
    bool consumed;
} PublishedData;

bool producer_running = true;
bool consumer_running = true;
PublishedData data = {0, true};

void producer_task(PublishedData* data) {
    for (int i=0; i<100; ++i) {
        // waits for the consumer task to consume data
        while (!data->consumed);

        data->data = i;
        data->consumed = false;
    }
    
    producer_running = false;
    while(1); // the task waits forever
}

void consumer_task(PublishedData* data) {
    for (int i=0; i<100; ++i) {
        // waits for the producer task to publish data
        while (data->consumed);

        if (data->data != i) {
            test_result = false;
            while(1); // the task waits forever
        }
        data->consumed = true;
    }

    test_result = true;
    producer_running = false;
    while(1); // the task waits forever
}

// The task checks wether both the producer and the consumer are finshed and 
// deactivates the scheduler

void checker_task() {
    while (producer_running || consumer_running);

    // The scheduler is deactivated
    scheduler_state = 0;
}

bool test_producer_consumer() {
    // create the tasks
    kcreate_task(producer_task, (void*)&data, 0);
    kcreate_task(consumer_task, (void*)&data, 0);
    kcreate_task(checker_task, NULL, 0);
}