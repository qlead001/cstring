#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>

#include "str.h"

/* * * * * * * *
 * Helper Code *
 * * * * * * * */

/* Print an error message msg to stderr and exit
 * with a failure status
 */
str strErr(const char* msg) {
    fprintf(stderr, "String Error: %s\n", msg);
    exit(EXIT_FAILURE);
}

/* * * * * * * * * * * * * * *
 * Dynamically Sized Strings *
 * * * * * * * * * * * * * * */

/* Return an empty str with default capacity */
str newStr(void) {
    str s;

    s.ptr = malloc(STR_CAP_DEFAULT * sizeof(char));
    s.len = 0;
    s.cap = STR_CAP_DEFAULT;

    *(s.ptr) = '\0';

    return s;
}

/* Return a str from a string literal */
str strFrom(const char* s) {
    str sNew;

    int len = strlen(s);
    int cap = STR_CALC_CAP(len);

    char* ptr = malloc(cap);

    /* Copy s into ptr with null terminator */
    memcpy(ptr, s, (len + 1) * sizeof(char));

    sNew.ptr = ptr;
    sNew.len = len;
    sNew.cap = cap;

    return sNew;
}

/* Free ptr and set all fields to 0 */
void freeStr(str* s) {
    free(s->ptr);

    s->ptr = NULL;
    s->len = 0;
    s->cap = 0;
}

/* Append s2 to s1 */
void append(str* s1, str s2) {
    int newLen = s1->len + s2.len;
    int newCap = newLen + 1;

    /* Resize string if necessary */
    if (s1->cap < newCap) {
        /* Allocate in blocks of STR_CAP_DEFAULT bytes */
        newCap = STR_CALC_CAP(newCap);

        s1->ptr = realloc(s1->ptr, newCap * sizeof(char));

        s1->cap = newCap;
    }
    
    /* Append s2 to s1 with null terminator */
    memcpy(s1->ptr + s1->len, s2.ptr, (s2.len + 1) * sizeof(char));

    s1->len = newLen;
}

/* Append string literal s2 to s1 */
void appendStr(str* s1, const char* s2) {
    int s2Len = strlen(s2);
    int newLen = s1->len + s2Len;
    int newCap = newLen + 1;

    /* Resize string if necessary */
    if (s1->cap < newCap) {
        /* Allocate in blocks of STR_CAP_DEFAULT bytes */
        newCap = STR_CALC_CAP(newCap);

        s1->ptr = realloc(s1->ptr, newCap * sizeof(char));

        s1->cap = newCap;
    }
    
    /* Append s2 to s1 with null terminator */
    memcpy(s1->ptr + s1->len, s2, (s2Len + 1) * sizeof(char));

    s1->len = newLen;
}

/* Concatenate many strings separated by newlines
 *  Parameters:
 *      Pass as many str* as needed followed by NULL
 */
void concatln(str* s1, str* s2, ...) {
    str* s;

    va_list args;
    va_start(args, s2);

    appendStr(s1, "\n");
    append(s1, *s2);
    
    while ((s = va_arg(args, str*))) {
        appendStr(s1, "\n");
        append(s1, *s);
    }

    va_end(args);
}

/* * * * * * * * * * * * * * * * * * * *
 * Dynamically Sized Array of Strings  *
 * * * * * * * * * * * * * * * * * * * */

/* Return an empty strArr with default capacity */
strArr newStrArr(void) {
    strArr a;

    a.ptr = malloc(ARR_CAP_DEFAULT * sizeof(str));
    a.len = 0;
    a.cap = ARR_CAP_DEFAULT;

    return a;
}

/* Free all the str's in the array and set len to 0 */
void dumpArr(strArr* arr) {
    int i;

    for (i = 0; i < arr->len; i++)
        freeStr(&((arr->ptr)[i]));

    arr->len = 0;
}

/* Dump arr, free ptr and set all fields to 0 */
void freeStrArr(strArr* arr) {
    dumpArr(arr);

    free(arr->ptr);

    arr->ptr = NULL;
    arr->cap = 0;
}

/* Push s onto arr and clear fields of s 
 *
 * This function takes ownership of s so
 * it cannot be used after pushing.
 */
void push(strArr* arr, str* s) {
    int newCap = arr->len + 1;

    /* Resize array if necessary */
    if (newCap > arr->cap) {
        /* Allocate in blocks of STR_CAP_DEFAULT bytes */
        newCap = ARR_CALC_CAP(newCap);

        arr->ptr = realloc(arr->ptr, newCap * sizeof(str));

        arr->cap = newCap;
    }

    /* Push s to end of arr and increase length */
    (arr->ptr)[arr->len] = *s;
    (arr->len)++;

    /* Clear s */
    s->ptr = NULL;
    s->len = 0;
    s->cap = 0;
}

/* Pop the top of arr and ownership of the
 * str is returned to the caller
 *
 * If the str on the top of arr is no
 * longer needed then free the returned str.
 *      free(pop(&arr).ptr);
 * Or use the macro popFree
 */
str pop(strArr* arr) {
    str s;
    int index = arr->len - 1;

    if (index < 0) {
        s.ptr = NULL;
        s.len = 0;
        s.cap = 0;
        return s;
    }

    s = (arr->ptr)[index];

    if (s.ptr == NULL)
        strErr("Popped str is null.");

    /* Clear str that was the top of arr */
    ((arr->ptr)[index]).ptr = NULL;
    ((arr->ptr)[index]).len = 0;
    ((arr->ptr)[index]).cap = 0;

    arr->len = index;

    return s;
}

/* Check if arr contains a str matching s
 *  Return Values:
 *      -1      arr does not contain s
 *      0..len  index into arr of the first str
 *              that matches s
 */
int contains(strArr arr, str s) {
    int i;

    for (i = 0; i < arr.len; i++)
        if (STRCMP((arr.ptr)[i], s) == 0)
            return i;

    return -1;
}

/* Check if arr contains a str representing s
 *  Return Values:
 *      -1      arr does not contain s
 *      0..len  index into arr of the first str
 *              whose ptr matches s
 */
int containsStr(strArr arr, const char* s) {
    int i;

    for (i = 0; i < arr.len; i++)
        if (strcmp(((arr.ptr)[i]).ptr, s) == 0)
            return i;

    return -1;
}
