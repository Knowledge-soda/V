#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#include"init.h"
#include"tools.h"

typedef InitData Data;

static void *aloc_int(int n){
    int *num = malloc(sizeof(int));
    *num = n;
    return num;
}

static int init_built_in(HashTable *table){
    insert_node("id", HS_SPC, aloc_int(BL_NOP), table);
    insert_node("_", HS_SPC, aloc_int(BL_LAST), table);
    insert_node("+", HS_SPC, aloc_int(BL_SUM), table);
    insert_node("*", HS_SPC, aloc_int(BL_MUL), table);
    insert_node("v", HS_SPC, aloc_int(BL_VAL), table);
    insert_node("=", HS_SPC, aloc_int(BL_MOV), table);
    insert_node("==", HS_SPC, aloc_int(BL_EQU), table);
    insert_node(">", HS_SPC, aloc_int(BL_MORE), table);
    insert_node("<", HS_SPC, aloc_int(BL_LESS), table);
    insert_node("if", HS_SPC, aloc_int(BL_IF), table);
    insert_node("ijmp", HS_SPC, aloc_int(BL_IJMP), table);
    insert_node("print", HS_SPC, aloc_int(BL_PRNT), table);
    insert_node("load", HS_SPC, aloc_int(BL_LOAD), table);
    insert_node("not", HS_SPC, aloc_int(BL_NOT), table);
    insert_node("and", HS_SPC, aloc_int(BL_AND), table);
    insert_node("or", HS_SPC, aloc_int(BL_OR), table);
    insert_node("-", HS_SPC, aloc_int(BL_SUB), table);
    insert_node("/", HS_SPC, aloc_int(BL_DIV), table);
    insert_node("%", HS_SPC, aloc_int(BL_MOD), table);
    insert_node("jmp", HS_SPC, aloc_int(BL_JMP), table);
    insert_node("ifc", HS_SPC, aloc_int(BL_IFC), table);
    insert_node("call", HS_SPC, aloc_int(BL_CALL), table);
    insert_node("printr", HS_SPC, aloc_int(BL_PRNR), table);
    insert_node("ifec", HS_SPC, aloc_int(BL_IFEC), table);
    insert_node("@", HS_SPC, aloc_int(BL_AT), table);
    insert_node("@=", HS_SPC, aloc_int(BL_ATMV), table);
    insert_node("[]", HS_SPC, aloc_int(BL_INDX), table);
    insert_node("printf", HS_SPC, aloc_int(BL_PRNF), table);
    insert_node("printfr", HS_SPC, aloc_int(BL_PRFR), table);
    insert_node("f+", HS_SPC, aloc_int(BL_FSUM), table);
    insert_node("f*", HS_SPC, aloc_int(BL_FMUL), table);
    insert_node("f-", HS_SPC, aloc_int(BL_FSUB), table);
    insert_node("f/", HS_SPC, aloc_int(BL_FDIV), table);
    insert_node("loadf", HS_SPC, aloc_int(BL_LDF), table);
    insert_node("f==", HS_SPC, aloc_int(BL_FEQU), table);
    insert_node("f>", HS_SPC, aloc_int(BL_FORE), table);
    insert_node("f<", HS_SPC, aloc_int(BL_FESS), table);
    insert_node("int", HS_SPC, aloc_int(BL_ITF), table);
    insert_node("float", HS_SPC, aloc_int(BL_FTI), table);

    insert_node("_start", HS_VAR, aloc_int(-1), table);
    return NO_ERROR;
}

static int init_data(Data *data){
    Line *first;
    Atom *tmp;
    first = malloc(sizeof(Line));
    if (!first) return MEMORY_ERROR;
    tmp = malloc(sizeof(Atom));
    if (!tmp) return MEMORY_ERROR;
    tmp -> type = AT_NAME;
    tmp -> str = malloc(sizeof("id"));
    if (!tmp -> str) return MEMORY_ERROR;
    strcpy(tmp -> str, "id");
    first -> first = tmp;

    tmp = malloc(sizeof(Atom));
    if (!tmp) return MEMORY_ERROR;
    tmp -> type = AT_BEGIN;
    first -> first -> next = tmp;
    first -> last = tmp;
    first -> name = malloc(sizeof("_start"));
    if (!first -> name) return MEMORY_ERROR;
    first -> arg_first = NULL;
    strcpy(first -> name, "_start");
    tmp -> next = NULL;

    data -> first = first;
    data -> last = first;
    data -> var_num = 0;
    data -> alloc_num = 0;
    data -> max_alloc_num = 16;
    data -> allocs = malloc(sizeof(int) * 16);
    if (!data -> allocs) return MEMORY_ERROR;
    data -> sizes = malloc(sizeof(int) * 16);
    if (!data -> sizes) return MEMORY_ERROR;
    data -> table = malloc(sizeof(HashTable));
    if (!data -> table) return MEMORY_ERROR;
    return init_table(data -> table);
}

