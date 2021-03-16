#include "specific.h"
/* reference source:https://github.com/reeze/tipi/blob/master/book/chapt03/03-01-01-hashtable.markdown
                    http://www.cse.yorku.ca/~oz/hash.html*/

assoc* assoc_init(int keysize){
    int i;
    assoc *table = (assoc*)ncalloc(1,sizeof(assoc));
    /* Check the pass-in parameter. */
    if(keysize < 0)
        on_error("Function/assoc_init:Error of input parameter.\n");

    table->max_elem = INI_SIZE;
    table->used_elem = 0;
    table->sizeof_key = keysize;
    table->elem = (element **)ncalloc(INI_SIZE, sizeof(element*));
    for(i = 0; i < INI_SIZE;i++){
        table->elem[i] = NULL;
    }
    return table;
}

void assoc_insert(assoc** a, void* key, void* data){
    unsigned int index = find_index(*a, key);
    unsigned int i;
    /* Check the pass-in parameter. */
    if((a == NULL)||(key == NULL))
        on_error("Function/assoc_insert:Error of input parameter.\n");

    /* Check if it requires resize. */
    if((*a)->used_elem >= 0.75 * (*a)->max_elem)
        *a = table_resize(*a);

    /* Find if the same key already exists.
    If so, compare whether the data corresponding to the key is consistent.
    If they are consistent, return directly, or, updating the data before return.  */
    if((i = index_lookup(*a, key)) != (unsigned int)-1){
        if(assoc_lookup(*a, key) == data)
            return;
        else{
            (*a)->elem[i]->data = data;
            return;
        }
    }
    else{
    /* If the key doesn't exist in the table, find the next empty element to store.
    Find the 'next element' in circle.*/
        for(i = index; (*a)->elem[i]!=NULL; i = (i + 1) % (*a)->max_elem);
        (*a)->elem[i] = (element *)ncalloc(1, sizeof(element));
        (*a)->elem[i]->data = data;
        (*a)->elem[i]->key = key;
        (*a)->used_elem++;
        return;
    }
}

assoc* table_resize(assoc* a){
	unsigned long pre_size = a->max_elem;
    element **elem;
    element **pre_elem;
    unsigned long i = 0;
    /* Check the pass-in parameter. */
    if(a == NULL)
        on_error("Function/table_resize:Error of input parameter.\n");

    /* Check whether it is over the size of table, that is 'unsigned int'.*/
    if(2 * a->max_elem - 4294967295 >= 0)
        on_error("Error: Out of the HASH Table's capability.\n");
    /* Set a temporary pointer for the element of hash table. */
	a->max_elem = a->max_elem * 2;
	a->used_elem = 0;
	elem = (element **)ncalloc(a->max_elem, sizeof(element *));
	pre_elem = a->elem;
	a->elem = elem;
	for(i=0; i < pre_size; i++){
		if(pre_elem[i])
			assoc_insert(&a, pre_elem[i]->key, pre_elem[i]->data);
	}
	free(pre_elem);
	return a;
}

void* assoc_lookup(assoc* a, void* key){
    unsigned int i = INI_INDEX;
    /* Check the pass-in parameter. */
    if((a == NULL)||(key == NULL))
        on_error("Function/assoc_lookup:Error of input parameter.\n");

    if((i = index_lookup(a, key)) == (unsigned int)INI_INDEX)
        return NULL;
    else
        return a->elem[i]->data;
}

/* Return the index number of the element in hash table if the key exist, or return -1*/
unsigned int index_lookup(assoc* a, void* key){
    unsigned int index,i;
    /* Check the pass-in parameter. */
    if((a == NULL)||(key == NULL))
        on_error("Function/index_lookup:Error of input parameter.\n");

    index = find_index(a, key);
    if(a->sizeof_key == 0){
        for(i = index; a->elem[i] != NULL; i = (i + 1) %a->max_elem){
            if(strcmp(key, a->elem[index]->key) == 0)
                return index;
        }
        return INI_INDEX;
    }
    else{
        while(a->elem[index] != NULL){
            if(memcmp(key, a->elem[index]->key, a->sizeof_key) == 0)
                return index;
            index = (index + 1) % a->max_elem;
        }
        return INI_INDEX;
    }
}

unsigned int assoc_count(assoc* a){
    if(a == NULL)
        on_error("Function/assoc_count:Error of input parameter.\n");

    return a->used_elem;
}

void assoc_free(assoc* a){
    unsigned long i;

    if(a == NULL)
        on_error("Function/assoc_free:Error of input parameter.\n");

    for(i = 0; i < a->max_elem; i++)
        free(a->elem[i]);
    free(a->elem);
    free(a);
}

/* Choose the hash function based on the type of key. */
unsigned int find_index(assoc* a, void* key){
    if(a->sizeof_key == 0)
        return str_hash(a, key);
    else
        return else_hash(a, key);
}

/* Hash function for string. */
unsigned int str_hash(assoc* a, void *key){
	unsigned int hash = 5381;
	unsigned char *cur = key;

    while(*cur != '\0'){
        hash = (hash * 33 ^ *cur) % a->max_elem;
		cur++;
	}
	return hash;
}

/* Hash function for other data type.*/
unsigned int else_hash(assoc* a, void *key){
    int i = 0, cur;
    unsigned int hash = 5381;
    char *k = key;
    int *intg;
    /* Just to make it run faster when the input is integer. */
    if(a->sizeof_key == sizeof(int)){
        intg = (int *)key;
        return *intg % (a->max_elem);
    }
    else{
    /* If the input is neither string nor integer. It can also run well
    when the input is integer by the following method but is bit slower.*/
        while((cur = *k++) && (i < a->sizeof_key)){
            hash = (hash * 33 ^ cur);
            i++;
        }
    }
    return hash % (a->max_elem);
}
