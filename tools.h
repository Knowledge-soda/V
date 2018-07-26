#ifndef TOOLS_INCLUDED
#define TOOLS_INCLUDED

#include<stdio.h>

#define get_line(str, m_size) fgets(str, m_size, stdin)

int is_numeral(char c);
int is_num(char *str);
int str_num(char *str);
int str_float(char *str);

void del_nl(char *str);

int count_paren(char *str, char a, char b, int start);
#define count_par(str, start) count_paren(str, '(', ')', start)
int truncate(char *str);
int equal(char *a, char *b);

void num_name(int n, char *str);
int get_real_line(char *str, int size);
void print_error(int err);

#endif
