#include<stdlib.h>
#include<stdio.h>
#include "str.h"

#include "tests.h"

/* Helper function to generate a long string of length len */
char* gen_long(int len) {
    int i;

    char* long_string = malloc(len * sizeof(char));

    for (i = 0; i < len-1; i++)
        long_string[i] = ' ' + (i % ('~' - ' ' + 1));
    long_string[len-1] = '\0';

    return long_string;
}

void from_literal_test(void) {
    str s;

    test("From Literal");

    s = strFrom("test");

    if (strcmp("test", STRSTR(s)))
        test_abort("String did not match literal.");
    else
        test_pass();

    freeStr(&s);
}

void from_long_string_test(void) {
    str s;

    char* long_string = gen_long(257);

    test("From Long String");

    s = strFrom(long_string);

    if (strcmp(long_string, STRSTR(s)))
        test_fail("String did not match literal.");
    else
        test_pass();

    freeStr(&s);
    free(long_string);
}

void free_test(void) {
    str s;

    test("Free");

    s = strFrom("test");
    freeStr(&s);

    if (STRSTR(s) != NULL)
        test_fail("String pointer is not null after free.");
    else if (STRLEN(s) != 0)
        test_fail("String length is not 0 after free.");
    else if (STRCAP(s) != 0)
        test_fail("String capacity is not 0 after free.");
    else
        test_pass();
}

void new_test(void) {
    str s;

    test("New");

    s = newStr();

    if (strcmp("", STRSTR(s)))
        test_abort("String did not match empty string.");
    else if (STRLEN(s) != 0)
        test_fail("Empty string has length greater than 0.");
    else if (STRCAP(s) != STR_CAP_DEFAULT)
        test_fail("Empty string capacity is not the default.");
    else
        test_pass();

    freeStr(&s);
}

void append_inplace_test(void) {
    str s1;
    str s2;

    test("Append In-Place");

    s1 = strFrom("test");
    s2 = strFrom(" foobar");
    append(&s1, s2);

    if (STRCAP(s1) != STR_CAP_DEFAULT)
        test_fail("Append changed the capacity unnecessarily.");
    else if (strcmp("test foobar", STRSTR(s1)))
        test_fail("Append did not perform correctly.");
    else if (strcmp(" foobar", STRSTR(s2)))
        test_fail("Append made changes to the second string.");
    else
        test_pass();

    freeStr(&s1);
    freeStr(&s2);
}

void append_long_test(void) {
    str s1;
    str s2;

    char* long_string = gen_long(STR_CAP_DEFAULT - 4);
    char expected[STR_CAP_DEFAULT + 25] = "test string";

    test("Append Long");

    strcat(expected, long_string);

    s1 = strFrom("test string");
    s2 = strFrom(long_string);
    append(&s1, s2);

    if (STRCAP(s1) != STR_CAP_DEFAULT * 2)
        test_fail("Append did not correctly increase capacity.");
    else if (strcmp(expected, STRSTR(s1)))
        test_fail("Append did not perform correctly.");
    else if (strcmp(long_string, STRSTR(s2)))
        test_fail("Append made changes to the second string.");
    else
        test_pass();

    freeStr(&s1);
    freeStr(&s2);
    free(long_string);
}

void append_literal_test(void) {
    str s1;

    test("Append Literal");

    s1 = strFrom("test");
    appendStr(&s1, " foobar");

    if (STRCAP(s1) != STR_CAP_DEFAULT)
        test_fail("Append changed the capacity unnecessarily.");
    else if (strcmp("test foobar", STRSTR(s1)))
        test_fail("Append did not perform correctly.");
    else
        test_pass();

    freeStr(&s1);
}

void append_literal_long_test(void) {
    str s1;

    char* long_string = gen_long(STR_CAP_DEFAULT - 4);
    char expected[STR_CAP_DEFAULT + 25] = "test string";

    test("Append Literal Long");

    strcat(expected, long_string);

    s1 = strFrom("test string");
    appendStr(&s1, long_string);
    if (STRCAP(s1) != STR_CAP_DEFAULT * 2)
        test_fail("Append did not correctly increase capacity.");
    else if (strcmp(expected, STRSTR(s1)))
        test_fail("Append did not perform correctly.");
    else
        test_pass();

    freeStr(&s1);
    free(long_string);
}

#define NUM_TESTS   8

int main(void) {
    int i;

    void (*test_funcs[NUM_TESTS])(void) = {
        from_literal_test, from_long_string_test, free_test, new_test,
        append_inplace_test, append_long_test, append_literal_test,
        append_literal_long_test
    };

    init_tests("unit");


    for (i = 0; i < NUM_TESTS; i++)
        (*test_funcs[i])();

    test_print_results();

    return (test_fails < 255)? test_fails : 255;
}
