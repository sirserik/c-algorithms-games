#include <stdio.h>

/* Обмен двух int через указатели (см. главу 3). */
void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

/* Разбиение Ломуто: опорный — последний элемент a[hi].
   Расставляем меньшие опорного слева, большие — справа,
   и возвращаем итоговую позицию опорного. */
int partition(int a[], int lo, int hi)
{
    int pivot = a[hi];         /* опорный (pivot) */
    int i = lo - 1;            /* граница «меньших опорного» */

    for (int j = lo; j < hi; j++) {
        if (a[j] < pivot) {    /* нашли элемент меньше опорного */
            i++;
            swap(&a[i], &a[j]);
        }
    }
    swap(&a[i + 1], &a[hi]);   /* опорный встаёт на своё место */
    return i + 1;
}

/* Быстрая сортировка: разбили относительно опорного и
   рекурсивно сортируем левую и правую части. */
void quicksort(int a[], int lo, int hi)
{
    if (lo >= hi) {            /* 0 или 1 элемент — уже отсортировано */
        return;
    }
    int p = partition(a, lo, hi);
    quicksort(a, lo, p - 1);
    quicksort(a, p + 1, hi);
}

int main(void)
{
    int a[] = {5, 2, 9, 1, 5, 6, 3, 8, 7, 0};
    int n = (int)(sizeof a / sizeof a[0]);

    quicksort(a, 0, n - 1);

    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}
