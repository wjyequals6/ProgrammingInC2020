#pragma once

#include "general.h"
#define MAX_EACH_WORD 20

typedef struct str{
    char word[MAX_EACH_WORD];
    struct str* next;
}str;

typedef struct list{
    int size;
    str* start;
    str* current;
}list;

list* init_list(void);
str* init_str(char* c);
int size(list* l);
bool to_next_word(list* l);
char* cur_word(list* l);
int cur_index(char c);
bool add(list* l, char* c);
bool free_list(list* l);
bool print_list(list* l);
