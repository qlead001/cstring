#ifndef _STR_H
#define _STR_H  1

#ifndef STR_CAP_DEFAULT
#  define STR_CAP_DEFAULT   128
#endif

#define CALC_CAP(cap)   \
    ((((cap) / STR_CAP_DEFAULT) + 1) * STR_CAP_DEFAULT)

#define STRSTR(s)   ((s).ptr)
#define STRLEN(s)   ((s).len)
#define STRCAP(s)   ((s).cap)

struct str {
    char* ptr;
    int len;
    int cap;
};

typedef struct str str;

void append(str* s1, str s2);
str strFrom(const char* s);
str newStr(void);
void freeStr(str* s);

#endif  /* str.h */
