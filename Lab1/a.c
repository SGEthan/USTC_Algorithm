#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <windows.h>
#define N 500000

typedef struct
{
    int *array;
    int length;
}bucket;

void random_compare(int n);
void nearly_sorted_gen(int *array, int n);
void nearly_reversed_sorted_gen(int *array, int n);
void compare(int *array, int n);
void rand_gen(int *array, int n);
void nearly_sorted_compare(int n);
void nearly_reversed_sorted_compare(int n);
void array_print(int *array, int n);
double time_sec();
void array_copy(int *array, int *array1, int n);
void Insert_Sort(int *array, int n);
void merge_sort(int *array, int l, int r);
void merge(int *array, int l, int r);
void quick_sort(int *array, int low, int high);
int partition(int *array, int low, int high);
void heapify(int *array, int n, int i);
void heap_sort(int *array, int n);
int getMax(int arr[], int n);
void countSort(int arr[], int n, int exp);
void radix_sort(int arr[], int n);
void bucket_sort(int arr[], int n);
void bucket_insert(bucket *bucket_list, int max, int data, int n);
int min_(int a, int b);

int main()
{
    printf("hello\n");
    // random_compare(50000);
    random_compare(100000);
    // random_compare(200000);
    // // random_compare(300000);
    // // random_compare(500000);

    // random_compare(100000);
    // nearly_sorted_compare(100000);
    // random_compare(100000);
    // nearly_reversed_sorted_compare(100000);
    // random_compare(100000);

    system("pause");
}

double time_sec()
{
    clock_t t = clock();
    return ((double)t) / CLOCKS_PER_SEC;
}

void array_copy(int *array, int *array1, int n)
{
    for (int i = 0; i < n; i++)
    {
        array1[i] = array[i];
    }
}

void array_print(int *array, int n)
{
    for (int i = 0; i < n; i++)
    {
        if(array[i]<0)
            printf("%d\n", array[i]);
    }
}

void rand_gen(int *array, int n)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++)
    {
        array[i] = rand();
    }
}

void Insert_Sort(int *array, int n)
{
    int i;
    for (i = 1; i < n; i++)
    {
        int tmp = array[i];
        int j = i;

        while (array[j-1] > tmp && j > 0)
        {
            array[j] = array[j - 1];
            j--;
        }
        array[j] = tmp;
    }
}

void merge_sort(int *array, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;

        merge_sort(array, l, m);
        merge_sort(array, m + 1, r);

        merge(array, l, r);
    }
}

