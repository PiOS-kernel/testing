#include "tests.h"
#include "stdbool.h"
#include "../includes/_common.h"

// Heap tests
extern bool heap_init_test();
extern bool count_segments_test();
extern bool available_space_test();
extern bool heap_compaction_test();
extern bool test_allocate_segment();

// Tasks tests
extern bool test_queue_empty();
extern bool test_enqueue();
extern bool test_dequeue();
extern bool test_stack_push();
extern bool test_stack_start();
extern bool test_stack_end();

// Syscalls tests
extern bool test_create_task();
/*extern bool test_task_switch();*/

// Utility functions tests
extern bool test_memcpy();

typedef struct Test {
    char *name;
    bool (*test)();
} Test;

Test all_tests[] = {
    // Heap tests
    { "heap_init_test", heap_init_test },
    { "count_segments_test", count_segments_test },
    { "available_space_test", available_space_test },
    { "heap_compaction_test", heap_compaction_test },
    { "test_allocate_segment", test_allocate_segment },

    // Tasks tests
    { "test_queue_empty", test_queue_empty },
    { "test_enqueue", test_enqueue },
    { "test_dequeue", test_dequeue },
    { "test_stack_push", test_stack_push },
    { "test_stack_start", test_stack_start },
    { "test_stack_end", test_stack_end },

    // Syscalls tests
    { "test_create_task", test_create_task },
    /*{ "test_task_switch", test_task_switch },*/

    // Utility functions tests
    { "test_memcpy", test_memcpy }
};

void test_runner() {
    int tests = sizeof(all_tests) / sizeof(all_tests[0]);
    int passed = 0;
    int failed = 0;
    serial_print("Running tests...\n");
    for (int i = 0; i < tests; i++) {
        serial_print(all_tests[i].name);
        serial_print(" ... ");
        if (all_tests[i].test()) {
            serial_print("[ ok :) ]\n");
            passed++;
        } else {
            serial_print("[ failed :( ]\n");
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
}
