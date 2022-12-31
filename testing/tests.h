#ifndef TESTS_H
#define TESTS_H

#include <stdbool.h>

// Assert is defined as a macro
#define ASSERT(expr) if (!(expr)) { return false; }

typedef struct Test {
    char *name;
    void (*test)();
} Test;

void tests_runner();

#endif