void merge(int *array, int l, int r)
{
    int i, j, k;
    int m = l + (r - l) / 2;
    int n1 = m - l + 1; 
    int n2 =  r - m;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++) 
        L[i] = array[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = array[m + 1+ j]; 

    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2) 
    { 
        if (L[i] <= R[j]) 
        { 
            array[k] = L[i]; 
            i++; 
        } 
        else
        { 
            array[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    while (i < n1) 
    { 
        array[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    while (j < n2) 
    { 
        array[k] = R[j]; 
        j++; 
        k++; 
    } 
}

void quick_sort(int *array, int low, int high)
{
    if (low < high)
    {
        int pivotloc = partition(array, low, high);
        quick_sort(array, low, pivotloc - 1);
        quick_sort(array, pivotloc + 1, high);
    }
}

int partition(int *array, int low, int high)
{
    int pivotkey = array[low];
    int tmp;
    while (low < high)
    {
        while (low < high && array[high] >= pivotkey)
            --high;
        tmp = array[high];
        array[high] = array[low];
        array[low] = tmp;

        while (low < high && array[low] <= pivotkey)
            ++low;
        tmp = array[high];
        array[high] = array[low];
        array[low] = tmp;
    }
    return low;
}

void heapify(int *array, int n, int i)
{
    int largest = i;
    int l = 2*i + 1; 
    int r = 2*i + 2;
    int tmp;

    if (l < n && array[l] > array[largest]) 
        largest = l; 
  
    if (r < n && array[r] > array[largest]) 
        largest = r; 
  
    if (largest != i) 
    { 
        tmp = array[i];
        array[i] = array[largest];
        array[largest] = tmp;
        heapify(array, n, largest); 
    } 
}

void heap_sort(int *array, int n)
{
    int tmp;
    for (int i = n / 2 - 1; i >= 0; i--) 
        heapify(array, n, i); 
  
    for (int i=n-1; i>=0; i--) 
    { 
        tmp = array[i];
        array[i] = array[0];
        array[0] = tmp;
        heapify(array, i, 0); 
    } 
}

int getMax(int arr[], int n) 
{ 
    int mx = arr[0]; 
    for (int i = 1; i < n; i++) 
        if (arr[i] > mx) 
            mx = arr[i]; 
    return mx; 
} 

void countSort(int arr[], int n, int exp) 
{
    int *output = (int *)malloc(n * sizeof(int));
    int i, count[10] = {0}; 
  
    for (i = 0; i < n; i++) 
        count[ (arr[i]/exp)%10 ]++; 
  
    for (i = 1; i < 10; i++) 
        count[i] += count[i - 1];
  
    for (i = n - 1; i >= 0; i--) 
    { 
        output[count[ (arr[i]/exp)%10 ] - 1] = arr[i];
        count[ (arr[i]/exp)%10 ]--; 
    } 
  
    for (i = 0; i < n; i++) 
        arr[i] = output[i]; 
} 
  
void radix_sort(int arr[], int n) 
{ 
    int m = getMax(arr, n); 
    for (int exp = 1; m/exp > 0; exp *= 10) 
        countSort(arr, n, exp); 
}

void bucket_insert(bucket *bucket_list, int max, int data, int n)
{
    int i = min_((int)((float)data/(float)max * (float)n), n - 1);
    bucket_list[i].array = (int *)realloc(bucket_list[i].array, (bucket_list[i].length + 1) * sizeof(int));
    bucket_list[i].array[bucket_list[i].length] = data;
    bucket_list[i].length += 1;
}

void bucket_sort(int *array, int n)
{
    bucket *bucket_list = (bucket *)malloc(n * sizeof(bucket));
    int size = sizeof(bucket_list);
    for (int i = 0; i < n; i++)
    {
        bucket_list[i].array = (int *)malloc(sizeof(int));
        bucket_list[i].length = 0;
    }

    int max = getMax(array, n);

    for (int i = 0; i < n; i++)
        bucket_insert(bucket_list, max, array[i], n);

    for (int i = 0; i < n; i++)
        if (bucket_list[i].length > 0)
            quick_sort(bucket_list[i].array, 0, bucket_list[i].length);

    int index = 0;
    for (int i = 0; i < n; i++)
        if (bucket_list[i].length > 0)
            for (int j = 0; j < bucket_list[i].length; j++)
            {
                array[index] = bucket_list[i].array[j];
                index++;
            }
}       

int min_(int a, int b)
{
    return (a < b ? a : b);
}

void random_compare(int n)
{
    printf("Array size: %d (randomly generated)\n", n);
    int *array = (int *)malloc(n * sizeof(int));
    rand_gen(array, n);
    compare(array, n);
    free(array);
    printf("\n");
}

void nearly_sorted_compare(int n)
{
    printf("Array size: %d (nearly sorted)\n", n);
    int *array = (int *)malloc(n * sizeof(int));
    nearly_sorted_gen(array, n);
    compare(array, n);
    printf("\n");
}

void nearly_reversed_sorted_compare(int n)
{
    printf("Array size: %d (nearly reversed sorted)\n", n);
    int *array = (int *)malloc(n * sizeof(int));
    nearly_reversed_sorted_gen(array, n);
    compare(array, n);
    printf("\n");
}

void compare(int *array, int n)
{
    int *array1 = (int *)malloc(n * sizeof(int));
    clock_t start_t, end_t;
    LARGE_INTEGER time_start, time_end;
    double dqFreq;
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
	dqFreq=(double)f.QuadPart;
    double time_waste, run_time;
    printf("Sort_Algorithm\t\ttime_used(microseconds)\n", time_waste);

    // Insert sort
    array_copy(array, array1, n);
    // start_t = clock();
    QueryPerformanceCounter(&time_start);
    Insert_Sort(array1, n);
    // end_t = clock();
    // time_waste = (double)(end_t - start_t);
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("Insert_Sort\t\t %lf\n", run_time);

    // Merge sort
    array_copy(array, array1, n);
    // start_t = clock();
    QueryPerformanceCounter(&time_start);
    merge_sort(array1, 0, n-1);
    // end_t = clock();
    // time_waste = (double)(end_t - start_t);
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("Merge_Sort\t\t %lf\n", run_time);

    // Quick sort
    array_copy(array, array1, n);
    // start_t = clock();
    QueryPerformanceCounter(&time_start);
    quick_sort(array1, 0, n-1);
    array_print(array1, n);
    // end_t = clock();
    // time_waste = (double)(end_t - start_t);
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("Quick_Sort\t\t %lf\n", run_time);

    // Heap sort 
    array_copy(array, array1, n);
    // start_t = clock();
    QueryPerformanceCounter(&time_start);
    heap_sort(array1, n);
    // end_t = clock();
    // time_waste = (double)(end_t - start_t);
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("Heap_Sort\t\t %lf\n", run_time);

    // Radix sort 
    array_copy(array, array1, n);
    // start_t = clock();
    QueryPerformanceCounter(&time_start);
    radix_sort(array1, n);
    // end_t = clock();
    // time_waste = (double)(end_t - start_t);
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("Radix_Sort\t\t %lf\n", run_time);

    // Bucket sort 
    array_copy(array, array1, n);
    // start_t = clock();
    QueryPerformanceCounter(&time_start);
    bucket_sort(array1, n);
    // end_t = clock();
    // time_waste = (double)(end_t - start_t);
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("Bucket_Sort\t\t %lf\n", run_time);
    free(array1);
}

void nearly_sorted_gen(int *array, int n)
{
    srand((unsigned)time(NULL));
    int i;
    int slice = n/100;

    for (i = 0; i < n; i++)
    {
        array[i] = rand()%slice + (i/slice)*100;
    }
}

void nearly_reversed_sorted_gen(int *array, int n)
{
    srand((unsigned)time(NULL));
    int i;
    int slice = n/100;

    for (i = 0; i < n; i++)
    {
        array[i] = rand()%slice + ((n-i)/slice)*100;
    }
}

