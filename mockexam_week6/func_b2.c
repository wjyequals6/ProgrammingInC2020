#include "common.h"

int closestmean(const int l[], const int n)
{
    int ave, sum = 0, i, min, index = 0;
    for (i = 0; i < n; i++){
        sum = sum + l[i];
    }
    ave = sum / i;
    min = abs(l[0] - ave);
    for (i = 1; i < n; i++){

        if(abs(l[i] - ave) < min){
            min = abs(l[i] - ave);
            index = i;
        }
    }
    return l[index];
}
