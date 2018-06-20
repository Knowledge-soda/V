#include<stdio.h>
#include<string.h>

#include"misc.h"
#include"tools.h"


int is_numeral(char c){
    return ('0' <= c && c <= '9') || c == ' ' || c == '-';
}

int is_num(char *str){
    int i, sign = 1;
    char last;
    for (i = 0;str[i];i++){
        if (!is_numeral(str[i])) return 0;
        if (!sign && str[i] == '-') return 0;
        if (str[i] != ' '){
            sign = 0;
            last = str[i];
        }
    }
    return last != '-';
}

int str_num(char *str){
    int i, ret = 0, sign = 1;
    for (i = 0;str[i];i++){
        if (str[i] == ' ') continue;
        if (str[i] == '-') sign = -1;
        else {
            ret *= 10;
            ret += str[i];
            ret -= '0';
        }
    }
    return ret * sign;
}

void del_nl(char *str){
    int i = strlen(str) - 1;
    if (str[i] == '\n')
        str[i] = '\0';
}

int count_paren(char *str, char a, char b, int start){
    int i;
    for (i = 0;str[i];i++){
        if (str[i] == a) start++;
        if (str[i] == b) start--;
        if (start < 0) return -1;
    }
    return start;
}

int truncate(char *str){
    // returns size of end string
    int i, dif, last;
    for (i = 0;str[i] == ' ' && str[i] != '\0';i++);
    if (str[i] == '\0'){
        str[0] = '\0';
        return 0;
    }
    dif = i;
    for (;str[i];i++){
        str[i - dif] = str[i];
        if (str[i] != ' ') last = i;
    }
    str[last - dif + 1] = '\0';
    return last - dif + 1;
}

int equal(char *a, char *b){
    int i;
    for (i = 0;a[i];i++)
        if (a[i] != b[i])
            return 0;
    return !b[i];
}

void num_name(int n, char *str){
    int i = 0;
    str[i] = 'a' + n % 26;
    n /= 26;
    i++;
    while (n){
        n--;
        str[i] = 'a' + n % 26;
        n /= 26;
        i++;
    }
    str[i] = '\0';
}

int get_real_line(char *line, int line_size){
    int paren = 0, comment = 0, i = 0, j = 0;
    while (1){
        if (!get_line(line + j, line_size - j)){
            if (paren || comment) return EOF_EARLY_ERROR;
            return EOF_ENCOUNTERED;
        }
        i = j;
        for (;line[i];i++){
            if (line[i] == '\n') continue;
            if (line[i] == '/' && line[i + 1] == '*'){
                comment++;
            }
            if (!comment){
                if (line[i] == '('){
                    paren++;
                }
                if (line[i] == ')'){
                    paren--;
                }
                line[j] = line[i];
                j++;
            }
            if (line[i - 1] == '*' && line[i] == '/'){
                comment--;
            }
            if (comment < 0) return COM_CLOSE_ERROR;
            if (paren < 0) return PAREN_CLOSE_ERROR;
        }
        if (!paren && !comment){
            line[j] = '\0';
            return NO_ERROR;
        }
        if (feof(stdin)){
            return EOF_EARLY_ERROR;
        }
    }
}


void print_error(int err){
    if (err == NAME_ERROR){
        fprintf(stderr, "ERROR: name already exists!\n");
    }
    if (err == MEMORY_ERROR){
        fprintf(stderr, "ERROR: ran out of memory!\n");
    }
    if (err == INNER_ERROR){
        fprintf(stderr, "ERROR: my fault (report this error)!\n");
    }
    if (err == DECL_TYPE_ERROR){
        fprintf(stderr, "ERROR: invalid token on declaration line!\n");
    }
    if (err == COM_CLOSE_ERROR){
        fprintf(stderr, "ERROR: comment closed too many times!\n");
    }
    if (err == PAREN_CLOSE_ERROR){
        fprintf(stderr, "ERROR: parenthesis closed too many times!\n");
    }
    if (err == EOF_EARLY_ERROR){
        fprintf(stderr, "ERROR: EOF while reading statement!\n");
    }
    if (err == EOF_ENCOUNTERED){
        fprintf(stderr, "ERROR: this is not an error (report this non error)!\n");
    }
    if (err == ARG_ERROR){
        fprintf(stderr, "ERROR: invalid token in arguments!\n");
    }
    if (err == EXIST_ERROR){
        fprintf(stderr, "ERROR: name not defined!\n");
    }
    if (err == NO_NAME_ERROR){
        fprintf(stderr, "ERROR: function has no name!\n");
    }
    if (err == UNDEF_ERROR){
        fprintf(stderr, "ERROR: built-in function not defined!\n");
    }
}
