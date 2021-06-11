#include<stdlib.h>
#include<stdarg.h>
#include<stdio.h>
#include "str.h"

#include "tests.h"

/* Helper function to generate an array
 *  Parameters:
 *      Pass as many char* as needed followed by NULL
 * Only use after testing is done for:
 *  - newStrArr
 *  - push
 */
strArr gen_test_arr(const char* s1, ...) {
    char* c;
    str s;
    strArr arr;

    va_list args;
    va_start(args, s1);

    DISABLE_DEBUG();

    arr = newStrArr();

    s = strFrom(s1);
    push(&arr, &s);
    
    while ((c = va_arg(args, char*))) {
        s = strFrom(c);
        push(&arr, &s);
    }

    va_end(args);

    REVERT_DEBUG();

    return arr;
}

void new_test(void) {
    strArr arr;

    test("New");

    arr = newStrArr();

    if (ARRLEN(arr) != 0)
        test_abort("Empty array has length greater than 0.");
    else if (ARRCAP(arr) != ARR_CAP_DEFAULT)
        test_abort("Empty array capacity is not the default.");
    else
        test_pass();

    freeStrArr(&arr);
}


void push_test(void) {
#define ITERS   5
    strArr arr;
    str s;

    char c[4];
    int i;

    test("Push");

    c[3] = '\0';

    arr = newStrArr();

    for (i = 0; i < ITERS; i++) {
        c[0] = 'a' + i*3;
        c[1] = 'b' + i*3;
        c[2] = 'c' + i*3;
        
        s = strFrom(c);
        push(&arr, &s);

        if (strcmp(STRSTR(ARR(arr)[i]), c))
            test_abort("Push did not push the str.");
        else if (STRSTR(s) != NULL || STRLEN(s) != 0 || STRCAP(s) != 0)
            test_fail("Push did not clear input string.");
        else if (ARRLEN(arr) != i+1)
            test_fail("Push did not update length.");
        else if (ARRCAP(arr) != ARR_CAP_DEFAULT)
            test_fail("Push changed the capacity unnecessarily.");
        else
            continue;
        break;
    }

    if (i == ITERS) test_pass();

    freeStrArr(&arr);
#undef  ITERS
}

void push_null_test(void) {
    str s;
    strArr arr = gen_test_arr("test", "www", "foo", NULL);

    test("Push Null");

    s.ptr = NULL;
    s.len = 0;
    s.cap = 0;

    push(&arr, &s);

    if (!arrErrRaised)
        test_fail("Pushing an empty str did not raise an error.");
    else {
        arrErrRaised = 0;
        test_pass();
    }

    freeStrArr(&arr);
}

void push_many_test(void) {
    str s;
    strArr arr = gen_test_arr("abc", "123", "x", "test", "abc", "123", "x",
            "test", "abc", "123", "x", "test", "abc", "123", "x", "test", NULL);

    test("Push Many");

    if (strcmp(STRSTR(ARR(arr)[2]), "x"))
        test_fail("Push did not push correctly.");
    else if (ARRLEN(arr) != 16)
        test_fail("Push did not update length.");
    else if (ARRCAP(arr) != ARR_CAP_DEFAULT)
        test_fail("Push changed the capacity unnecessarily.");
    else {
        s = strFrom("9w9");
        push(&arr, &s);

        if (strcmp(STRSTR(ARR(arr)[16]), "9w9"))
            test_fail("Push failed on capacity boundary.");
        else if (STRSTR(s) != NULL || STRLEN(s) != 0 || STRCAP(s) != 0)
            test_fail("Push did not clear input string on capacity boundary.");
        else if (ARRLEN(arr) != 17)
            test_fail("Push did not update length on capacity boundary.");
        else if (ARRCAP(arr) != 2 * ARR_CAP_DEFAULT)
            test_fail("Push did not change the capacity correctly.");
        else
            test_pass();
    }

    freeStrArr(&arr);
}

void dump_test(void) {
    int i;
    str s;
    strArr arr;

    test("Dump");

    arr = gen_test_arr("abc", "123", "xyz", NULL);
    dumpArr(&arr);

    for (i = 0; i < 3; i++) {
        s = ARR(arr)[i];

        if (STRSTR(s) != NULL || STRLEN(s) != 0 || STRCAP(s) != 0) {
            test_fail("Dump did not clear str's.");
            break;
        }
    }

    if (i == 3) {
        if (ARRLEN(arr) != 0)
            test_fail("Dump did not update length.");
        else if (ARRCAP(arr) != ARR_CAP_DEFAULT)
            test_fail("Dump changed the capacity unnecessarily.");
        else
            test_pass();
    }

    freeStrArr(&arr);
}

void free_test(void) {
    strArr arr;

    test("Free");

    arr = gen_test_arr("abc", "123", "xyz", NULL);
    freeStrArr(&arr);

    if (ARR(arr) != NULL)
        test_fail("Free did not set ptr to null.");
    else if (ARRCAP(arr) != 0)
        test_fail("Free did not set capacity to 0.");
    else
        test_pass();
}

