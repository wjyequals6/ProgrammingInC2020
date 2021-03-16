#include "general.h"
/* Reference : Some functions was copied from Neill's Github with the same name. */
void on_error(const char* s)
{
   fprintf(stderr, "%s\n", s);
   exit(EXIT_FAILURE);
}

void* ncalloc(int n, size_t size)
{
   void* v = calloc(n, size);
   if(v==NULL){
      on_error("Cannot calloc() space");
   }
   return v;
}

void* nfopen(char* fname, char* mode)
{
   FILE* fp;
   fp = fopen(fname, mode);
   if(!fp){
      on_error("Cannot open file");
   }
   return fp;
}

bool is_number(char* s){
    int i, count = 0;
    if(strspn(s, "0123456789.") != strlen(s)) return false;
    for(i = 0; i < (int)strlen(s); i++){
        if(*(s + i) == '.') count++;
    }
    if(count > 1) return false;
    /* interger */
    if(strspn(s, "0123456789") == strlen(s)) return true;
    /* float */
    if(fabs(atof(s) - 0) > 1e-5) return true;
    return false;
}

float get_number(char* s){
    if(is_capital_letter(s)){
        return toupper(*s) - 'A';
    }
    if(is_number(s)){
        return atof(s);
    }
    return -1;
}

bool is_capital_letter(char* s){
    if(strlen(s) == 1 && (toupper(*s) >= 'A') && (toupper(*s) <= 'Z')){
        return true;
    }
    return false;
}
