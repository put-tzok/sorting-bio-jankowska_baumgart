#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned int ns[] = {1000, 10000, 20000, 30000, 40000, 42500};
int min_argument;

void fill_increasing(int *t, unsigned int n) {
    for (int i=0; i<n; i++){
      t[i] = i;
    }
}

void fill_decreasing(int *t, unsigned int n) {
    int zmienna_dec = 0;
    for (int i=n; i>0; i--){
      t[zmienna_dec] = i;
      zmienna_dec++;
    }
}

void fill_vshape(int *t, unsigned int n) {
    int pierwszy = 0; int ostatni = n-1;
    int zmiana = 1;
    for (int i = n; i > 0; i--) {
    if (zmiana == 1) {
        t[pierwszy] = i;
        pierwszy++;
    }
    else {
        t[ostatni] = i;
        ostatni--;
    }
    zmiana *= -1;
    }
}
int minimalarg (int *t, int min_n, int max_n){
    min_argument = min_n;
    for (int i=min_n; i<max_n; i++){
      if (t[i]<t[min_argument]){
        min_argument=i;
      }
    }
    return min_argument;
}

void swap(int *t, int i,int j){
    int tmp = t[i];
    t[i]=t[j];
    t[j]=tmp;
}
void selection_sort(int *t, unsigned int n) { 
    for (int i=0; i<n; i++){
        int j = minimalarg(t,i,n);
        swap(t,i,j);
    }
}


void insertion_sort(int *t, unsigned int n) { 
    for (int j=1; j<n; j++){
      int key = t[j];
      int i = j-1;
      while ((i>= 0) && (t[i]>key)) {
        t[i+1] = t[i];
        i = i-1;
      }
       t[i + 1] = key;
    }
}

int random (int p, int r){
    int losowa = rand()%(r-p+1);
    return losowa+p;
}

int partition (int *t, int p, int r){
  int x = t[r];
  int i = p-1;
  for (int j=p; j<r; j++){
    if (t[j]<=x){
      i = i+1;
      swap(t, i, j);
    }
  }
  i = i+1;
  swap (t, i, r);
  return i;
}

int random_partition (int *t, int p, int r){
  int i = random(p, r); 
  swap(t, i, r);
  return partition(t, p, r);
}

void sorting (int *t, int p, int r){
  if (p<r){
    int q = partition(t, p, r);
    sorting (t, p, q-1); 
    sorting (t, q+1, r);
  }
}

void quick_sort(int *t, unsigned int n) { 
    sorting(t, 0, n-1);
}

int size = 0;
void heap_add(int *heap, int v) {
    heap[size] = v;
    heap_up(heap, size);
    size = size + 1;
}
void heap_up(int *heap, int index){
    if (index > 0) {
        int parent = (index - 1) / 2; 
        if (heap[index] < heap[parent]) {
          swap(heap, index, parent);
          heap_up(heap, parent);
        }
    }
}
int heap_poll(int *heap){
  int v = heap[0];
  size = size - 1;
  heap[0] = heap[size];
  heap_down(heap, 0, size);
  return v;
}

void heap_down(int *heap, int index){
  int l = index*2 + 1;
  int r = index*2 + 2;
  if (l < size) { ////CZY TO JEST DOBRZE?
    if (r >= size || heap[l] < heap[r]) {
      min_argument = l;
    }
    else {min_argument = r;}
    if (heap[min_argument] < heap[index]){
      swap(heap, min_argument, index);
      heap_down(heap, min_argument);
    }
  }
}

void heap_sort(int *t, unsigned int n) { 
  int heap[n];  
  for (int i=0; i<n; i++) {
    heap_add(heap, t[i]);
  }
  for (int i=0; i<n; i++) {
    t[i] = heap_poll(heap);
  }
}

void fill_random(int *t, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        t[i] = rand();
    }
}

void is_random(int *t, unsigned int n) { 
    return;
}

void is_increasing(int *t, unsigned int n) {
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] > t[i - 1]);
    }
}

void is_decreasing(int *t, unsigned int n) { 
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] < t[i - 1]);
    }
}

void is_vshape(int *t, unsigned int n) {
    int *begin = t;
    int *end = t + n - 1;

    while (end - begin > 1) {
        assert(*begin > *end);
        begin++;
        assert(*end > *begin);
        end--;
    }
}

void is_sorted(int *t, unsigned int n) {
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] >= t[i - 1]);
    }
}

void (*fill_functions[])(int *, unsigned int) = { fill_random, fill_increasing, fill_decreasing, fill_vshape };
void (*check_functions[])(int *, unsigned int) = { is_random, is_increasing, is_decreasing, is_vshape };
void (*sort_functions[])(int *, unsigned int) = { selection_sort, insertion_sort, quick_sort, heap_sort };

char *fill_names[] = { "Random", "Increasing", "Decreasing", "V-Shape" };
char *sort_names[] = { "SelectionSort", "InsertionSort", "QuickSort", "HeapSort" };

int main() {
    srand(time(NULL));
    for (unsigned int i = 0; i < sizeof(sort_functions) / sizeof(*sort_functions); i++){
        void (*sort)(int *, unsigned int) = sort_functions[i];

        for (unsigned int j = 0; j < sizeof(fill_functions) / sizeof(*fill_functions); j++) {
            void (*fill)(int *, unsigned int) = fill_functions[j]; 
            void (*check)(int *, unsigned int) = check_functions[j];

            for (unsigned int k = 0; k < sizeof(ns) / sizeof(*ns); k++) {
                unsigned int n = ns[k];
                int *t = malloc(n * sizeof(*t));

                fill(t, n);
                check(t, n);

                clock_t begin = clock();
                sort(t, n);
                clock_t end = clock();
                is_sorted(t, n);

                printf("%s\t%s\t%u\t%f\n", sort_names[i], fill_names[j], n, (double)(end - begin) / (double) CLOCKS_PER_SEC);
                free(t);
            }
        }
    }
    return 0;
}
