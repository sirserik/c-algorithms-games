/* Глава 7. Замер: во сколько раз бинарный поиск быстрее линейного.
 *
 * Берём миллион отсортированных чисел и много раз ищем в нём последний
 * элемент — это худший случай для линейного поиска.
 *
 *   cc -std=c11 -Wall -Wextra -O2 07-bench.c -o 07-bench
 *   ./07-bench
 *
 * Абсолютные секунды зависят от машины, а вот порядок разницы — нет.
 *
 * Все найденные индексы складываем в sink. Без этого умный компилятор
 * имеет право заметить, что результат поиска никому не нужен, и
 * выбросить весь цикл — тогда мы замерим пустоту.
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
    int last = (n - 1) * 2;                /* последний элемент */
    long long sink = 0;                    /* копилка результатов */

    clock_t t0 = clock();
    for (int q = 0; q < queries; q++) {
        sink += linear_search(a, n, last);
    }
    double lin = (double)(clock() - t0) / CLOCKS_PER_SEC;

    t0 = clock();
    for (int q = 0; q < queries; q++) {
        sink += binary_search(a, n, last);
    }
    double bin = (double)(clock() - t0) / CLOCKS_PER_SEC;

    /* Тот же бинарный поиск, но каждый раз ищем другое число:
       процессор не может «запомнить дорогу», и время честнее. */
    t0 = clock();
    for (int q = 0; q < queries; q++) {
        sink += binary_search(a, n, (q * 7919 % n) * 2);
    }
    double bin_mix = (double)(clock() - t0) / CLOCKS_PER_SEC;

    printf("массив: %d элементов, по %d поисков\n", n, queries);
    printf("линейный, всегда последний: %8.4f c  (%7.2f мкс на поиск)\n",
           lin, lin * 1e6 / queries);
    printf("бинарный, всегда последний: %8.4f c  (%7.2f мкс на поиск)\n",
           bin, bin * 1e6 / queries);
    printf("бинарный, каждый раз новый: %8.4f c  (%7.2f мкс на поиск)\n",
           bin_mix, bin_mix * 1e6 / queries);
    if (bin_mix > 0.0) {
        printf("линейный медленнее бинарного примерно в %.0f раз\n",
               lin / bin_mix);
    }
    printf("(контрольная сумма индексов: %lld)\n", sink);

    free(a);
    return 0;
}
