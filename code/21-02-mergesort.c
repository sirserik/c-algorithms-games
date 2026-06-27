#include <stdio.h>
#include <stdlib.h>

/* Слияние двух отсортированных половин a[lo..mid] и a[mid+1..hi]
   через временный буфер tmp. Берём меньший из голов и пишем по
   порядку — отсюда стабильность сортировки. */
void merge(int a[], int tmp[], int lo, int mid, int hi)
{
    int i = lo;          /* бегунок по левой половине */
    int j = mid + 1;     /* бегунок по правой половине */
    int k = lo;          /* куда пишем в tmp */

    while (i <= mid && j <= hi) {
        if (a[i] <= a[j]) {     /* <= , а не <  — это и даёт стабильность */
            tmp[k++] = a[i++];
        } else {
            tmp[k++] = a[j++];
        }
    }
    while (i <= mid) {          /* хвост левой половины */
        tmp[k++] = a[i++];
    }
    while (j <= hi) {           /* хвост правой половины */
        tmp[k++] = a[j++];
    }
    for (int t = lo; t <= hi; t++) {   /* копируем обратно в a */
        a[t] = tmp[t];
    }
}

/* Сортировка слиянием: делим пополам, сортируем половины, сливаем. */
void merge_sort(int a[], int tmp[], int lo, int hi)
{
    if (lo >= hi) {
        return;
    }
    int mid = lo + (hi - lo) / 2;   /* так не переполнится при больших lo,hi */
    merge_sort(a, tmp, lo, mid);
    merge_sort(a, tmp, mid + 1, hi);
    merge(a, tmp, lo, mid, hi);
}

int main(void)
{
    int a[] = {5, 2, 9, 1, 5, 6, 3, 8, 7, 0};
    int n = (int)(sizeof a / sizeof a[0]);

    /* один общий временный буфер на весь алгоритм */
    int *tmp = malloc((size_t)n * sizeof(int));
    if (tmp == NULL) {
        printf("Не хватило памяти\n");
        return 1;
    }

    merge_sort(a, tmp, 0, n - 1);

    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    free(tmp);
    return 0;
}
