#ifndef TESTS_H
#define TESTS_H

// Assert is defined as a macro
#define ASSERT(expr) if (!(expr)) { return false; }

typedef struct Test {
    char *name;
    bool (*test)();
} Test;

void tests_runner();

#endif