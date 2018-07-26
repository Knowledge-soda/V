#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"tools.h"
#include"hashtable.h"
#include"lexer.h"
#include"init.h"
#include"parser.h"

#define MAXLINE 10000
#define MAXVARS 10000

void print_pnode(ParserNode *node, int inden){
    int i;
    ParserNode *tmp;
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
    if (is_func(node)){
        printf(":\n");
        for (i = 0;i < inden;i++) printf("  ");
        for (tmp = node -> first;tmp;tmp = tmp -> next){
            print_pnode(tmp, inden + 1);
        }
        putchar('\n');
        for (i = 1;i < inden;i++) printf("  ");
    } else {
        printf(", ");
    }
}

int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "ERROR: Wrong number of parameters (%i) for test!\n", argc - 1);
        return 1;
    }

    if (equal(argv[1], "count_paren")){
        int start;
        scanf("%i\n", &start);
        char line[MAXLINE];
        get_line(line, MAXLINE);
        char a, b;
        a = line[0];
        b = line[1];
        get_line(line, MAXLINE);
        printf("%i\n", count_paren(line, a, b, start));
    }

    if (equal(argv[1], "str_num")){
        char line[MAXLINE];
        int tmp, ans;
        float pr;
        get_line(line, MAXLINE);
        del_nl(line);
        tmp = is_num(line);
        if (tmp == 2){
            ans = str_num(line);
            memcpy(&pr, &ans, sizeof(float));
            printf("%.4f\n", pr);
        } else if (tmp == 1){
            printf("%i\n", str_num(line));
        } else {
            printf("Not a number!\n");
        }
    }

    if (equal(argv[1], "trunc")){
        char line[MAXLINE];
        int size;
        get_line(line, MAXLINE);
        del_nl(line);
        size = truncate(line);
        printf("|%s|\n", line);
        printf("%i\n", size);
    }

    if (equal(argv[1], "num_name")){
        char line[MAXLINE];
        get_line(line, MAXLINE);
        del_nl(line);
        int end = str_num(line);
        int i;
        for (i = 0;i < end;i++){
            char out[100];
            num_name(i, out);
            printf("%s\n", out);
        }
    }

    if (equal(argv[1], "hashtable")){
        char name[MAXLINE];
        char line[MAXLINE];
        int vars[MAXVARS];
        int var_size = 0;
        int err;
        HashTable table;
        HashNode *node;
        init_table(&table);
        while (1){
            get_line(name, MAXLINE);
            if (name[0] == '0' && name[1] == '\n') break;
            del_nl(name);
            get_line(line, MAXLINE);
            if (line[0] == '?' && line[1] == '\n'){
                node = get_node(name, &table);
                if (node)
                    printf("%i\n", *((int*)(node -> data)));
                else
                    printf("EXISTS NOT\n");
            } else {
                del_nl(line);
                vars[var_size] = str_num(line);
                err = insert_node(name, 0, vars + var_size, &table);
                if (err == 1){
                    printf("EXISTS\n");
                } else {
                    var_size++;
                }
            }
        }
    }

    if (equal(argv[1], "hashtable_tmp")){
        char name[MAXLINE];
        char line[MAXLINE];
        int vars[MAXVARS];
        int var_size = 0;
        int err;
        HashTable table;
        HashNode *node;
        init_table(&table);
        start_tmp_session(&table);
        while (1){
            get_line(name, MAXLINE);
            if (name[0] == '0' && name[1] == '\n') break;
            del_nl(name);
            get_line(line, MAXLINE);
            if (line[0] == '?'){
                node = get_node(name, &table);
                if (node)
                    printf("%i\n", *((int*)(node -> data)));
                else
                    printf("EXISTS NOT\n");
            } else if (line[0] == 't'){
                get_line(line, MAXLINE);
                del_nl(line);
                vars[var_size] = str_num(line);
                err = insert_tmp_node(name, 0, vars + var_size, &table);
                if (err == 1){
                    printf("EXISTS\n");
                } else {
                    var_size++;
                }
            } else if (line[0] == 'n'){
                get_line(line, MAXLINE);
                del_nl(line);
                vars[var_size] = str_num(line);
                err = insert_node(name, 0, vars + var_size, &table);
                if (err == 1){
                    printf("EXISTS\n");
                } else {
                    var_size++;
                }
            } else if (line[0] == 'r'){
                end_tmp_session(&table);
                start_tmp_session(&table);
            } else {
                del_nl(line);
                fprintf(stderr, "Error: invalid operator type '%s'!\n", line);
                return 1;
            }
        }
    }

    if (equal(argv[1], "get_real_line")){
        char line[MAXLINE];
        int ret;
        ret = get_real_line(line, MAXLINE);
        while (!ret){
            printf("%s\n", line);
            ret = get_real_line(line, MAXLINE);
        }
        if (ret == COM_CLOSE_ERROR){
            printf("Comment closed too many times!\n");
        }
        if (ret == PAREN_CLOSE_ERROR){
            printf("Parenthesis closed too many times!\n");
        }
        if (ret == EOF_EARLY_ERROR){
            printf("EOF while reading statement!\n");
        }
    }

    if (equal(argv[1], "lexer")){
        char line[MAXLINE];
        get_line(line, MAXLINE);
        del_nl(line);
        Atom *first;
        first = malloc(sizeof(Atom));
        atomise_line(line, first);
        while (first){
            print_atom(first);
            if (first -> next){
                printf(" ");
            } else {
                printf("\n");
            }
            first = first -> next;
        }
    }

    if (equal(argv[1], "init")){
        InitData *data;
        data = malloc(sizeof(InitData));
        int err = init_file(data, MAXLINE);
        if (err) print_error(err);
        Line *line;
        Atom *atom;
        line = data -> first;
        while (line){
            printf("%s:\n    ", line -> name);
            atom = line -> arg_first;
            while (atom){
                print_atom(atom);
                if (atom -> next){
                    printf(" ");
                } else {
                    printf(" : ");
                }
                atom = atom -> next;
            }
            atom = line -> first;
            while (atom){
                print_atom(atom);
                if (atom -> next){
                    printf(" ");
                } else {
                    printf("\n");
                }
                atom = atom -> next;
            }
            line = line -> next;
        }
    }
    
    if (equal(argv[1], "parse")){
        InitData *data;
        data = malloc(sizeof(InitData));
        int err = init_file(data, MAXLINE);
        if (err) print_error(err);
        ParserFile *file;
        ParserFunction *func;
        file = malloc(sizeof(ParserFile));
        err = parse_file(data, file);
        if (err) print_error(err);
        for (func = file -> first;func;func = func -> next){
            printf("%i::\n  ", func -> name);
            print_pnode(func -> first, 2);
            printf("\n");
        }
    }

    return 0;
}
