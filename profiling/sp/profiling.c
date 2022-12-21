#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#include "pcg_basic.h"

#include "quick_select.h"
#include "heap_select.h"
#include "median_of_medians_select.h"

struct thread_data{
    double t_min;
    int *input, s, k;
    FILE *out;
};

void *quick_function(void *thread_data);
void *heap_function(void *thread_data);
void *mms_function(void *thread_data);

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
    FILE *h_out = fopen("hs.csv", "w");
    fprintf(h_out, "n,t,a\n");
    FILE *m_out = fopen("ms.csv", "w");
    fprintf(m_out, "n,t,a\n");
    int m = 10; // numero di input per dimensione (campione)
    int n = 100; // numero campioni (dimensioni input)
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
    // pthread init
    pthread_t thread_quick, thread_heap, thread_mms;
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
            // thread #1
            struct thread_data quick_data;
            quick_data.t_min = t_min;
            quick_data.input = input;
            quick_data.s = s;
            quick_data.k = k;
            quick_data.out = q_out;
            pthread_create(&thread_quick, NULL, quick_function, (void *)&quick_data);
            // thread #2
            struct thread_data heap_data;
            heap_data.t_min = t_min;
            heap_data.input = input;
            heap_data.s = s;
            heap_data.k = k;
            heap_data.out = h_out;
            pthread_create(&thread_heap, NULL, heap_function, (void *)&heap_data);
            // thread #3
            struct thread_data mms_data;
            mms_data.t_min = t_min;
            mms_data.input = input;
            mms_data.s = s;
            mms_data.k = k;
            mms_data.out = m_out;
            pthread_create(&thread_mms, NULL, mms_function, (void *)&mms_data);
            // pthread join
            pthread_join(thread_quick, NULL);
            pthread_join(thread_heap, NULL);
            pthread_join(thread_mms, NULL);
        }
        free(input);
        // break;
    }
    fclose(q_out);
    fclose(h_out);
    fclose(m_out);
}

void *quick_function(void *thread_data){
    int c = 0;
    // timer init
    struct timespec start, end;
    struct thread_data *data = thread_data;
    clock_gettime(CLOCK_MONOTONIC, &start);
    do {
        quick_select(data->input, 0, data->s - 1, data->k);
        clock_gettime(CLOCK_MONOTONIC, &end);
        c++;
    } while(duration(start, end) < data->t_min);
    fprintf(data->out, "%d,%lf,%s\n", data->s, (duration(start, end)) / c, "Quick Select");
    // fprintf(q_out, "%d,%d,%d,%lf\n", s, k, q_v, (duration(start, end)) / c);
    return NULL;
}

void *heap_function(void *thread_data){
    int c = 0;
    // timer init
    struct timespec start, end;
    struct thread_data *data = thread_data;
    clock_gettime(CLOCK_MONOTONIC, &start);
    do {
        heap_select(data->input, data->s, data->k);
        clock_gettime(CLOCK_MONOTONIC, &end);
        c++;
    } while(duration(start, end) < data->t_min);
    fprintf(data->out, "%d,%lf,%s\n", data->s, (duration(start, end)) / c, "Heap Select");
    // fprintf(h_out, "%d,%d,%d,%lf\n", s, k, h_v, (duration(start, end)) / c);
    return NULL;
}

void *mms_function(void *thread_data){
    int c = 0;
    // timer init
    struct timespec start, end;
    struct thread_data *data = thread_data;
    clock_gettime(CLOCK_MONOTONIC, &start);
    do {
        mms(data->input, 0, data->s - 1, data->k);
        clock_gettime(CLOCK_MONOTONIC, &end);
        c++;
    } while(duration(start, end) < data->t_min);
    fprintf(data->out, "%d,%lf,%s\n", data->s, (duration(start, end)) / c, "MoM Select");
    // fprintf(m_out, "%d,%d,%d,%lf\n", s, k, m_v, (duration(start, end)) / c);
    return NULL;
}