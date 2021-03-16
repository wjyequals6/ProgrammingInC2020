#include "stringlist.h"
/* Reference : Some functions was copied from Neill's Github : arraylist. */

list* init_list(void){
   list* l = (list*)ncalloc(sizeof(list), 1);
   l->size = 0;
   l->start = NULL;
   l->current = l->start;

   return l;
}

str* init_str(char* c){
    str* s = (str*)ncalloc(sizeof(str), 1);
    s->next = NULL;
    if(c){
        strcpy(s->word, c);
    }
    return s;
}

int size(list* l){
   return l->size;
}

bool to_next_word(list* l){

    if(l){
        l->current = l->current->next;
        /*printf("%s\n", l->current->word);*/
        return true;
    }
    return false;
}

char* cur_word(list* l){
    return l->current->word;
}

int cur_index(char c) {
    return toupper(c) - 'A';
}

bool add(list* l, char* c){
   str* s;
   if(l){
      s = init_str(c);
      if(l->start == NULL){
          l->start = s;
          l->current = l->start;
      }
      else{
          l->current->next = s;
          to_next_word(l);
      }
      l->size = l->size + 1;
      return true;
   }
   return false;
}

bool free_list(list* l){
   if(l){
      str* tmp;
      str* p = l->start;
      while(p != NULL){
         tmp = p->next;
         free(p);
         p = tmp;
      }
      free(l);
      return true;
   }
   on_error("Error : [print_list] list is empty.\n");
   return false;
}

bool print_list(list* l) {
    if(l){
        printf("————String List(Size: %d)————\n",size(l));
        if(l->start){
            l->current = l->start;
            do{
                printf("%s\n",cur_word(l));
                to_next_word(l);
            }while(l->current);
        }
        printf("————END————\n");
        return true;
    }
    on_error("Error : [print_list] list is empty.\n");
    return false;
}
