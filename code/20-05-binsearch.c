#include <stdio.h>

/* Бинарный поиск в отсортированном массиве.
   Возвращает индекс target или -1, если не найдено. */
int binary_search(const int *a, int n, int target)
{
    int lo = 0;
    int hi = n - 1;             /* оба конца ВКЛЮЧИТЕЛЬНО */
    while (lo <= hi) {          /* пока отрезок непустой */
        int mid = lo + (hi - lo) / 2;   /* середина без переполнения */
        if (a[mid] == target) {
            return mid;
        } else if (a[mid] < target) {
            lo = mid + 1;       /* искомое правее */
        } else {
            hi = mid - 1;       /* искомое левее */
        }
    }
    return -1;
}

int main(void)
{
    int a[] = {-7, -3, 0, 2, 5, 8, 11, 17, 42};
    int n = (int)(sizeof a / sizeof a[0]);

    int tests[] = {5, 42, -7, 100, 0};
    int tn = (int)(sizeof tests / sizeof tests[0]);

    for (int i = 0; i < tn; i++) {
        int idx = binary_search(a, n, tests[i]);
        if (idx >= 0) {
            printf("%4d найдено на индексе %d\n", tests[i], idx);
        } else {
            printf("%4d не найдено\n", tests[i]);
        }
    }
    return 0;
}
