#include<stdio.h>
#include<string.h>

#include"hashtable.h"

#define MAXLINE (2000)

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

int equal(char *a, char *b){
    int i;
    for (i = 0;a[i];i++)
        if (a[i] != b[i])
            return 0;
    return !b[i];
}

void del_nl(char *str){
    int i = strlen(str) - 1;
    if (str[i] == '\n')
        str[i] = '\0';
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

int parse_file(char *filename){
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL){
        fprintf(stderr, "Error while opening file '%s'!\n", filename);
        return 1;
    }
    char line[MAXLINE];
    char tmp_filename[200];
    basename(filename, tmp_filename);
    char name[50];
    int err, i, j = 0, comm = 0;
    while (fgets(line, MAXLINE, file)){
        del_nl(line);
        if (line[0] == '-' && line[1] == '-' && line[2] == ' '){
            for (i = 3;line[i] != ' ';i++){
                if (line[i] == '/' && line[i + 1] == '*') comm++;
                name[j] = line[i];
                j++;
            }
            name[j] = '\0';
            if (equal(name, "include")){
                strcat(tmp_filename, line + i + 1);
                err = parse_file(tmp_filename);
                if (err) return err;
                basename(filename, tmp_filename);
            }
        } else {
            puts(line);
        }
    }
    return 0;
}

int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "Error, wrong number of parameters!\n");
        return 1;
    }
    return parse_file(argv[1]);
}
