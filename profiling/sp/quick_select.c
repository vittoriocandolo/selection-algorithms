#include "swap.h"
#include "partition.h"

int quick_select(int *a, int p, int q, int i){
    if(p == q){
        return a[p];
    } else {
        int r = partition(a, p, q);
        if(i == r){
            return a[r];
        } else if(i < r){
           return quick_select(a, p, r - 1, i);
        } else {
            return quick_select(a, r + 1, q, i);
        }
    }
}