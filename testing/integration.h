#ifndef INTEGRATION_H
#define INTEGRATION_H

#include <stdint.h>
#include <stdbool.h>

typedef struct RunnerState {
    uint32_t lr;
    uint32_t sp;
    uint32_t xpsr;
} RunnerState;

extern RunnerState runner_state;
extern int scheduler_state;
extern int scheduler_running;
extern bool test_result;

#endif