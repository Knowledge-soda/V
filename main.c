#include<stdlib.h>
#include<stdio.h>

#include"compiler.h"
#include"tools.h"

#define MAXLINE 10000

int main(void){
    int err;
    InitData *data;
    ParserFile *file;
    data = malloc(sizeof(InitData));
    if (!data) print_error(MEMORY_ERROR);
    file = malloc(sizeof(ParserFile));
    if (!file) print_error(MEMORY_ERROR);
    err = init_file(data, MAXLINE);
    if (err) print_error(err);
    err = parse_file(data, file);
    if (err) print_error(err);
    err = compile_file(file);
    if (err) print_error(err);
    return 0;
}
