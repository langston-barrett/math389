#ifndef _mergesort_h
#define _mergesort_h

int max(int i, int j);
int min(int i, int j);

void mer(int *arr, int* dest, int bin_size, int portion);
void merge(int *arr, int n, int *dest, int bin_size);
void merge_sort(int *arr, int n);
void print_array(int *arr, int n);

#endif
