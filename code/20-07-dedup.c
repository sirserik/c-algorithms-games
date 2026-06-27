#include <stdio.h>

/* Удаление дубликатов из ОТСОРТИРОВАННОГО массива на месте.
   Возвращает новую длину; уникальные элементы — в начале a. */
int dedup_sorted(int *a, int n)
{
    if (n == 0) {
        return 0;
    }
    int w = 1;                  /* w — куда писать следующий уникальный */
    for (int r = 1; r < n; r++) {   /* r — что читаем */
        if (a[r] != a[w - 1]) {     /* отличается от последнего записанного */
            a[w] = a[r];
            w++;
        }
    }
    return w;
}

int main(void)
{
    int a[] = {1, 1, 2, 2, 2, 3, 5, 5, 8};
    int n = (int)(sizeof a / sizeof a[0]);

    int m = dedup_sorted(a, n);

    printf("Уникальных: %d\n", m);
    for (int i = 0; i < m; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}
