#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"tools.h"

#include"hashtable.h"

#define MAGIC (12289)
#define INIT_TMP_ALLOC (256)

typedef HashNode Node;
typedef HashTable Table;

static int hash(char *str){
    int i, ret = 0;
    for (i = 0;str[i];i++){
        ret *= 256;
        ret += str[i];
        ret %= MAGIC;
    }
    return ret;
}

int init_table(Table *table){
    table -> heads = malloc(sizeof(Node*) * MAGIC);
    if (!table -> heads) return MEMORY_ERROR;
    table -> tails = malloc(sizeof(Node*) * MAGIC);
    if (!table -> tails) return MEMORY_ERROR;
    table -> tmps = malloc(sizeof(Node*) * INIT_TMP_ALLOC);
    if (!table -> tmps) return MEMORY_ERROR;
    table -> tmp_size = 0;
    table -> tmpness = 0;
    return NO_ERROR;
}

Node *get_node(char *name, Table *table){
    int h = hash(name);
    Node *ret;
    ret = (table -> heads)[h];
    while (ret && !equal(name, ret -> name)){
        ret = ret -> next;
    }
    return ret;
}

Node *get_tmp_node(char *name, byte tmpness, Table *table){
    int h = hash(name);
    Node *ret;
    ret = (table -> heads)[h];
    if (ret && ret -> tmpness < tmpness) return NULL;
    while (ret && !equal(name, ret -> name)){
        ret = ret -> next;
        if (ret -> tmpness < tmpness) return NULL;
    }
    return ret;
}

void delete_node(Node *node, Table *table){
    int h = hash(node -> name);
    if (node -> next){
        node -> next -> prev = node -> prev;
    } else {
        (table -> tails)[h] = node -> prev;
    }
    if (node -> prev){
        node -> prev -> next = node -> next;
    } else {
        (table -> heads)[h] = node -> next;
    }
}

int insert_node(char *name, byte type, void *data, Table *table){
    int h = hash(name);
    //printf("ins: %s\n", name);
    Node *new;
    new = get_node(name, table);
    if (new != NULL){
        //printf("name = %s\n", name);
        return NAME_ERROR;
    }

    new = malloc(sizeof(Node));
    new -> name = malloc((sizeof(char) + 1) * strlen(name));
    strcpy(new -> name, name);
    new -> type = type;
    new -> data = data;
    new -> tmpness = 0;

    new -> next = NULL;
    if ((table -> tails)[h]){
        (table -> tails)[h] -> next = new;
    } else {
        (table -> heads)[h] = new;
    }
    new -> prev = (table -> tails)[h];
    (table -> tails)[h] = new;
    
    return NO_ERROR;
}

int insert_tmp_node(char *name, byte type, void *data, Table *table){
    int h = hash(name);
    //printf("ins(tmp): %s\n", name);
    Node *new;
    new = get_tmp_node(name, table -> tmpness, table);
    if (new != NULL){
        //printf("name(tmp) = %s\n", name);
        return NAME_ERROR;
    }

    new = malloc(sizeof(Node));
    new -> name = malloc((sizeof(char) + 1) * strlen(name));
    strcpy(new -> name, name);
    new -> type = type;
    new -> data = data;
    new -> tmpness = table -> tmpness;

    (table -> tmps)[table -> tmp_size] = new;
    (table -> tmp_size)++;

    new -> prev = NULL;
    new -> next = (table -> heads)[h];
    if ((table -> heads)[h]){
        (table -> heads)[h] -> prev = new;
    } else {
        (table -> tails)[h] = new;
    }
    (table -> heads)[h] = new;

    return NO_ERROR;
}

void start_tmp_session(Table *table){
    (table -> tmpness)++;
}

void end_tmp_session(Table *table){
    int tmpness = table -> tmpness, size = table -> tmp_size;
    while (size && (table -> tmps)[size - 1] -> tmpness == tmpness){
        delete_node((table -> tmps)[size - 1], table);
        size--;
    }
    table -> tmp_size = size;
    (table -> tmpness)--;
}
