#include<stdio.h>
#include<string.h>

#include"hashtable.h"
#include"tools.h"

#define MAXLINE (2000)

int parse_file(char *filename);

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

int prep_line(char *line, char *tmp_filename){
    int err;
    char name[200];
    line = read_word(line + 3, name);
    if (equal(name, "include")){
        char *file_name = tmp_filename + strlen(tmp_filename);
        line = read_str(line, file_name);
        while (*file_name){
            err = parse_file(tmp_filename);
            if (err) return err;
            line = read_str(line, file_name);
        }
    }
    return 0;
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
    char name[200];
    int err, i, j, comm = 0;
    while (fgets(line, MAXLINE, file)){
        if (!comm && line[0] == '-' && line[1] == '-' && line[2] == ' '){
            err = prep_line(line, tmp_filename);
            if (err) return err;
        } else {
            /*
            j = 0;
            for (i = 0;line[i];i++){
                if (line[i] == '/' && line[i + 1] == '*') comm++;
                if (line[i] == '*' && line[i + 1] == '/') comm--;
                if (!comm){
                    name[j] = line[i];
                    j++;
                }
            } */
            printf("%s", line);
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
