#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include "general.h"
#define INI_INDEX -1
#define INI_SIZE 16
#define EXIT_FAILURE 1

typedef struct element{
    void* key;
    void* data;
}element;
typedef struct table{
    element** elem;
    unsigned int used_elem;
    unsigned int max_elem;
    int sizeof_key;
}assoc;

assoc* assoc_init(int keysize);
void assoc_insert(assoc** a, void* key, void* data);
unsigned int assoc_count(assoc* a);
void* assoc_lookup(assoc* a, void* key);
void assoc_free(assoc* a);

unsigned int find_index(assoc* a, void* key);
unsigned int str_hash(assoc* a, void *key);
unsigned int else_hash(assoc* a, void *key);
unsigned int index_lookup(assoc* a, void* key);
assoc* table_resize(assoc* a);
