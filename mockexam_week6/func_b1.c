#include "common.h"

int numdiff(const int l[], const int n)
{
    int i, j, k = 0, counter = 0;
    bool flag;
    if(n == 0){
        return 0;
    }
    for(i = 1; i < n; i++){
        flag = false;
        for(j = 0; j < i;j++){
            if(l[i] == l[j]){
                counter++;
                flag = true;
            }
        }
        if(flag == true){
            k++;
        }
    }
    if(k == 0){
        return n;
    }
    else{
        return (n - (2 * counter) / k + 1);
    }

}
