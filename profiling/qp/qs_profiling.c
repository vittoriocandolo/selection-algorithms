#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "pcg_basic.h"

#include "quick_select.h"
#include "quick_select_r.h"

double duration(struct timespec start, struct timespec end){
    return end.tv_sec - start.tv_sec + ((end.tv_nsec - start.tv_nsec ) / (double) 1000000000.0);
}

double get_resolution(){
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    do {
        clock_gettime(CLOCK_MONOTONIC, &end);
    } while (duration(start, end) == 0.0);
    return duration(start, end);
}

int main(){
    double r = get_resolution();
    double e = 1001.0;
    double t_min = r * e;
    // printf("%f, %f", r , t_min);
    srand(time(NULL)); // seed #1
    FILE *q_out = fopen("qs.csv", "w");
    fprintf(q_out, "n,t,a\n");
    FILE *qr_out = fopen("qs_r.csv", "w");
    fprintf(qr_out, "n,t,a\n");
    int m = 10; // numero di input per dimensione (campione)
    int n = 500; // numero campioni (dimensioni input)
    int n_min = 100; // campione n0
    int n_max = 5000000; // campione n499
    // parametri formula esponenziale
    int a = n_min;
    long double b = logl((long double) (n_max / n_min)) / ((n - 1) * logl((long double) 2.0));
    int size[n]; // vettore che contiene i campioni (possibili lunghezze di input)
    for(int i = 0; i < n; i++){
        size[i] = (int) (a * pow((long double) 2.0, b * i));
    }
    /*
    for(int i = 0; i < n; i++){
    	printf("%d\n", size[i]);
    }
    */
    int s;
    struct timespec start, end;
    // seed #2
    pcg32_random_t rng1;
    pcg32_srandom_r(&rng1, time(NULL), (intptr_t)&rng1);
    for(int i = 0; i < n; i++){
        s = size[i];
    	int *input = malloc(s * sizeof(int));
    	for(int j = 0; j < m; j++){
    	    for(int k = 0; k < s; k++){
    	    	// metodo della moltiplicazione
	    		double d = ldexp(pcg32_random_r(&rng1), -32);
	    		input[k] = (round((d * (15 * s))) * pow(-1, rand()));
	    	}
	    	/*
	    	for(int l = 0; l < s; l++){
	        	printf("%d\n", input[l]);
	    	}
	    	break;
	    	*/
	    	double p = ldexp(pcg32_random_r(&rng1), -32);
	    	int k = p * s; // 0 <= k < s
	    	int c = 0;
	    	// timer init
        	clock_gettime(CLOCK_MONOTONIC, &start);
        	do {
        		quick_select(input, 0, s - 1, k);
		    	clock_gettime(CLOCK_MONOTONIC, &end);
		    	c++;
        	} while(duration(start, end) < t_min);
            fprintf(q_out, "%d,%lf,%s\n", s, (duration(start, end)) / c, "Naive");
	    	c = 0;
        	clock_gettime(CLOCK_MONOTONIC, &start);
        	do {
        		quick_select_r(input, 0, s - 1, k);
		    	clock_gettime(CLOCK_MONOTONIC, &end);
		    	c++;
        	} while(duration(start, end) < t_min);
	    	fprintf(qr_out, "%d,%lf,%s\n", s, (duration(start, end)) / c, "Random Pivot");
		}
		free(input);
		// break;
	}
    fclose(q_out);
    fclose(qr_out);
}