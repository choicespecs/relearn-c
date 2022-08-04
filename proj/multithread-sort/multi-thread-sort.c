#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include<stdbool.h>
#include<pthread.h>


int * array;
int * first_half_array;
int * second_half_array;

int * copyArray(const int * arr, const int start, const int end, const int size)
{
    int copy_size = end - start;
    int * copy = calloc(copy_size, sizeof(int));
    int * begin_copy = copy;
    for (int i = 0; i < size; i++)
    {
        if ((i >= start) && (i < end))
           *copy++ = *arr;
        arr++;
    }
    return begin_copy;
}

void printArray(const int * arr, const int size) 
{
    printf("{ ");
    for (int i = 0; i < size; i++) 
    {
        if (i != (size - 1)) 
            printf("%i, ", *arr);
        else
            printf("%i ", *arr);

        arr++;
    }
    printf("}\n");
}

bool isInteger(const char* string, const int size) 
{
    for (int i = 0; i < size; i++)
    {
        if(!isdigit(*string))
            return false;

        string++;
    }
    return true;
}

#define ARRAY_SIZE 1000
#define FILE_SIZE 1024

int * createArray(int * size)
{
    FILE *fp;
    char * line = NULL;
    char * file = malloc(sizeof(char) * FILE_SIZE);
    int * array = calloc(ARRAY_SIZE, sizeof(int));
    int * start = array;
    size_t len = 0;
    ssize_t read;
    printf("::multi-thread-sort::\n");
    printf("input filename: ");
    scanf("%s", file);
    fp = fopen(file, "r");
    if (fp == NULL) 
    {
        printf("filename does not exist\n");
        exit(EXIT_FAILURE);
    }
    while ((read = getline(&line, &len, fp)) != -1)
    {
        *(line - (--read)) = '\0';
        if (!isInteger(line, read))
        {
            printf("ERROR::invalid input\n");
            if (line)
                free(line);
            exit(EXIT_FAILURE);
        }
        *array++ = atoi(line);
        (*size)++;
    }
    fclose(fp);
    free(file);
    if (line)
        free(line);
    return start;
}

int comp(const void *first, const void* second)
{
    int *a = (int*) first;
    int *b = (int*) second;

    return *a - *b;
}

struct thread_args {
    int * array;
    int size;
};

void * sort_thread_one(void * arguments)
{
    printf("\nEntered the first thread\n");
    struct thread_args *args = arguments;
    int * array = args->array;
    qsort(array, args->size, sizeof(int), comp);
    first_half_array = array;
    printf("Finished sorting half of array: ");
    printArray(first_half_array, args->size);
    pthread_exit(NULL);
    return NULL;
}

void * sort_thread_two(void * arguments)
{
    printf("\nEntered the second thread\n");
    struct thread_args *args = arguments;
    int * array = args->array;
    qsort(array, args->size, sizeof(int), comp);
    second_half_array = array;
    printf("Finished sorting half of array: ");
    printArray(second_half_array, args->size);
    pthread_exit(NULL);
}


int * merge(const int* arr1, const int arr1_size, const int* arr2, const int arr2_size)
{
    int * array = calloc(ARRAY_SIZE, sizeof(int));
    int * start = array;
    int i = 0, j = 0, k = 0;
    while (i < arr1_size && j < arr2_size)
    {
        if (*arr1 < *arr2)
        {
            *array++ = *arr1++;
            i++;
        }
        else
        {
            *array++ = *arr2++;
            j++;
        }
        k++;
    }
    while (i < arr1_size)
    {
        *array++ = *arr1++;
        i++;
    }

    while (j < arr2_size)
    {
        *array++ = *arr2++;
        j++;
    }
    return start;
}

int main() 
{
    int size = 0, half, copy_two_size;
    pthread_t thread1;
    pthread_t thread2;
    struct thread_args *args1 = malloc(sizeof(struct thread_args));
    struct thread_args *args2 = malloc(sizeof(struct thread_args));

    array = createArray(&size);
    printf("\nCreated Array from file: ");
    printArray(array, size);

    half = size / 2;
    copy_two_size = size - half;

    first_half_array = copyArray(array, 0, half, size);
    second_half_array = copyArray(array, half, size, size);

    args1->array = first_half_array;
    args1->size = half;

    args2->array=second_half_array;
    args2->size = copy_two_size;

    pthread_create(&thread1, NULL, sort_thread_one, args1);
    pthread_create(&thread2, NULL, sort_thread_two, args2);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    int * sorted = merge(first_half_array, half, second_half_array, copy_two_size);

    printf("\nReentered main thread and now merged array: ");
    printArray(sorted, size);

    exit(EXIT_SUCCESS);
}
