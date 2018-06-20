#ifndef HASHTABLE_INCLUDED
#define HASHTABLE_INCLUDED

#include"misc.h"

typedef struct h_node {
    struct h_node *next, *prev;
    char *name;
    byte type;
    byte tmpness;
    void *data;
} HashNode;

typedef struct table {
    HashNode **heads, **tails, **tmps;
    int tmp_size;
    byte tmpness;
} HashTable;

int init_table(HashTable *table);
HashNode *get_node(char *name, HashTable *table);
int insert_node(char *name, byte type, void *data, HashTable *table);
int insert_tmp_node(char *name, byte type, void *data, HashTable *table);
void start_tmp_session(HashTable *table);
void end_tmp_session(HashTable *table);

#endif
