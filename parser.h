#ifndef PARSER_INCLUDED
#define PARSER_INCLUDED

#include"misc.h"
#include"init.h"

#define PS_NUM (0) // constants (variables, places, numbers)
#define PS_VAR (1) // constant values (variables, places)
#define PS_VAL (2) // stack values (arguments)
#define PS_BLN (3) // built-in (functions, _)
#define PS_PLC (4) // real places (not references to places)
#define PS_COM (5) // ,

typedef struct p_node {
    int name, size;
    struct p_node *first, *last, *next;
    byte type;
} ParserNode;

typedef struct p_function {
    int name;
    ParserNode *first;
    struct p_function *next;
} ParserFunction;

typedef struct p_file {
    ParserFunction *first;
    int *data;
    int data_size;
    char **strings;
    int *str_vals;
    int str_size, str_max_size;
} ParserFile;

const char *built_in_name(int n);
void dbg_parser(ParserNode *node);
byte get_type(ParserNode *node);
byte is_func(ParserNode *node);
int parse_file(InitData *data, ParserFile *file);

#endif
