#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#include"parser.h"

#include"lexer.h"
#include"init.h"

typedef ParserNode Node;
typedef ParserFunction Function;
typedef ParserFile File;

byte get_type(Node *node){
    return (node -> type) >> 1;
}

byte is_func(Node *node){
    return (node -> type) & 1;
}

static void set_type(Node *node, byte type){
    node -> type = type << 1;
}

static void set_func(Node *node){
    node -> type |= 1;
}

static void *aloc_int(int n){
    int *num = malloc(sizeof(int));
    *num = n;
    return num;
}

static int add_str(File *file, int n, char *str){
    (file -> strings)[file -> str_size] = str;
    (file -> str_vals)[file -> str_size] = n;
    (file -> str_size)++;
    if (file -> str_size == file -> str_max_size){
        (file -> str_max_size) *= 2;
        file -> strings = realloc(file -> strings, sizeof(char*) * (file -> str_max_size));
        if (!file -> strings) return MEMORY_ERROR;
        file -> str_vals = realloc(file -> str_vals, sizeof(int) * (file -> str_max_size));
        if (!file -> str_vals) return MEMORY_ERROR;
    }
    return NO_ERROR;
}

const char *built_in_name(int n){
    if (n == BL_NOP){
        return "NOP";
    }
    if (n == BL_LAST){
        return "LAST";
    }
    if (n == BL_SUM){
        return "SUM";
    }
    if (n == BL_MUL){
        return "MUL";
    }
    if (n == BL_VAL){
        return "VAL";
    }
    if (n == BL_MOV){
        return "MOV";
    }
    if (n == BL_EQU){
        return "EQU";
    }
    if (n == BL_MORE){
        return "MORE";
    }
    if (n == BL_LESS){
        return "LESS";
    }
    if (n == BL_IF){
        return "IF";
    }
    if (n == BL_IJMP){
        return "IJMP";
    }
    if (n == BL_PRNT){
        return "PRNT";
    }
    if (n == BL_LOAD){
        return "LOAD";
    }
    if (n == BL_NOT){
        return "NOT";
    }
    if (n == BL_AND){
        return "AND";
    }
    if (n == BL_OR){
        return "OR";
    }
    if (n == BL_SUB){
        return "SUB";
    }
    if (n == BL_DIV){
        return "DIV";
    }
    if (n == BL_MOD){
        return "MOD";
    }
    if (n == BL_JMP){
        return "JMP";
    }
    if (n == BL_IFC){
        return "IFC";
    }
    if (n == BL_STR){
        return "STR";
    }
    if (n == BL_IFEC){
        return "IFEC";
    }
    if (n == BL_AT){
        return "AT";
    }
    if (n == BL_ATMV){
        return "ATMV";
    }
    if (n == BL_INDX){
        return "INDX";
    }
    if (n == BL_PRNF){
        return "PRNF";
    }
    if (n == BL_PRFR){
        return "PRFR";
    }
    if (n == BL_FSUM){
        return "FSUM";
    }
    if (n == BL_FMUL){
        return "FMUL";
    }
    if (n == BL_FSUB){
        return "FSUB";
    }
    if (n == BL_FDIV){
        return "FDIV";
    }
    if (n == BL_LDF){
        return "LDF";
    }
    if (n == BL_FEQU){
        return "FEQU";
    }
    if (n == BL_FORE){
        return "FORE";
    }
    if (n == BL_FESS){
        return "FESS";
    }
    return "UNDEFINED";
}

void dbg_parser(ParserNode *node){
    if (get_type(node) == PS_NUM){
        printf("NUM(%i)", node -> name);
    } else if (get_type(node) == PS_VAR){
        printf("VAR(%i)", node -> name);
    } else if (get_type(node) == PS_VAL){
        printf("VAL(%i)", node -> name);
    } else if (get_type(node) == PS_BLN){
        printf("BLN(%s)", built_in_name(node -> name));
    } else if (get_type(node) == PS_PLC){
        printf("PLC(%i)", node -> name);
    } else if (get_type(node) == PS_COM){
        printf("COM");
    }
}

static int argumentise(Atom *first, HashTable *table){
    if (first -> type != AT_BEGIN) return ARG_ERROR;
    int n = 0, err;
    first = first -> next;
    while (first -> type != AT_END){
        if (first -> type == AT_NAME){
            err = insert_tmp_node(first -> str, HS_VAL,
                                  aloc_int(n), table);
            if (err) return err;
            n++;
        } else if (first -> type != AT_SEP) return ARG_ERROR;
        if (!first -> next) return ARG_ERROR;
        first = first -> next;
    }
    return NO_ERROR;
}

