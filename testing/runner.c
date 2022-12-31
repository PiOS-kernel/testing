#include "tests.h"
#include "msp.h"
#include "../includes/tools.h"
#include "../pios-kernel/kernel/kernel.h"
#include "../pios-kernel/kernel/exceptions.h"

// Set to true by the test tasks when the test is completed.
bool test_completed = false;
// Set to true by the test tasks if the test passes. 
bool test_result = false; 

// Producer consumer test
extern void test_producer_consumer();

Test tests[] = {
    {"test_producer_consumer", test_producer_consumer},
};

void tests_runner() {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    int tests_count = sizeof(tests)/sizeof(Test);

    int passed = 0;
    int failed = 0;
    serial_print("\nRunning integration tests...\n\n");

    int i=0;
    for (; i<tests_count; ++i) {
        serial_print(tests[i].name);
        serial_print(" ... ");

        // Setup the test
        tests[i].test();

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

    while(1);
}
