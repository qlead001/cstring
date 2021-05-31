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

void compare_macro_test(void) {
    str s1;
    str s2;
    str s3;

    test("Compare Macro");

    s1 = strFrom("abc123");
    s2 = strFrom("abx123");
    s3 = strFrom("abc123");

    if (STRCMP(s1, s2) >= 0)
        test_fail("Compare macro did not return less than.");
    else if (STRCMP(s2, s1) <= 0)
        test_fail("Compare macro did not return greater than.");
    else if (STRCMP(s1, s3) != 0)
        test_fail("Compare macro did not return equal.");
    else {
        append(&s3, s2);
        if (STRCMP(s1, s3) >= 0)
            test_fail("Compare macro did not return "
                      "less than for shorter string.");
        else if (STRCMP(s3, s1) <= 0)
            test_fail("Compare macro did not return "
                      "greater than for longer string.");
        else if ((STRCMP(s2, s3) <= 0) || (STRCMP(s3, s2) >= 0))
            test_fail("Compare macro failed for "
                      "different length strings.");
        else
            test_pass();
    }

    freeStr(&s1);
    freeStr(&s2);
    freeStr(&s3);
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

void append_to_empty_test(void) {
    str s1;
    str s2;

    test("Append To Empty");

    s1 = newStr();
    s2 = strFrom("new string");
    append(&s1, s2);

    if (STRCAP(s1) != STR_CAP_DEFAULT)
        test_fail("Append changed the capacity unnecessarily.");
    else if (strcmp("new string", STRSTR(s1)))
        test_fail("Append did not perform correctly.");
    else if (strcmp("new string", STRSTR(s2)))
        test_fail("Append made changes to the second string.");
    else
        test_pass();

    freeStr(&s1);
    freeStr(&s2);
}

void append_free_test(void) {
    str s1;
    str s2;

    test("Append Free");

    s1 = strFrom("foo test");
    s2 = strFrom(" bar");
    appendFree(&s1, &s2);

    if (STRCAP(s1) != STR_CAP_DEFAULT)
        test_fail("Append changed the capacity unnecessarily.");
    else if (strcmp("foo test bar", STRSTR(s1)))
        test_fail("Append did not perform correctly.");
    else if (STRSTR(s2) != NULL)
        test_fail("String pointer is not null after free.");
    else if (STRLEN(s2) != 0)
        test_fail("String length is not 0 after free.");
    else if (STRCAP(s2) != 0)
        test_fail("String capacity is not 0 after free.");
    else
        test_pass();

    freeStr(&s1);
}

void concat_single_test(void) {
    str s1;
    str s2;

    test("Concat Single");

    s1 = strFrom("line 1");
    s2 = strFrom("line 2");
    concatln(&s1, &s2, NULL);

    if (STRCAP(s1) != STR_CAP_DEFAULT)
        test_fail("Concatln changed the capacity unnecessarily.");
    else if (strcmp("line 1\nline 2", STRSTR(s1)))
        test_fail("Concatln did not perform correctly.");
    else if (strcmp("line 2", STRSTR(s2)))
        test_fail("Second string was unnecessarily modified.");
    else
        test_pass();

    freeStr(&s1);
    freeStr(&s2);
}

void concat_multi_test(void) {
    str s1, s2, s3, s4;

    test("Concat Multiple");

    s1 = strFrom("line 1");
    s2 = strFrom("line 2");
    s3 = strFrom("line 3");
    s4 = strFrom("line 4");
    concatln(&s1, &s2, &s3, &s4, NULL);

    if (STRCAP(s1) != STR_CAP_DEFAULT)
        test_fail("Concatln changed the capacity unnecessarily.");
    else if (strcmp("line 1\nline 2\nline 3\nline 4", STRSTR(s1)))
        test_fail("Concatln did not perform correctly.");
    else if (strcmp("line 2", STRSTR(s2))
             || strcmp("line 3", STRSTR(s3))
             || strcmp("line 4", STRSTR(s4))
            )
        test_fail("A string other than the first was "
                  "unnecessarily modified.");
    else
        test_pass();

    freeStr(&s1);
    freeStr(&s2);
    freeStr(&s3);
    freeStr(&s4);
}

#define NUM_TESTS   13

int main(void) {
    int i;

    void (*test_funcs[NUM_TESTS])(void) = {
        from_literal_test, from_long_string_test, compare_macro_test,
        free_test, new_test, append_inplace_test, append_long_test,
        append_literal_test, append_literal_long_test, append_to_empty_test,
        append_free_test, concat_single_test, concat_multi_test
    };

    init_tests("str unit");

    for (i = 0; i < NUM_TESTS; i++)
        (*test_funcs[i])();

    test_print_results();

    return (test_fails < 255) ? test_fails : 255;
}
