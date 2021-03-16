#include "specific.h"

assoc* assoc_init(int keysize){
    int i;
    assoc *table = (assoc*)ncalloc(1,sizeof(assoc));
    /* Check the pass-in parameter. */
    if(keysize < 0)
        on_error("Function/assoc_init:Error of input parameter.\n");

    table->max_elem = INI_SIZE;
    table->used_elem = 0;
    table->sizeof_key = keysize;
    table->tot_used = 0;
    table->elem1 = (element **)ncalloc(INI_SIZE, sizeof(element*));
    table->elem2 = (element **)ncalloc(INI_SIZE, sizeof(element*));
    for(i = 0; i < INI_SIZE;i++){
        table->elem1[i] = NULL;
        table->elem2[i] = NULL;
    }
    return table;
}

void assoc_insert(assoc** a, void* key, void* data){
    unsigned int index1 = find_index_1(*a, key);
    unsigned int index2 = find_index_2(*a, key);
    element* e, *f;
    /* Check the pass-in parameter. */
    if((a == NULL)||(key == NULL))
        on_error("Function/assoc_insert:Error of input parameter.\n");

    if((e = insert(&(*a)->elem1[index1],key,data)) == NULL){
        (*a)->used_elem++;
        return;
    }
    else{
        if((f = insert(&((*a)->elem2[index2]),e->key,e->data)) == NULL){
            (*a)->used_elem++;
            return;
        }
        else{
            if((e = insert(&((*a)->elem1[index1]),f->key,f->data)) == NULL){
                (*a)->used_elem++;
                return;
            }
            else{
                if((f = insert(&((*a)->elem2[index2]),e->key,e->data)) == NULL){
                    (*a)->used_elem++;
                    return;
                }
                table_resize(*a);
                assoc_insert(a, key, data);
            }
        }
    }
}

assoc* table_resize(assoc* a){
	unsigned long pre_size = a->max_elem;
    element **elem1, **elem2;
    element **pre_elem1, **pre_elem2;
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
	elem1 = (element **)ncalloc(a->max_elem, sizeof(element *));
    elem2 = (element **)ncalloc(a->max_elem, sizeof(element *));
	pre_elem1 = a->elem1;
    pre_elem2 = a->elem2;
	a->elem1 = elem1;
    a->elem2 = elem2;
	for(i=0; i < pre_size; i++){
		if(pre_elem1[i])
			assoc_insert(&a, pre_elem1[i]->key, pre_elem1[i]->data);
        if(pre_elem2[i])
    		assoc_insert(&a, pre_elem2[i]->key, pre_elem2[i]->data);
	}
	free(pre_elem1);
    free(pre_elem2);
	return a;
}

void* assoc_lookup(assoc* a, void* key){
    unsigned int index1, index2;
    /* Check the pass-in parameter. */
    if((a == NULL)||(key == NULL))
        on_error("Function/assoc_lookup:Error of input parameter.\n");
    printf("[lookup]%s\n", (char *)key);
    index1 = find_index_1(a, key);
    index2 = find_index_2(a, key);
    if(a->sizeof_key == 0){
        if(a->elem1[index1] != NULL)
            if(strcmp(key, a->elem1[index1]->key) == 0)
                return a->elem1[index1]->data;
        if(a->elem2[index2] != NULL)
            if(strcmp(key, a->elem2[index2]->key) == 0)
                return a->elem2[index2]->data;
    }
    else{
        if(a->elem1[index1] != NULL)
            if(memcmp(key, a->elem1[index1]->key, a->sizeof_key) == 0)
                return a->elem1[index1]->data;
        if(a->elem2[index2] != NULL)
            if(memcmp(key, a->elem2[index2]->key, a->sizeof_key) == 0)
                return a->elem2[index2]->data;
    }
    return NULL;
}

void assoc_free(assoc* a){
    unsigned long i;

    if(a == NULL)
        on_error("Function/assoc_free:Error of input parameter.\n");

    for(i = 0; i < a->max_elem; i++){
        free(a->elem1[i]);
        free(a->elem2[i]);
    }
    free(a->elem1);
    free(a->elem2);
    free(a);
}

unsigned int assoc_count(assoc* a){
    if(a == NULL)
        on_error("Function/assoc_count:Error of input parameter.\n");

    return a->used_elem;
}

element* insert(element** e, void* key, void* data){
    element *pre;
    if((*e) == NULL){
        *e = (element *)ncalloc(1, sizeof(element));
        (*e)->data = data;
        (*e)->key = key;
        return NULL;
    }
    else{
        pre = (*e);
        *e = (element *)ncalloc(1, sizeof(element));
        (*e)->data = data;
        (*e)->key = key;
        return pre;
    }
}

unsigned int find_index_1(assoc* a, void* key){
    unsigned int hash;
    unsigned int c;
    int i = 0;
    unsigned int *intg;
    char *k;

    if(a->sizeof_key == 0){
        hash = 5381;
        k = key;
        while(*k != '\0'){
            hash = (hash * 33 ^ *k) % a->max_elem;
    		k++;
    	}
    }
    else if(a->sizeof_key == sizeof(int)){
        intg = (unsigned int *)key;
        hash = *intg % (a->max_elem);
    }
    else{
        k = key;
        while((c = *k++) && (i < a->sizeof_key)){
            hash = (hash * 33 ^ c);
            i++;
        }
        hash = hash % (a->max_elem);
    }
    return hash;
}

unsigned int find_index_2(assoc* a, void* key){
    unsigned int hash = 0;
    unsigned int c;
    int i = 0;
    unsigned int *intg;
    char *k;

    if(a->sizeof_key == 0){
        hash = 5381;
        k = key;
        while(*k != '\0'){
            hash = ((*k++) + (hash << 6) + (hash << 16) - hash) % (a->max_elem);
    		k++;
    	}
    }
    else if(a->sizeof_key == sizeof(int)){
        intg = (unsigned int *)key;
        hash = ((*intg++) + (hash << 6) + (hash << 16) - hash) % (a->max_elem);
    }
    else{
        k = key;
        while((c = *k++) && (i < a->sizeof_key)){
            hash = ((*k++) + (hash << 6) + (hash << 16) - hash) % (a->max_elem);
            i++;
        }
    }
    return hash % (a->max_elem);
}
