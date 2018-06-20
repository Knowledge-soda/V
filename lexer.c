#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"tools.h"
#include"lexer.h"

static int is_def(char *str){
    return (str[0] == ' ' && str[1] == ':' &&
            str[2] == '=' && str[3] == ' ');
}

static int is_dec(char *str){
    return (str[0] == ':' && str[1] == ' ');
}

static int atom_type(char *name){
    if (is_num(name)) return AT_NUM;
    if (name[strlen(name) - 1] == ':') return AT_PLACE;
    return AT_NAME;
}


static int is_key(char *str){
    return (*str == '(' || *str == ')' || *str == '"' ||
            *str == ',' || is_dec(str) || is_def(str));
}

int atomise_line(char *line, Atom *first){
    int i, j = 0, size, str = 0;
    char name[strlen(line) + 1];
    if (line[0] == ':'){
        line[0] = ' ';
        first -> type = AT_DECLARE;
        first -> next = malloc(sizeof(Atom));
        first = first -> next;
    }

    for (i = 0;line[i];i++){
        name[j] = line[i];
        if (!str && is_key(line + i)){
            name[j] = '\0';
            j = 0;
            size = truncate(name);
            if (size){
                first -> type = is_dec(line + i) ? AT_PLACE : atom_type(name);
                if (first -> type == AT_NUM){
                    first -> value = str_num(name);
                } else {
                    first -> str = malloc(sizeof(char) * (size + 1));
                    strcpy(first -> str, name);
                }
                first -> next = malloc(sizeof(Atom));
                first = first -> next;
            }

            if (is_dec(line + i)) continue;
            if (is_def(line + i)){
                first -> type = AT_DEF;
                i += 3;
            } else if (line[i] == '('){
                first -> type = AT_BEGIN;
            } else if (line[i] == ')'){
                first -> type = AT_END;
            } else if (line[i] == '"'){
                str = 1;
            } else if (line[i] == ','){
                first -> type = AT_SEP;
            } else {
                printf("line = '%s'\n", line);
                printf("T=%i, s=%c, i=%i\n", first -> type, line[i], i);
                return INNER_ERROR;
            }
            if (!str){
                first -> next = malloc(sizeof(Atom));
                first = first -> next;
            }
        } else if (str && line[i] == '"'){
            if (line[i + 1] == '"'){
                name[j] = '"';
                i++;
            } else {
                first -> type = AT_STR;
                name[j] = '\0';
                j = 0;
                first -> str = malloc(sizeof(char) * (j + 1));
                strcpy(first -> str, name);
                first -> next = malloc(sizeof(Atom));
                first = first -> next;
                str = 0;
            }
        } else {
            j++;
        }
    }
    name[j] = '\0';
    j = 0;
    size = truncate(name);
    if (size){
        first -> type = atom_type(name);
        if (first -> type == AT_NUM){
            first -> value = str_num(name);
        } else {
            first -> str = malloc(sizeof(char) * (size + 1));
            strcpy(first -> str, name);
        }
        first -> next = malloc(sizeof(Atom));
        first = first -> next;
    }

    first -> type = AT_EOL;
    first -> next = NULL;
    return NO_ERROR;
}
