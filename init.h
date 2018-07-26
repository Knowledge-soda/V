#ifndef INIT_INCLUDED
#define INIT_INCLUDED

#include"misc.h"
#include"lexer.h"
#include"hashtable.h"

#define HS_VAR (0)
#define HS_VAL (1)
#define HS_SPC (2)

#define BL_NOP  (0)
#define BL_LAST (1)
#define BL_SUM  (2)
#define BL_MUL  (3)
#define BL_VAL  (4)
#define BL_MOV  (5)
#define BL_EQU  (6)
#define BL_MORE (7)
#define BL_LESS (8)
#define BL_IF   (9)
#define BL_IJMP (10)
#define BL_PRNT (11)
#define BL_LOAD (12)
#define BL_NOT  (13)
#define BL_AND  (14)
#define BL_OR   (15)
#define BL_SUB  (16)
#define BL_DIV  (17)
#define BL_MOD  (18)
#define BL_JMP  (19)
#define BL_STR  (20)
#define BL_IFC  (21)
#define BL_CALL (22)
#define BL_PRNR (23)
#define BL_IFEC (24)
#define BL_AT   (25)
#define BL_ATMV (26)
#define BL_INDX (27)
#define BL_PRNF (28)
#define BL_PRFR (29)
#define BL_FSUM (30)
#define BL_FMUL (31)
#define BL_FSUB (32)
#define BL_FDIV (33)
#define BL_LDF  (34)
#define BL_FEQU (35)
#define BL_FORE (36)
#define BL_FESS (37)


typedef struct line {
    char *name;
    int num;
    Atom *first, *last, *arg_first;
    struct line *next;
} Line;

typedef struct init_data_struct {
    Line *first, *last;
    int *allocs, *sizes;
    int var_num, alloc_num, max_alloc_num;
    HashTable *table;
} InitData;

int init_file(InitData *data, int maxline);

#endif
