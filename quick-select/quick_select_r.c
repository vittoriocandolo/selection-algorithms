#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int h_partition(int *a, int p, int q){
	int pivot = a[p];
	int i = p - 1, j = q + 1;
	while(1){
		do {
			i++;
		} while (a[i] < pivot);
		do {
			j--;
		} while(a[j] > pivot);

		if(i >= j){
			return j;
		}
		int temp = a[i];
		a[i] = a[j];
		a[j] = temp;
	}
}

int partition_r(int *a, int p, int q){
	srand(time(0));
	int random = p + rand() % (q - p);
	int temp = a[random];
	a[random] = a[p];
	a[p] = temp;
	return h_partition(a, p, q);
}

int quick_select_r(int *a, int p, int q, int i){
    if(p == q){
        return a[p];
    } else {
        int r = partition_r(a, p, q);
        if(i == r){
            return a[r];
        } else if(i < r){
           return quick_select_r(a, p, r - 1, i);
        } else {
            return quick_select_r(a, r + 1, q, i);
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
    printf("%d", quick_select_r(n, 0, size - 1, k));
    return 0;
}