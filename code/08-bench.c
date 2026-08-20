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
    int a = *(const int *)pa;
    int b = *(const int *)pb;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Заполняем массив одними и теми же «случайными» числами:
   srand(1) каждый раз задаёт rand одну и ту же последовательность,
   поэтому все сортировки получают одинаковые данные. */
static void fill_random(int a[], int n)
{
    srand(1);
    for (int i = 0; i < n; i++) {
        a[i] = rand();
    }
}

/* Замеряем одну сортировку на свежезаполненном массиве. */
static double measure(void (*sort)(int[], int), int a[], int n)
{
    fill_random(a, n);
    clock_t t0 = clock();
    sort(a, n);
    return (double)(clock() - t0) / CLOCKS_PER_SEC;
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
        double b = measure(bubble_sort, a, n);
        double e = measure(selection_sort, a, n);
        double i = measure(insertion_sort, a, n);
        printf("%8d %9.3f %9.3f %10.3f\n", n, b, e, i);
    }

    /* Пузырёк с флагом на уже отсортированном массиве: один проход. */
    fill_random(a, 40000);
    insertion_sort(a, 40000);
    clock_t t0 = clock();
    bubble_sort(a, 40000);
    printf("\nпузырёк по уже отсортированным 40000: %.4f c\n",
           (double)(clock() - t0) / CLOCKS_PER_SEC);

    /* А теперь qsort на миллионе — в 25 раз больше, чем 40000. */
    fill_random(a, max_n);
    t0 = clock();
    qsort(a, (size_t)max_n, sizeof a[0], cmp_int);
    printf("qsort по %d элементам: %.4f c\n", max_n,
           (double)(clock() - t0) / CLOCKS_PER_SEC);

    free(a);
    return 0;
}
