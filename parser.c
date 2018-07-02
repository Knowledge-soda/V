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
    if (n == BL_STR){
        return "STR";
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
        first = first -> next;
    }
    return NO_ERROR;
}

int parse_line(Atom *atom, Node *first, HashTable *table){
    HashNode *tmp;
    Node *stack[MAXDEPTH];
    int stack_size = 0;
    while (atom){
    //    printf("cl(%i)\n", atom -> type);
        if (atom -> type == AT_NUM){
            set_type(first, PS_NUM);
            first -> name = atom -> value;
        }
        if (atom -> type == AT_NAME){
            tmp = get_node(atom -> str, table);
    //      printf(":::%s\n", atom -> str);
            if (!tmp) return EXIST_ERROR;
    //        printf(":::%s(%i)\n", atom -> str, tmp -> type);
            if (tmp -> type == HS_VAR) set_type(first, PS_VAR);
            if (tmp -> type == HS_VAL) set_type(first, PS_VAL);
            if (tmp -> type == HS_SPC) set_type(first, PS_BLN);
            first -> name = *((int*)(tmp -> data));
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
    //        printf("(stack:%i@%i\n", first -> name, get_type(first));
            stack_size++;
            first -> next = malloc(sizeof(Node));
            first -> first = malloc(sizeof(Node));
            first = first -> first;
            atom = atom -> next;
        } else if (atom -> type == AT_END){
    //        printf("END\n");
            set_type(first, PS_BLN);
    //        printf(".0\n");
            first -> name = BL_NOP;
    //        printf(".1\n");
            stack_size--;
            stack[stack_size] -> last = first;
            first -> next = NULL;
    //        printf(".2\n");
            first = stack[stack_size] -> next;
    //        printf(")stack:%i@%i\n", stack[stack_size] -> name, get_type(stack[stack_size]));
    //        printf("sz = %i\n", stack_size);
        } else {
    //        printf("NORMAL\n");
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
        read = get_node(line -> name, data -> table);
    //  printf("%s\n", line -> name);
        if (!read) return EXIST_ERROR;
        curr -> name = *((int*)(read -> data));
        curr -> first = malloc(sizeof(ParserNode));

        start_tmp_session(data -> table);
    //    printf("ARG\n");
        if (line -> arg_first){
            err = argumentise(line -> arg_first, data -> table);
            if (err) return err;
        }
    //    printf("ANA\n");
        err = analyse(line -> first, file, data);
        if (err) return err;
    //    printf("PLN\n");
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
    file -> data_size = data -> alloc_num;


    return NO_ERROR;
}
