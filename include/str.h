#ifndef _STR_H
#define _STR_H  1

/* * * * * * * * * * * * * * *
 * Miscellaneous Definitions *
 * * * * * * * * * * * * * * */

#ifdef  ERR_SILENT
#   ifndef  ARR_ERR_SILENT
#       define  ARR_ERR_SILENT  ERR_SILENT
#   endif
#   ifndef  STR_ERR_SILENT
#       define  STR_ERR_SILENT  ERR_SILENT
#   endif
#endif

#ifdef  DEBUG
#   ifndef  ARR_DEBUG
#       define  ARR_DEBUG   DEBUG
#   endif
#   ifndef  STR_DEBUG
#       define  STR_DEBUG   DEBUG
#   endif
#endif

#define XSTR(s) STRINGIFY(s)
#define STRINGIFY(s)    #s

#define MIN(a, b)   ((a) < (b) ? (a) : (b))

#define BITPACK(bits, val, pos) ((bits) = (val) ? \
        ((bits) | (1<<(pos))) : ((bits) & ~(1<<(pos))))
#define BITUNPACK(bits, pos)   (((bits) & (1<<(pos))) >> (pos))

/* * * * * * * * * * * * * * *
 * Dynamically Sized Strings *
 * * * * * * * * * * * * * * */

/* Set the default block size to grow strings by */
#ifndef STR_CAP_DEFAULT
#   define STR_CAP_DEFAULT   128
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
 * Error Code  *
 * * * * * * * */

/* If an error is set to silent provide a flag to indicate an error
 * and a 64 byte array to store error message */

#if defined(ARR_ERR_SILENT) && ARR_ERR_SILENT
int arrErrRaised;
char arrErrMsg[64];
str arrErr(const char* msg);
#else
str arrErr(const char* msg)  __attribute__ ((noreturn));
#endif

#if defined(STR_ERR_SILENT) && STR_ERR_SILENT
int strErrRaised;
char strErrMsg[64];
char strErr(const char* msg);
#else
char strErr(const char* msg)  __attribute__ ((noreturn));
#endif

/* * * * * * * *
 * Debug Code  *
 * * * * * * * */

/* Default to yellow ANSI escape */
#ifndef DEBUG_START
#   define  DEBUG_START "\x1b[33m"
#endif

/* Default to reset ANSI escape */
#ifndef DEBUG_END
#   define  DEBUG_END   "\x1b[0m"
#endif

/* Default to stderr */
#ifndef ARR_DEBUG_FD
#   define  ARR_DEBUG_FD    stderr
#endif

#if defined(ARR_DEBUG) && ARR_DEBUG
#   define  ARR_DEBUG_OUT(msg) do { \
        dbg_temp_str = msg; \
        if (dbg_disabled) { free(dbg_temp_str.ptr); break; } \
        DISABLE_DEBUG(); \
        fputs(DEBUG_START, ARR_DEBUG_FD); \
        fputs(dbg_temp_str.ptr, ARR_DEBUG_FD); \
        fputs(DEBUG_END "\n", ARR_DEBUG_FD); \
        free(dbg_temp_str.ptr); \
        REVERT_DEBUG(); \
        } while(0)
#   define  ARR_STAT(arr)   do { \
        if (dbg_disabled) break; \
        DISABLE_DEBUG(); \
        dbg_temp_str = arrToStr((arr)); \
        fprintf(ARR_DEBUG_FD, "\t" DEBUG_START "Arr: %s, Len: %d, Cap: %d" \
                DEBUG_END "\n", dbg_temp_str.ptr, (arr).len, (arr).cap); \
        freeStr(&dbg_temp_str); \
        REVERT_DEBUG(); \
        } while(0)
#else
#   define  ARR_DEBUG_OUT(args) do {} while(0)
#   define  ARR_STAT(arr)       do {} while(0)
#endif

/* Default to stderr */
#ifndef STR_DEBUG_FD
#   define  STR_DEBUG_FD    stderr
#endif

#if defined(STR_DEBUG) && STR_DEBUG
#   define  STR_DEBUG_OUT(msg) do { \
        dbg_temp_str = msg; \
        if (dbg_disabled) { free(dbg_temp_str.ptr); break; } \
        DISABLE_DEBUG(); \
        fputs(DEBUG_START, STR_DEBUG_FD); \
        fputs(dbg_temp_str.ptr, STR_DEBUG_FD); \
        fputs(DEBUG_END "\n", STR_DEBUG_FD); \
        free(dbg_temp_str.ptr); \
        REVERT_DEBUG(); \
        } while(0)
#   define  STR_STAT(str)   do { \
        if (dbg_disabled) break; \
        DISABLE_DEBUG(); \
        fprintf(STR_DEBUG_FD, "\t" DEBUG_START "Str: \"%s\", Len: %d, Cap: %d" \
                DEBUG_END "\n", (str).ptr, (str).len, (str).cap); \
        REVERT_DEBUG(); \
        } while(0)
#else
#   define  STR_DEBUG_OUT(args) do {} while(0)
#   define  STR_STAT(str)       do {} while(0)
#endif

#if (defined(ARR_DEBUG) && ARR_DEBUG) || (defined(STR_DEBUG) && STR_DEBUG)
#   include<limits.h>

    /* Temporary storage for macros to use before freeing */
    str dbg_temp_str;

#   ifndef  DEBUG_STACK_LEN
#       define  DEBUG_STACK_LEN 32
#   endif

#   define  DBG_INDEX   ((dbg_curr / CHAR_BIT) + dbg_start) % DEBUG_STACK_LEN
    char dbg_disabled;
    char dbg_stack[DEBUG_STACK_LEN];
    int dbg_curr, dbg_start;

#   define  ENABLE_DEBUG()  do { \
        if (dbg_curr >= DEBUG_STACK_LEN * CHAR_BIT - 1) { \
            dbg_start = (dbg_start + 1) % DEBUG_STACK_LEN; \
        } else { \
            dbg_curr++; \
        } \
        BITPACK(dbg_stack[DBG_INDEX], dbg_disabled, dbg_curr % CHAR_BIT); \
        dbg_disabled = 0; \
        } while(0)
#   define  DISABLE_DEBUG() do { \
        if (dbg_curr >= DEBUG_STACK_LEN * CHAR_BIT - 1) { \
            dbg_start = (dbg_start + 1) % DEBUG_STACK_LEN; \
        } else { \
            dbg_curr++; \
        } \
        BITPACK(dbg_stack[DBG_INDEX], dbg_disabled, dbg_curr % CHAR_BIT); \
        dbg_disabled = 1; \
        } while(0)
#   define  REVERT_DEBUG()  do { \
        if (dbg_curr < 0) break; \
        dbg_disabled = BITUNPACK(dbg_stack[DBG_INDEX], dbg_curr % CHAR_BIT); \
        dbg_curr--; \
        } while(0)
#else
#   define  ENABLE_DEBUG()  do {} while(0)
#   define  DISABLE_DEBUG() do {} while(0)
#   define  REVERT_DEBUG()  do {} while(0)
#endif

str arrToStr(strArr arr);
str strprintf(const char *format, ...);

#endif  /* str.h */
