#ifndef _STR_H
#define _STR_H  1

/* * * * * * * * * * * * * * *
 * Dynamically Sized Strings *
 * * * * * * * * * * * * * * */

/* Set the default block size to grow strings by */
#ifndef STR_CAP_DEFAULT
#  define STR_CAP_DEFAULT   128
#endif

/* Calculate the size in bytes necessary to contain cap */
#define STR_CALC_CAP(cap)   \
    ((((cap) / STR_CAP_DEFAULT) + 1) * STR_CAP_DEFAULT)

/* Macros for accessing fields of the str struct */
#define STRSTR(s)   ((s).ptr)
#define STRLEN(s)   ((s).len)
#define STRCAP(s)   ((s).cap)

/* Get the i-th char in str or throws an error if out of bounds
 *  Parameters:
 *      str s
 *      int i
 */
#define GETCHAR(s, i)   \
    ((i) < (s).len && (i) >= 0 ? \
     ((s).ptr)[(i)] : strErr(#i" is out of bounds"))

/* Append s2 to s1 then free s2
 *  Parameters:
 *      str* s1
 *      str* s2
 * Wrapped in single run do while for more safety
 */
#define appendFree(s1, s2)  do {\
    append((s1), *(s2)); \
    freeStr((s2)); \
    } while (0)

/* Compare strings s1 and s2
 *  Parameters:
 *      str s1
 *      str s2
 *  Return Value:
 *      <0  first non-matching char has a lower value in s1 than s2
 *      0   strings are equal
 *      >0  first non-matching char has a greater value in s1 than s2
 */
#define STRCMP(s1, s2)  \
    (memcmp(STRSTR((s1)), STRSTR((s2)), \
            (MIN(STRLEN((s1)), STRLEN((s2))) + 1) * sizeof(char)))

struct str {
    char* ptr;
    int len;
    int cap;
};

typedef struct str str;

str newStr(void);
str strFrom(const char* s);
void freeStr(str* s);

void append(str* s1, str s2);
void appendStr(str* s1, const char* s2);

void concatln(str* s1, str* s2, ...);

/* * * * * * * * * * * * * * * * * * * *
 * Dynamically Sized Array of Strings  *
 * * * * * * * * * * * * * * * * * * * */

/* Set the default block size to grow arrays by */
#ifndef ARR_CAP_DEFAULT
#  define ARR_CAP_DEFAULT   16
#endif

/* Calculate the size in bytes necessary to contain cap */
#define ARR_CALC_CAP(cap)   \
    ((((cap) / ARR_CAP_DEFAULT) + 1) * ARR_CAP_DEFAULT)

/* Macros for accessing fields of the strArr struct */
#define ARR(a)      ((a).ptr)
#define ARRLEN(a)   ((a).len)
#define ARRCAP(a)   ((a).cap)

/* Pop the top off arr and free the str
 *  Parameters:
 *      strArr* arr
 */
#define popFree(arr)    (free(pop((arr)).ptr))

/* Get the i-th str in arr or throws an error if out of bounds
 *  Parameters:
 *      strArr  arr
 *      int     i
 */
#define GETSTR(arr, i)  \
    ((i) < (arr).len && (i) >= 0 ? \
     ((arr).ptr)[(i)] : arrErr(#i" is out of bounds"))

/* Peek at the top of arr or throws an error if empty
 *  Parameters:
 *      strArr  arr
 */
#define peek(arr)   ((arr).len > 0 ? \
        ((arr).ptr)[(arr).len - 1] : arrErr("arr is empty"))

struct strArr {
    str* ptr;
    int len;
    int cap;
};

typedef struct strArr strArr;

strArr newStrArr(void);
void dumpArr(strArr* arr);
void freeStrArr(strArr* arr);

void push(strArr* arr, str* s);
str pop(strArr* arr);

int contains(strArr arr, str s);
int containsStr(strArr arr, const char* s);

/* * * * * * * *
 * Helper Code *
 * * * * * * * */

#define MIN(a, b)   ((a) < (b) ? (a) : (b))

/* If errors are silent store error message in errName */
#if defined(ERR_SILENT) || defined(ARR_ERR_SILENT) || defined(STR_ERR_SILENT)
#endif

/* If an error is set to silent provide a flag to indicate an error
 * and a 64 byte array to store error message */
#if defined(ERR_SILENT) || defined(ARR_ERR_SILENT)
int arrErrRaised;
char arrErrMsg[64];
str arrErr(const char* msg);
#else
str arrErr(const char* msg)  __attribute__ ((noreturn));
#endif

#if defined(ERR_SILENT) || defined(STR_ERR_SILENT)
int strErrRaised;
char strErrMsg[64];
char strErr(const char* msg);
#else
char strErr(const char* msg)  __attribute__ ((noreturn));
#endif

#endif  /* str.h */