static int analyse(Atom *first, File *file, InitData *data){
    int err;
    Atom *starts[MAXLAMBDADEPTH], *mids[MAXLAMBDADEPTH];
    Atom *prev, *tmp;
    int sub = 0;
    Line *new;
    while (first){
        if (first -> type == AT_PLACE){
            err = insert_tmp_node(first -> str, HS_VAR,
                                  aloc_int(data -> var_num),
                                  data -> table);
            if (err) return err;
            (data -> var_num)++;
        }
        if (first -> type == AT_STR){
            first -> value = data -> var_num;
            err = add_str(file, data -> var_num, first -> str);
            if (err) return err;
            (data -> var_num)++;
        }
        if (first -> type == AT_SUB_BEG){
            starts[sub] = first;
            mids[sub] = NULL;
            sub++;
        }
        if (first -> type == AT_DEF){
            mids[sub - 1] = first;
        }
        if (first -> type == AT_SUB_END){
            sub--;
            new = malloc(sizeof(Line));
            if (!new) return MEMORY_ERROR;
            starts[sub] -> value = data -> var_num;
            tmp = malloc(sizeof(Atom));
            tmp -> type = AT_BEGIN;
            if (mids[sub]){
                tmp -> next = starts[sub] -> next;
            } else {
                mids[sub] = malloc(sizeof(Atom));
                mids[sub] -> type = AT_END;
                mids[sub] -> next = starts[sub] -> next;
                tmp -> next = mids[sub];
            }
            new -> name = NULL;
            new -> num = data -> var_num;
            new -> arg_first = tmp;
            new -> first = mids[sub] -> next;
            mids[sub] -> type = AT_END;
            new -> last = prev;
            prev -> next = NULL;
            new -> next = NULL;
            data -> last -> next = new;
            data -> last = new;
            (data -> var_num)++;
            starts[sub] -> next = first -> next;
        }
        prev = first;
        first = first -> next;
    }
    return NO_ERROR;
}

int parse_line(Atom *atom, Node *first, HashTable *table){
    HashNode *tmp;
    Node *stack[MAXDEPTH];
    int stack_size = 0;
    while (atom){
        if (atom -> type == AT_NUM){
            set_type(first, PS_NUM);
            first -> name = atom -> value;
        }
        if (atom -> type == AT_NAME){
            tmp = get_node(atom -> str, table);
            if (!tmp) return EXIST_ERROR;
            if (tmp -> type == HS_VAR) set_type(first, PS_VAR);
            if (tmp -> type == HS_VAL) set_type(first, PS_VAL);
            if (tmp -> type == HS_SPC) set_type(first, PS_BLN);
            first -> name = *((int*)(tmp -> data));
        }
        if (atom -> type == AT_SUB_BEG){
            set_type(first, PS_VAR);
            first -> name = atom -> value;
        }
        if (atom -> type == AT_PLACE){
            set_type(first, PS_PLC);
            tmp = get_node(atom -> str, table);
            first -> name = *((int*)(tmp -> data));
        }
        if (atom -> type == AT_BEGIN){
            return NO_NAME_ERROR;
        }
        if (atom -> type == AT_SEP){
            set_type(first, PS_COM);
            (stack[stack_size - 1] -> size)++;
        }
        if (atom -> type == AT_STR){
            set_type(first, PS_BLN);
            first -> name = BL_STR;
            set_func(first);
            first -> size = 1;
            first -> first = malloc(sizeof(Node));
            set_type(first -> first, PS_VAR);
            first -> first -> name = atom -> value;
            first -> last = malloc(sizeof(Node));
            set_type(first -> last, PS_BLN);
            first -> last -> name = BL_NOP;
        }
        if (atom && atom -> next && atom -> next -> type == AT_BEGIN){
            set_func(first);
            first -> size = 1;
            stack[stack_size] = first;
            stack_size++;
            first -> next = malloc(sizeof(Node));
            first -> first = malloc(sizeof(Node));
            first = first -> first;
            atom = atom -> next;
        } else if (atom -> type == AT_END){
            set_type(first, PS_BLN);
            first -> name = BL_NOP;
            stack_size--;
            stack[stack_size] -> last = first;
            first -> next = NULL;
            first = stack[stack_size] -> next;
        } else {
            first -> next = malloc(sizeof(Node));
            first = first -> next;
        }
        atom = atom -> next;
    }
    set_type(first, PS_BLN);
    first -> name = BL_NOP;

    return NO_ERROR;
}


int parse_file(InitData *data, File *file){
    Line *line;
    Function *curr;
    HashNode *read;
    int err;
    curr = malloc(sizeof(Function));
    file -> first = curr;
    file -> str_max_size = 16;
    file -> strings = malloc(sizeof(char*) * 16);
    file -> str_vals = malloc(sizeof(int) * 16);
    file -> str_size = 0;
    for (line = data -> first;line;line = line -> next){
        if (line -> name){
            read = get_node(line -> name, data -> table);
            if (!read) return EXIST_ERROR;
            curr -> name = *((int*)(read -> data));
        } else {
            curr -> name = line -> num;
        }
        curr -> first = malloc(sizeof(ParserNode));

        start_tmp_session(data -> table);
        if (line -> arg_first){
            err = argumentise(line -> arg_first, data -> table);
            if (err) return err;
        }
        err = analyse(line -> first, file, data);
        if (err) return err;
        err = parse_line(line -> first, curr -> first, data -> table);
        if (err) return err;
        end_tmp_session(data -> table);

        if (line -> next){
            curr -> next = malloc(sizeof(Function));
            curr = curr -> next;
        } else {
            curr -> next = NULL;
        }
    }
    file -> data = data -> allocs;
    file -> sizes = data -> sizes;
    file -> data_size = data -> alloc_num;

    return NO_ERROR;
}
