#include <stdlib.h>

typedef struct pair pair;
struct pair{
    int key;
    int position;
};

typedef struct heap heap;
struct heap{
    pair *arr;
    int size;
    int capacity;
};

int parent(int i){
    return (i - 1) / 2;
}

int left_child(int i){
    return (2 * i + 1);
}

int right_child(int i){
    return (2 * i + 2);
}

heap *init_heap(int capacity){
    heap *h = malloc(sizeof(heap));
    h->arr = malloc(capacity * sizeof(pair));
    h->capacity = capacity;
    h->size = 0;
    return h;
}

heap *min_insert(heap *heap, int element, int position){
    if(heap->size == heap->capacity){
        return heap;
    }
    heap->size++;
    heap->arr[heap->size - 1].key = element;
    heap->arr[heap->size - 1].position = position;
    int curr = heap->size - 1;
    while(curr > 0 && heap->arr[parent(curr)].key > heap->arr[curr].key){
        pair temp = heap->arr[parent(curr)];
        heap->arr[parent(curr)] = heap->arr[curr];
        heap->arr[curr] = temp;
        curr = parent(curr);
    }
    return heap; 
}

heap *max_insert(heap *heap, int element, int position){
    if(heap->size == heap->capacity){
        return heap;
    }
    heap->size++;
    heap->arr[heap->size - 1].key = element;
    heap->arr[heap->size - 1].position = position;
    int curr = heap->size - 1;
    while(curr > 0 && heap->arr[parent(curr)].key < heap->arr[curr].key){
        pair temp = heap->arr[parent(curr)];
        heap->arr[parent(curr)] = heap->arr[curr];
        heap->arr[curr] = temp;
        curr = parent(curr);
    }
    return heap; 
}

heap *min_heapify(heap *heap, int index){
    if (heap->size <= 1){
        return heap;
    }
    int left = left_child(index);
    int right = right_child(index);
    int smallest = index;
    if(left < heap->size && heap->arr[left].key < heap->arr[index].key){
        smallest = left;
    }
    if(right < heap->size && heap->arr[right].key < heap->arr[smallest].key){
        smallest = right; 
    }
    if(smallest != index){ 
        pair temp = heap->arr[index];
        heap->arr[index] = heap->arr[smallest];
        heap->arr[smallest] = temp;
        heap = min_heapify(heap, smallest); 
    }
    return heap;
}

heap *max_heapify(heap *heap, int index){
    if (heap->size <= 1){
        return heap;
    }
    int left = left_child(index);
    int right = right_child(index);
    int largest = index;
    if(left < heap->size && heap->arr[left].key > heap->arr[index].key){
        largest = left;
    }
    if(right < heap->size && heap->arr[right].key > heap->arr[largest].key){
        largest = right; 
    }
    if(largest != index){ 
        pair temp = heap->arr[index];
        heap->arr[index] = heap->arr[largest];
        heap->arr[largest] = temp;
        heap = max_heapify(heap, largest); 
    }
    return heap;
}

heap *min_build(heap *heap){
    heap->size = heap->capacity;
    for(int i = heap->size / 2 - 1; i >= 0; i--){
        min_heapify(heap, i);
    }
    for(int i = 0; i < heap->capacity; i++){
        heap->arr[i].position = i;
    }
    return heap;
}

heap *max_build(heap *heap){
    heap->size = heap->capacity;
    for(int i = heap->size / 2 - 1; i >= 0; i--){
        max_heapify(heap, i);
    }
    for(int i = 0; i < heap->capacity; i++){
        heap->arr[i].position = i;
    }
    return heap;
}

heap *min_extract(heap* heap){
    if(!heap || heap->size == 0){
        return heap;
    }
    int size = heap->size;
    pair last_element = heap->arr[size-1];
    heap->arr[0] = last_element;
    heap->size--;
    size--;
    heap = min_heapify(heap, 0);
    return heap;
}

heap *max_extract(heap* heap){
    if(!heap || heap->size == 0){
        return heap;
    }
    int size = heap->size;
    pair last_element = heap->arr[size-1];
    heap->arr[0] = last_element;
    heap->size--;
    size--;
    heap = max_heapify(heap, 0);
    return heap;
}

pair get_root(heap *heap){
    return heap->arr[0];
}

int min_select(heap *h1, int k){
    int v;
    heap *h2 = init_heap(h1->capacity);
    min_insert(h2, (get_root(h1)).key, h2->size);
    int position;
    for(int i = 0; i < k-1; i++){
        position = get_root(h2).position;
        min_extract(h2);
        if(left_child(position) < h1->size){
            min_insert(h2, h1->arr[left_child(position)].key, left_child(position));
        }
        if(right_child(position) < h1->size){
            min_insert(h2, h1->arr[right_child(position)].key, right_child(position));
        }
    }
    v = get_root(h2).key;
    free(h2->arr);
    free(h2);
    return v;
}

int max_select(heap *h1, int k){
    int v;
    heap *h2 = init_heap(h1->capacity);
    max_insert(h2, (get_root(h1)).key, h2->size);
    int position;
    for(int i = 0; i < k-1; i++){
        position = get_root(h2).position;
        max_extract(h2);
        if(left_child(position) < h1->size){
            max_insert(h2, h1->arr[left_child(position)].key, left_child(position));
        }
        if(right_child(position) < h1->size){
            max_insert(h2, h1->arr[right_child(position)].key, right_child(position));
        }
    }
    v = get_root(h2).key;
    free(h2->arr);
    free(h2);
    return v;
}

int heap_select(int *a, int len, int k){
    int v;
    pair *input_to_pair = malloc(len * sizeof(pair));
    for(int i = 0; i < len; i++){
        input_to_pair[i].key = a[i];
        input_to_pair[i].position = i;
    }
    heap *h1 = init_heap(len);
    for(int i = 0; i < len; i++){
        h1->arr[i] = input_to_pair[i];
    }
    if(k <= len / 2){
        h1 = min_build(h1);
        free(input_to_pair);
        v = min_select(h1, k--);
        free(h1->arr);
        free(h1);
        return v;
    } else {
        h1 = max_build(h1);
        free(input_to_pair);
        v = max_select(h1, len - k + 1);
        free(h1->arr);
        free(h1);
        return v;
    }
}