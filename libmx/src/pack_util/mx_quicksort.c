#include "libmx.h"

static void skip(char *pivot, char **arr, int *i, int *j) {
    while (mx_strlen(arr[*i]) < mx_strlen(pivot))
        (*i)++;
    while (mx_strlen(arr[*j]) > mx_strlen(pivot))
        (*j)--;
}

static void swap(char **s1, char **s2) {
    char *tmp = *s1;
    *s1 = *s2;
    *s2 = tmp;
}

static void compare_and_swap(char **arr, int *i, int *j, int *shifts_number) {
    if (*i <= *j) {
        if ((*i != *j) && (mx_strlen(arr[*i]) != mx_strlen(arr[*j]))) {
            swap(&arr[*i], &arr[*j]);
            (*shifts_number)++;
        }
        (*i)++;
        (*j)--;
    }
}

int mx_quicksort(char **arr, int left, int right) {
    int shifts_number = 0;
    char *pivot = NULL;
    int i = 0;
    int j = 0;

    if (!arr)
        return -1;
    if (left < right) {
        pivot = arr[left + (right - left) / 2];
        i = left;
        j = right;
        while (i <= j) {
            skip(pivot, arr, &i, &j);
            compare_and_swap(arr, &i, &j, &shifts_number);
        }
        shifts_number += mx_quicksort(arr, left, j);
        shifts_number += mx_quicksort(arr, i, right);
    }
    return shifts_number;
}
