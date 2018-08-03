#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"hashtable.h"
#include"tools.h"

#define MAXLINE (2000)

int parse_file(char *filename, HashTable *table);

static int is_def(char *str){
    return (str[0] == ' ' && str[1] == ':' &&
            str[2] == '=' && str[3] == ' ');
}

static int is_dec(char *str){
    return (str[0] == ':' && str[1] == ' ');
}

static int is_key(char *str){
    return (*str == '(' || *str == ')' || *str == '"' ||
            *str == ',' || *str == '{' || *str == '}' ||
            is_dec(str) || is_def(str));
}

void basename(char *path, char *base){
    int i, last = 0;
    for (i = 0;path[i];i++){
        if (path[i] == '/'){
            last = i + 1;
        }
    }
    for (i = 0;i < last;i++){
        base[i] = path[i];
    }
    base[i] = '\0';
}

char *read_word(char *line, char *name){
    int i, j = 0, trn = 0, comm = 0;
    for (i = 0;!trn || line[i] != ' ';i++){
        if (line[i] == '/' && line[i + 1] == '*') comm++;
        if (line[i] == '/' && line[i - 1] == '/') comm--;
        else if (!comm){
            if (trn || line[i] != ' '){
                trn = 1;
                name[j] = line[i];
                j++;
            }
        }
    }
    name[j] = '\0';
    return line + i;
}

char *read_str(char *line, char *name){
    int i, j = 0, comm = 0, qt = 0;
    for (i = 0;qt < 2;i++){
        if (line[i] == '\n') break;
        if (line[i] == '/' && line[i + 1] == '*') comm++;
        if (line[i] == '/' && line[i - 1] == '/') comm--;
        else if (!comm){
            if (line[i] == '"') qt++;
            else if (qt == 1){
                name[j] = line[i];
                j++;
            }
        }
    }
    name[j] = '\0';
    return line + i + 1;
}

int prep_line(char *line, char *tmp_filename, HashTable *table){
    int err;
    char name[200];
    line = read_word(line + 3, name);
    if (equal(name, "include")){
        char *file_name = tmp_filename + strlen(tmp_filename);
        line = read_str(line, file_name);
        while (*file_name){
            err = parse_file(tmp_filename, table);
            if (err) return err;
            line = read_str(line, file_name);
        }
    }
    if (equal(name, "define")){
        line = read_str(line, name);
        char *cont;
        cont = malloc(sizeof(char) * 200);
        line = read_str(line, cont);
        insert_node(name, 0, cont, table);
    }
    return 0;
}

int proc_line(char *line, int *comm, HashTable *table){
    int j, i = 0;
    if (line[0] == ':'){
        i++;
        printf(":");
    }
    char name[200];
    HashNode *node;
    for (j = 0;line[i] != '\n';i++){
        if (line[i] == '/' && line[i + 1] == '*') (*comm)++;
        if (line[i] == '/' && line[i - 1] == '*') (*comm)--;
        else if (!(*comm)){
            if (is_key(line + i)){
                name[j] = '\0';
                truncate(name);
                j = 0;
                node = get_node(name, table);
                if (node){
                    printf("%s", (char *)node -> data);
                } else {
                    printf("%s", name);
                }
                if (is_dec(line + i)){
                    printf(": ");
                    i++;
                }
                if (is_def(line + i)){
                    printf(" := ");
                    i += 3;
                }
                else printf("%c", line[i]);
            } else {
                name[j] = line[i];
                j++;
            }
        }
    }
    name[j] = '\0';
    truncate(name);
    node = get_node(name, table);
    if (node){
        printf("%s", (char *)node -> data);
    } else {
        printf("%s", name);
    }
    printf("\n");
    return 0;
}

int parse_file(char *filename, HashTable *table){
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL){
        fprintf(stderr, "Error while opening file '%s'!\n", filename);
        return 1;
    }
    char line[MAXLINE];
    char tmp_filename[200];
    basename(filename, tmp_filename);
    int err, comm = 0;
    while (fgets(line, MAXLINE, file)){
        if (!comm && line[0] == '-' && line[1] == '-' && line[2] == ' '){
            err = prep_line(line, tmp_filename, table);
            if (err) return err;
        } else {
            err = proc_line(line, &comm, table);
            if (err) return err;
        }
    }
    return 0;
}

int main(int argc, char **argv){
    HashTable *table;
    table = malloc(sizeof(HashTable));
    init_table(table);
    if (argc != 2){
        fprintf(stderr, "Error, wrong number of parameters!\n");
        return 1;
    }
    return parse_file(argv[1], table);
}