void get_test(void) {
    strArr arr;

    test("Get");

    arr = gen_test_arr("abc", "123", "xyz", NULL);

    if (strcmp(STRSTR(GETSTR(arr, 0)), "abc"))
        test_fail("Get did not retrieve the first string.");
    else if (strcmp(STRSTR(GETSTR(arr, 1)), "123"))
        test_fail("Get did not retrieve the second string.");
    else if (strcmp(STRSTR(GETSTR(arr, 2)), "xyz"))
        test_fail("Get did not retrieve the third string.");
    else {
        GETSTR(arr, -1);

        if (!arrErrRaised)
            test_fail("Get did not raise an error on index -1.");
        else {
            arrErrRaised = 0;
            GETSTR(arr, ARRLEN(arr));

            if (!arrErrRaised)
                test_fail("Get did not raise an error on index = len.");
            else {
                arrErrRaised = 0;
                test_pass();
            }
        }
    }

    freeStrArr(&arr);
}

void peek_test(void) {
    str s1, s2;
    strArr arr;

    test("Peek");

    arr = gen_test_arr("abc", "123", "xyz", NULL);
    s1 = peek(arr);
    s2 = peek(arr);

    if (strcmp(STRSTR(s1), "xyz"))
        test_fail("Peek did not return the top of arr.");
    else if (STRCMP(s1, s2))
        test_fail("Peeking twice returned different values.");
    else if (STRCMP(GETSTR(arr, 2), s1))
        test_fail("Peek changed the value of the top of arr.");
    else {
        ARRLEN(arr) = 0;
        peek(arr);

        if (!arrErrRaised)
            test_fail("Peek did not raise an error on empty arr.");
        else {
            arrErrRaised = 0;
            test_pass();
        }

        ARRLEN(arr) = 3;
    }

    freeStrArr(&arr);
}

void pop_test(void) {
    str s;
    strArr arr;

    test("Pop");

    arr = gen_test_arr("abc", "123", "xyz", NULL);
    s = pop(&arr);

    if (strcmp(STRSTR(s), "xyz"))
        test_fail("Pop did not return the top of arr");
    else if (STRSTR(ARR(arr)[2]) != NULL || STRLEN(ARR(arr)[2]) != 0
            || STRCAP(ARR(arr)[2]) != 0)
        test_fail("Pop did not clear the str on the top of arr.");
    else if (ARRLEN(arr) != 2)
        test_fail("Pop did not decrease the length of arr.");
    else if (ARRCAP(arr) != ARR_CAP_DEFAULT)
        test_fail("Pop changed the capacity.");
    else {
        /* There is no good way to ensure that popFree correctly
         * frees the memory of the str that was popped. This call
         * to popFree is meant to ensure that it can at least be
         * compiled and run without any errors.
         */
        popFree(&arr);
        test_pass();
    }

    freeStr(&s);
    freeStrArr(&arr);
}

void pop_all_test(void) {
    str s;
    strArr arr;

    test("Pop All");

    arr = gen_test_arr("abc", "123", "xyz", NULL);

    while (ARRLEN(arr)) {
        s = pop(&arr);
        freeStr(&s);
    }

    pop(&arr);

    if (!arrErrRaised)
        test_fail("Pop did not raise an error when arr was empty.");
    else {
        arrErrRaised = 0;

        /* This is for testing, do not do this in real code */
        ARRLEN(arr) = 1;
        pop(&arr);

        if (!arrErrRaised)
            test_fail("Pop did not raise an error when str was null.");
        else {
            arrErrRaised = 0;
            test_pass();
        }
    }

    freeStrArr(&arr);
}

void contains_test(void) {
    strArr arr;
    str s1, s2, s3, s4;

    test("Contains");

    arr = gen_test_arr("abc", "123", "xyz", NULL);

    s1 = strFrom("abc");
    s2 = strFrom("xyz");
    s3 = strFrom("1234");
    s4 = strFrom("123");

    if (contains(arr, s1) != 0)
        test_fail("Contains failed to find first entry.");
    else if (contains(arr, s2) != 2)
        test_fail("Contains failed to find third/last entry.");
    else if (contains(arr, s3) != -1)
        test_fail("Contains did not return -1 for a str not in arr.");
    else if (contains(arr, s4) != 1)
        test_fail("Contains failed to find second entry.");
    else
        test_pass();

    freeStrArr(&arr);
    freeStr(&s1); freeStr(&s2); freeStr(&s3); freeStr(&s4);
}

void contains_literal_test(void) {
    strArr arr;

    test("Contains Literal");

    arr = gen_test_arr("abc", "123", "xyz", NULL);

    if (containsStr(arr, "abc") != 0)
        test_fail("ContainsStr failed to find first entry.");
    else if (containsStr(arr, "xyz") != 2)
        test_fail("ContainsStr failed to find third/last entry.");
    else if (containsStr(arr, "1234") != -1)
        test_fail("Contains did not return -1 for a str not in arr.");
    else if (containsStr(arr, "123") != 1)
        test_fail("Contains failed to find second entry.");
    else
        test_pass();

    freeStrArr(&arr);
}

#define NUM_TESTS   12

int main(void) {
    int i;

    void (*test_funcs[NUM_TESTS])(void) = {
        new_test, push_test, push_null_test, push_many_test, dump_test,
        free_test, get_test, peek_test, pop_test, pop_all_test, contains_test,
        contains_literal_test
    };

    init_tests("strArr unit");

    for (i = 0; i < NUM_TESTS; i++)
        (*test_funcs[i])();

    test_print_results();

    return (test_fails < 255) ? test_fails : 255;
}
