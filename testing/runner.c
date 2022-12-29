#include "integration.h"
#include "tests.h"
#include "../../includes/tools.h"

RunnerState runner_state = {0, 0, 0};
int scheduler_state = 0;
int scheduler_running = 0;
bool test_result = false; 
extern void summon_scheduler();

extern bool test_producer_consumer();

Test tests[] = {
    {"test_producer_consumer", test_producer_consumer},
};

void tests_runner() {
    while(1);
    int tests_count = sizeof(tests)/sizeof(Test);

    int passed = 0;
    int failed = 0;
    serial_print("\nRunning integration tests...\n");

    for (int i=0; i<tests_count; ++i) {
        // The kernel is initialized before each test to reset the heap
        // and the tasks queues.
        kernel_init();
        
        serial_print(tests[i].name);
        serial_print(" ... ");
        tests[i].test();
        summon_scheduler();

        if (test_result) {
            serial_println("[ ok ]");
            passed++;
        } else {
            serial_println("[ failed ]");
            failed++;
        }

        test_result = false;
    }

    char test_passed[10];
    char test_failed[10];
    itoa(passed, test_passed);
    itoa(failed, test_failed);
    serial_print("\n\nTests passed: ");
    serial_print(test_passed);
    serial_print("\nTests failed: ");
    serial_println(test_failed);
}