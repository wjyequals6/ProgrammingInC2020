#include "common.h"

char* strnodupes(const char* s){
    char* ans = (char *)calloc(strlen(s) + 1, sizeof(s));
    int i, j, flag, k = 0;
    //printf("%s, : %d, %d\n",s,(int)sizeof(s) + 1, (int)strlen(s));
    for(i = 0; i < (int)strlen(s); i++){
        flag = 0;
        for(j = 0; j <= k; j++){
            if (*(ans + j) == *(s + i)){
                flag++;
            }
        }
        if (flag == 0){
            *(ans + k) = *(s + i);
            k++;
        }
    }
    *(ans + k) = '\0';
    //printf("ans : %s\n",ans);
    return ans;

}

/*

 gcc main_a1.c func_a2.c -o a2 -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -fsanitize=address -fsanitize=undefined -g3 -lm
*/
