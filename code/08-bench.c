/* Глава 8. Замер: насколько O(n^2) хуже O(n log n).
 *
 * Сортируем один и тот же случайный массив тремя самописными
 * сортировками при растущем n, а потом даём qsort миллион элементов.
 *
 *   cc -std=c11 -Wall -Wextra -O2 08-bench.c -o 08-bench
 *   ./08-bench
 *
 * Секунды будут свои на каждой машине. Смотри не на них, а на то,
 * как меняется время при удвоении n.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bubble_sort(int a[], int n)
{
    for (int pass = 0; pass < n - 1; pass++) {
        int swapped = 0;
        for (int i = 0; i < n - 1 - pass; i++) {
            if (a[i] > a[i + 1]) {
                int t = a[i];
                a[i] = a[i + 1];
                a[i + 1] = t;
                swapped = 1;
            }
        }
        if (swapped == 0) {
            break;
        }
    }
}

void selection_sort(int a[], int n)
{
    for (int i = 0; i < n - 1; i++) {
        int min_pos = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[min_pos]) {
                min_pos = j;
            }
        }
        if (min_pos != i) {
            int t = a[i];
            a[i] = a[min_pos];
            a[min_pos] = t;
        }
    }
}

void insertion_sort(int a[], int n)
{
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

int cmp_int(const void *pa, const void *pb)
{
    const int *left = pa;    /* по этому адресу лежит int */
    const int *right = pb;
    int x = *left;           /* достаём сами числа */
    int y = *right;
    if (x < y) {
        return -1;
    }
    if (x > y) {
        return 1;
    }
    return 0;
}

/* Заполняем массив одними и теми же «случайными» числами.
   rand() выдаёт очередное случайное число, а srand(1) заставляет её
   каждый раз начинать одну и ту же последовательность. Поэтому все
   три сортировки получают одинаковые данные. Подробно — в главе 12. */
void fill_random(int a[], int n)
{
    srand(1);
    for (int i = 0; i < n; i++) {
        a[i] = rand();
    }
}

/* Сколько секунд прошло с момента start. */
double seconds_since(clock_t start)
{
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

int main(void)
{
    int max_n = 1000000;
    int *a = malloc((size_t)max_n * sizeof(int));
    if (a == NULL) {
        printf("Не хватило памяти\n");
        return 1;
    }

    int sizes[] = {5000, 10000, 20000, 40000};
    int count = (int)(sizeof sizes / sizeof sizes[0]);

    printf("       n   пузырёк   выбором  вставками\n");
    for (int s = 0; s < count; s++) {
        int n = sizes[s];

        fill_random(a, n);
        clock_t t0 = clock();
        bubble_sort(a, n);
        double bubble_sec = seconds_since(t0);

        fill_random(a, n);
        t0 = clock();
        selection_sort(a, n);
        double selection_sec = seconds_since(t0);

        fill_random(a, n);
        t0 = clock();
        insertion_sort(a, n);
        double insertion_sec = seconds_since(t0);

        printf("%8d %9.3f %9.3f %10.3f\n",
               n, bubble_sec, selection_sec, insertion_sec);
    }

    /* Пузырёк с флагом на уже отсортированном массиве: один проход. */
    fill_random(a, 40000);
    insertion_sort(a, 40000);
    clock_t t0 = clock();
    bubble_sort(a, 40000);
    printf("\nпузырёк по уже отсортированным 40000: %.6f c\n",
           seconds_since(t0));

    /* А теперь qsort на миллионе — в 25 раз больше, чем 40000. */
    fill_random(a, max_n);
    t0 = clock();
    qsort(a, (size_t)max_n, sizeof a[0], cmp_int);
    printf("qsort по %d элементам: %.4f c\n", max_n, seconds_since(t0));

    free(a);
    return 0;
}
