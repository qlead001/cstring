#ifndef _STR_H
#define _STR_H  1

/* Set the default block size to grow strings by */
#ifndef STR_CAP_DEFAULT
#  define STR_CAP_DEFAULT   128
#endif

/* Calculate the size in bytes necessary to contain cap */
#define CALC_CAP(cap)   \
    ((((cap) / STR_CAP_DEFAULT) + 1) * STR_CAP_DEFAULT)

/* Macros for accessing fields of the str struct */
#define STRSTR(s)   ((s).ptr)
#define STRLEN(s)   ((s).len)
#define STRCAP(s)   ((s).cap)

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

struct str {
    char* ptr;
    int len;
    int cap;
};

typedef struct str str;

void append(str* s1, str s2);
void appendStr(str* s1, const char* s2);
str strFrom(const char* s);
str newStr(void);
void freeStr(str* s);

#endif  /* str.h */
