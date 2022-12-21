#include "swap.h"
#include "partition.h"

void insertion_sort(int *a, int p, int q){
    int j;
    for(int i = p + 1; i <= q; i++){
        j = i;
        while(j > p && a[j - 1] > a[j]){
            swap(&a[j], &a[j - 1]);
            j--;
        }
    }
}

void mm(int *a, int p, int q){
    int i = p;
    int last = q - (q - p) % 5;
    while(i < q){
        if(i == last){
            insertion_sort(a, i, q);
        } else {
            insertion_sort(a, i, (i + 4));
        }
        i += 5;
    }
    if(q - p >= 5){
        i = p + 2;
        int c = 0;
        while(i <= q){
            swap(&a[i], &a[p + c++]);
            if(i < last - 3){
                i += 5;
            } else {
                i += 3 + (q - last) / 2;
                swap(&a[i], &a[p + c]);
                break;
            }
        }
        mm(a, p, p + c);
    } else {
        swap(&a[p], &a[(q - p) / 2 + p]);
    }
}

int mms(int *a, int p, int q, int i){
    int first = p;
    int last = q - (q - p) % 5;
    while(first < q){
        if(first == last){
            insertion_sort(a, first, q);
        } else {
            insertion_sort(a, first, (first + 4));
        }
        first += 5;
    }
    if(q - p < 5){
        return a[i];
    }
    int median = p + 2;
    int c = 0;
    while(median <= q){
        swap(&a[median], &a[p + c++]);
        if(median < last - 3){
            median += 5;
        } else {
            median += 3 + (q - last) / 2;
            swap(&a[median], &a[p + c]);
            break;
        }
    }
    mm(a, p, p + c);
    int r = partition(a, p, q);
    if(i == r){
        return a[r];
    }
    else if(i < r){
        return mms(a, p, r - 1, i);
    } else {
        return mms(a, r + 1, q, i);
    }
}