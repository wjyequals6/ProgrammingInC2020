#include "common.h"

bool struses(const char* s1, const char* s2){
    int len1 = 0, len2 = 0;
    int i, j, flag = 0;
    while (*(s1+len1) != '\0'){
        len1++;
    }
    while (*(s2+len2) != '\0'){
        len2++;
    }
    for (i = 0; i < len1; i++){
        for(j = 0; j < len2; j++){
            if(*(s1 + i) == *(s2 + j)){
                flag++;
            }
        }
    }
    if (flag == len1){
        return true;
    }
    else{
        return false;
    }

}
