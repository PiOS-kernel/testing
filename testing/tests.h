#ifndef TESTS_H
#define TESTS_H

// Assert is defined as a macro
#define ASSERT(expr) if (!(expr)) { return false; }

void test_runner();

#endif