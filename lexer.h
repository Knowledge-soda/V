#ifndef LEXER_INCLUDED
#define LEXER_INCLUDED

#include"misc.h"

#define AT_NUM (0)
#define AT_NAME (1)
#define AT_PLACE (2)
#define AT_BEGIN (3) // (
#define AT_SEP (4) // ,
#define AT_END (5) // )
#define AT_DEF (6) // :=
#define AT_DECLARE (7) // : at the beginning
#define AT_EOL (8) // end of effective line
#define AT_STR (9)

typedef struct atom {
    struct atom *next;
    byte type;
    int value;
    char *str;
} Atom;

int atomise_line(char *line, Atom *first);

#endif
