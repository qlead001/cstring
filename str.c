#include<stdlib.h>
#include<string.h>

#include "str.h"

/* Append s2 to s1 */
void append(str* s1, str s2) {
    int newLen = s1->len + s2.len;
    int newCap = newLen + 1;

    /* Resize string if necessary */
    if (s1->cap < newCap) {
        /* Allocate in blocks of STR_CAP_DEFAULT bytes */
        newCap = CALC_CAP(newCap);

        s1->ptr = realloc(s1->ptr, newCap);

        s1->cap = newCap;
    }
    
    /* Append s2 to s1 with null terminator */
    memcpy(s1->ptr + s1->len, s2.ptr, s2.len + 1);

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
        newCap = CALC_CAP(newCap);

        s1->ptr = realloc(s1->ptr, newCap);

        s1->cap = newCap;
    }
    
    /* Append s2 to s1 with null terminator */
    memcpy(s1->ptr + s1->len, s2, s2Len + 1);

    s1->len = newLen;
}

/* Return a str from a string literal */
str strFrom(const char* s) {
    str sNew;

    int len = strlen(s);
    int cap = CALC_CAP(len);

    char* ptr = malloc(cap);

    /* Copy s into ptr with null terminator */
    memcpy(ptr, s, len + 1);

    sNew.ptr = ptr;
    sNew.len = len;
    sNew.cap = cap;

    return sNew;
}

/* Return an empty str with default capacity */
str newStr(void) {
    str s;

    s.ptr = malloc(STR_CAP_DEFAULT);
    s.len = 0;
    s.cap = STR_CAP_DEFAULT;

    return s;
}

/* Free ptr and set all values to 0 */
void freeStr(str* s) {
    free(s->ptr);

    s->ptr = NULL;
    s->len = 0;
    s->cap = 0;
}
