#include "tests.h"
#include "../includes/tools.h"
#include "../pios-kernel/kernel/kernel.h"
#include "../pios-kernel/kernel/exceptions.h"

// Set to true by the test tasks when the test is completed.
extern EventHandle test_completed_event;

// Producer consumer test
extern void test_producer_consumer();
extern void test_events();
extern void test_kill();
extern void test_idle_task();
extern void test_pipe();

Test tests[] = {
    {"test_producer_consumer", test_producer_consumer},
    {"test_events", test_events},
    {"test_kill", test_kill},
    {"test_idle_task", test_idle_task},
    {"test_pipe", test_pipe},
};

void tests_runner() {
    int tests_count = sizeof(tests)/sizeof(Test);

    int passed = 0;
    int failed = 0;
    serial_print("\nRunning integration tests...\n\n");

    for (int i=0; i<tests_count; ++i) {
        serial_print(tests[i].name);
        serial_print(" ... ");

        // Setup the test
        tests[i].test();
        event_wait(test_completed_event);

        // The test result is retrieved
        bool test_result;
        get_event_msg(test_completed_event, &test_result);
        if (test_result == 1) {
            serial_println("[ ok ]");
            passed++;
        } else {
            serial_println("[ failed ]");
            failed++;
        }
    }

    char test_passed[10];
    char test_failed[10];
    itoa(passed, test_passed);
    itoa(failed, test_failed);
    serial_print("\n\nTests passed: ");
    serial_print(test_passed);
    serial_print("\nTests failed: ");
    serial_println(test_failed);

    while(1);
}