#include<stdio.h>
#include "str.h"

#include "tests.h"

#define NUM_TESTS   1

void from_literal_test(void) {
    test("from literal");

    str s = strFrom("test");
    if (strcmp("test", STRSTR(s)))
        test_fail("String did not match literal.");
    else
        test_pass();
}

int main(void) {
    printf("------------------\n");
    printf("Running unit tests\n");
    printf("------------------\n");

    void (*test_funcs[NUM_TESTS])(void) = {from_literal_test};

    int i;
    for (i = 0; i < NUM_TESTS; i++)
        (*test_funcs[i])();

    test_print_results();

    return test_fails;
}
