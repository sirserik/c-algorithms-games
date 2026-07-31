/* Глава 7. Замер: во сколько раз бинарный поиск быстрее линейного.
 *
 * Берём миллион отсортированных чисел и много раз ищем в нём последний
 * элемент — это худший случай для линейного поиска.
 *
 *   cc -std=c11 -Wall -Wextra -O2 07-bench.c -o 07-bench
 *   ./07-bench
 *
 * Абсолютные секунды зависят от машины, а вот порядок разницы — нет.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Линейный поиск: идём подряд, пока не найдём. */
int linear_search(const int a[], int n, int target)
{
    for (int i = 0; i < n; i++) {
        if (a[i] == target) {
            return i;
        }
    }
    return -1;
}

/* Бинарный поиск: каждый шаг отбрасывает половину зоны. */
int binary_search(const int a[], int n, int target)
{
    int low = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;   /* середина без переполнения */
        if (a[mid] == target) {
            return mid;
        } else if (a[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

int main(void)
{
    int n = 1000000;
    int *a = malloc((size_t)n * sizeof(int));
    if (a == NULL) {
        printf("Не хватило памяти\n");
        return 1;
    }
    for (int i = 0; i < n; i++) {
        a[i] = i * 2;                      /* массив уже отсортирован */
    }

    int queries = 10000;                   /* столько раз ищем */
    int target = (n - 1) * 2;              /* последний элемент — худший случай */

    clock_t t0 = clock();
    int found_lin = -1;
    for (int q = 0; q < queries; q++) {
        found_lin = linear_search(a, n, target);
    }
    double lin = (double)(clock() - t0) / CLOCKS_PER_SEC;

    t0 = clock();
    int found_bin = -1;
    for (int q = 0; q < queries; q++) {
        found_bin = binary_search(a, n, target);
    }
    double bin = (double)(clock() - t0) / CLOCKS_PER_SEC;

    printf("массив: %d элементов, %d поисков последнего элемента\n",
           n, queries);
    printf("линейный: %.4f c\n", lin);
    printf("бинарный: %.4f c\n", bin);
    if (bin > 0.0) {
        printf("разница:  в %.0f раз\n", lin / bin);
    }
    printf("(оба нашли индекс %d и %d)\n", found_lin, found_bin);

    free(a);
    return 0;
}