static int add_data(Data *data, int n, int size){
    (data -> allocs)[data -> alloc_num] = n;
    (data -> sizes)[data -> alloc_num] = size;
    (data -> alloc_num)++;
    if (data -> alloc_num == data -> max_alloc_num){
        (data -> max_alloc_num) *= 2;
        data -> allocs = realloc(data -> allocs, data -> max_alloc_num);
        if (!data -> allocs) return MEMORY_ERROR;
        data -> sizes = realloc(data -> sizes, data -> max_alloc_num);
        if (!data -> sizes) return MEMORY_ERROR;
    }
    return NO_ERROR;
}

static int init_line(Data *data, Atom *first){
    Atom *node = first;
    Atom *prev;
    if (node -> type == AT_EOL) return NO_ERROR;
    int err, size;
    char *str;

    if (node -> type == AT_DECLARE){
        node = node -> next;
        while (node -> type != AT_EOL){
            if (node -> type == AT_NAME){
                str = node -> str;
                if (node -> next -> type == AT_BEGIN){
                    node = node -> next -> next;
                    if (node -> type != AT_NUM || node -> next -> type != AT_END){
                        return DECL_TYPE_ERROR;
                    }
                    size = node -> value;
                    node = node -> next;
                } else {
                    size = 1;
                }
                err = insert_node(str, HS_VAR,
                                  aloc_int(data -> var_num),
                                  data -> table);
                if (err){
                    return err;
                }
                err = add_data(data, data -> var_num, size);
                if (err){
                    return err;
                }
                (data -> var_num)++;
            } else if (node -> type != AT_SEP){
                return DECL_TYPE_ERROR; 
            }
            node = node -> next;
        }
        return NO_ERROR;
    }

    int sub = 0;
    while (sub || node -> next -> type != AT_DEF){
        if (node -> next -> type == AT_EOL){
            data -> first -> last -> next = first;
            data -> first -> last = node;
            return NO_ERROR;
        }
        if (node -> type == AT_SUB_BEG) sub++;
        if (node -> type == AT_SUB_END) sub--;
        node = node -> next;
    }
    prev = node;
    node = node -> next;
    node = node -> next;
    prev -> next = NULL;

    Line *new;
    new = malloc(sizeof(Line));
    if (!new) return MEMORY_ERROR;
    new -> name = first -> str;
    new -> next = NULL;
    new -> first = node;
    new -> arg_first = first -> next;

    while (node -> next -> type != AT_EOL){
        node = node -> next;
    }
    node -> next = NULL;

    data -> last -> next = new;
    data -> last = new;
    err = insert_node(first -> str, HS_VAR,
                       aloc_int(data -> var_num), data -> table);
    if (err){
        return err;
    }
    (data -> var_num)++;

    return NO_ERROR;
}

int init_file(Data *data, int maxline){
    char line[maxline];
    int err;

    init_data(data);
    init_built_in(data -> table);

    Atom *reading;
    reading = malloc(sizeof(Atom));
    if (!reading) return MEMORY_ERROR;
    err = get_real_line(line, maxline);
    while (!err){
        err = atomise_line(line, reading);
        if (err) return err;
        err = init_line(data, reading);
        if (err) return err;
        reading = malloc(sizeof(Atom));
        if (!reading) return MEMORY_ERROR;
        err = get_real_line(line, maxline);
    }
    if (err != EOF_ENCOUNTERED) return err;

    reading = malloc(sizeof(Atom));
    if (!reading) return MEMORY_ERROR;
    reading -> type = AT_END;
    reading -> next = NULL;
    data -> first -> last -> next = reading;

    return NO_ERROR;
}
