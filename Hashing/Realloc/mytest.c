#include "specific.h"

#define WORDS 370119
#define NUMRANGE 100000

char* strduprev(char* str);
void test();

int main(void){

   static char strs[WORDS][50]={{0}};
   FILE *fp;
   char* tstr;
   void *p;
   unsigned int lngst;
   unsigned int j;
   assoc* a;
   static int i[WORDS];

   a = assoc_init(0);
   fp = nfopen("eng_370k_shuffle.txt", "rt");
   for(j=0; j<WORDS; j++){
      assert(assoc_count(a)==j);
      i[j] = j;
      if(fscanf(fp, "%s", strs[j])!=1){
         on_error("Failed to scan in a word?");
      }
      assoc_insert(&a, strs[j], &i[j]);
   }
   fclose(fp);


   lngst = 0;
   for(j=0; j<WORDS; j++){
      if(strlen(strs[j]) > lngst){
         tstr = strduprev(strs[j]);
         if(strcmp(tstr, strs[j])){
            if((p = assoc_lookup(a, tstr))!=NULL){
               lngst = strlen(tstr);
               printf("%s <-> %s = %d (%d in the file)\n", tstr, strs[j], lngst, *(int*)p);
            }
         }
         free(tstr);
      }
   }
   assoc_free(a);


   srand(time(NULL));
   a = assoc_init(sizeof(int));
   for(j=0; j<NUMRANGE; j++){
      i[j] = rand()%NUMRANGE;
      assoc_insert(&a, &i[j], NULL);
   }
   printf("%d unique numbers out of %d\n", assoc_count(a), j);

   assoc_free(a);

   return 0;
}

/* Make a copy, reversed */
char* strduprev(char* str)
{
   int i, j;
   char* t;
   j = strlen(str);
   t = ncalloc(j+1, 1); /* Add null char */
   strcpy(t, str);
   for(i=0, j--; i<j; i++,j--){
      /* Swap using bit-twiddling */
      t[i] ^= t[j];
      t[j] ^= t[i];
      t[i] ^= t[j];
   }
   return t;
}

void test(){

}
