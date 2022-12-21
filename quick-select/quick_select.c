#include <stdio.h>

#define SIZE 10000

int scan_array(int *a){
    char line[SIZE];
    scanf("%[^\n]", line);
    int size = 0, offset = 0, numFilled, n;
    do {
        numFilled = sscanf(line + offset, "%d%n", &(a[size]), &n);
        if(numFilled > 0){
            size++;
            offset += n;
        }
    } while(numFilled > 0);
    return size;
}

void swap(int *x, int *y){
    int temp = *x;
    *x = *y;
    *y = temp;
}

int partition(int *a, int p, int q){
    int i = p - 1;
    int x = a[q];
    for(int j = p; j <= q; j++){
        if(a[j] <= x){
            swap(&a[++i], &a[j]);
        }
    }
    return i;
}

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

int main(){
    int n[SIZE];
    int size;
    int k;
    size = scan_array(n);
    scanf("%d", &k);
    k--;
    printf("%d", quick_select(n, 0, size - 1, k));
    return 0;
}