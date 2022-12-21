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