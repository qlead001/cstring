#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>

#include "str.h"

/* * * * * * * * * * * * * * *
 * Dynamically Sized Strings *
 * * * * * * * * * * * * * * */

/* Return an empty str with default capacity */
str newStr(void) {
    str s;

    s.ptr = malloc(STR_CAP_DEFAULT * sizeof(char));
    s.len = 0;
    s.cap = STR_CAP_DEFAULT;

    /* If ptr is null then malloc failed */
    if (s.ptr == NULL)
        strErr("Malloc failed in newStr.");
    else
        /* Null terminate the empty string */
        *(s.ptr) = '\0';

    return s;
}

/* Return a str from a string literal */
str strFrom(const char* s) {
    str sNew;

    int len = strlen(s);
    int cap = STR_CALC_CAP(len);

    char* ptr = malloc(cap * sizeof(char));

    /* If ptr is null then malloc failed */
    if (ptr == NULL)
        strErr("Malloc failed in strFrom.");
    else
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

        /* If ptr is null then realloc failed */
        if (s1->ptr == NULL)
            strErr("Realloc failed in append.");

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

        /* If ptr is null then realloc failed */
        if (s1->ptr == NULL)
            strErr("Realloc failed in appendStr.");

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

    /* If ptr is null then malloc failed */
    if (a.ptr == NULL)
        arrErr("Malloc failed in newStrArr.");

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

    /* Throw an error if s is null */
    if (s == NULL || s->ptr == NULL)
        arrErr("Cannot push a null str.");

    /* Resize array if necessary */
    if (newCap > arr->cap) {
        /* Allocate in blocks of STR_CAP_DEFAULT bytes */
        newCap = ARR_CALC_CAP(newCap);

        arr->ptr = realloc(arr->ptr, newCap * sizeof(str));

        /* If ptr is null then realloc failed */
        if (arr->ptr == NULL)
            arrErr("Realloc failed in push.");

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

    /* Throw error if arr is empty */
    if (index < 0)
        return arrErr("Cannot pop from empty array.");

    s = (arr->ptr)[index];

    /* Throw error if top of arr is null */
    if (s.ptr == NULL)
        arrErr("Popped str is null.");

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

/* * * * * * * *
 * Helper Code *
 * * * * * * * */

/* Print an error message msg to stderr and exit
 * with a failure status unless ERR_SILENT or
 * ARR_ERR_SILENT is defined. If one or both is
 * defined the first 63 bytes of the error message
 * is written to arrErrMsg and arrErrRaised is set to 1.
 *
 * Return Value:
 *      str with all fields set to 0
 */
str arrErr(const char* msg) {
#if defined(ERR_SILENT) || defined(ARR_ERR_SILENT)
    str s;

    s.ptr = NULL;
    s.len = 0;
    s.cap = 0;

    strncpy(arrErrMsg, msg, 63);
    arrErrMsg[63] = '\0';
    arrErrRaised = 1;

    return s;
#else
    fprintf(stderr, "Array Error: %s\n", msg);
    exit(EXIT_FAILURE);
#endif
}

/* Print an error message msg to stderr and exit
 * with a failure status unless ERR_SILENT or
 * STR_ERR_SILENT is defined. If one or both is
 * defined the first 63 bytes of the error message
 * is written to strErrMsg and strErrRaised is set to 1.
 *
 * Return Value:
 *      '\0'
 */
char strErr(const char* msg) {
#if defined(ERR_SILENT) || defined(STR_ERR_SILENT)
    strncpy(strErrMsg, msg, 63);
    strErrMsg[63] = '\0';
    strErrRaised = 1;

    return '\0';
#else
    fprintf(stderr, "String Error: %s\n", msg);
    exit(EXIT_FAILURE);
#endif
}
