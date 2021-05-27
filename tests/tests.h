#ifndef _TESTS_H
#define _TESTS_H    1

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define CLR_RED "\x1b[0;31m"
#define CLR_GRN "\x1b[0;32m"
#define CLR_BLD "\x1b[1m"
#define CLR_RST "\x1b[0;0m"

const char* test_name;

int test_runs = 0;
int test_fails = 0;
int test_passes = 0;
int aborted = 0;

// Start a new test
void test(const char* name) {
    printf("Running %s test\n", name);

    test_name = name;
    test_runs++;
}

// Mark test as failure and optionally print message
void test_fail(const char* msg) {
    int msgLen = strlen(msg);
    int nameLen = strlen(test_name);

    if (msgLen > 0)
        printf(CLR_RED "Fail: %s\n" CLR_RST, msg);
    printf(CLR_RED "%s test failed\n" CLR_RST, test_name);

    test_fails++;

    // Calculate length of longest message
    int count = msgLen + 6;
    if (nameLen + 13 > count)
        count = nameLen + 13;
    else if (nameLen + 12 > count)
        count = nameLen + 12;

    int i;
    for(i = 0; i < count; i++)
        printf("-");
    printf("\n");
}

// Mark test as a success
void test_pass(void) {
    printf(CLR_GRN "%s test passed\n" CLR_RST, test_name);

    test_passes++;
}

// Print results of testing
void test_print_results(void) {
    printf("\n");
    if (test_passes == test_runs) {
        printf(CLR_GRN "All %d tests passed\n" CLR_RST, test_runs);
        return;
    }

    float passes = 100.0 * ((float) test_passes) / ((float) test_runs);

    if (test_fails >= test_passes)
        printf(CLR_RED);

    printf("%d out of %d (%.2f%%) tests passed\n",
            test_passes, test_runs, passes);
    printf(CLR_RST);

    if (aborted) {
        printf("\n" CLR_RED CLR_BLD);
        printf("Testing aborted due to %s test.\n", test_name);
        printf("Some tests may not have run.\n" CLR_RST);
    }
}

// End testing early and considers current test a failure
void test_abort(const char* msg) {
    aborted = 1;

    test_fail(msg);
    test_print_results();

    exit(-test_fails);
}

#endif  /* tests.h */