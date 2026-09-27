/* Глава 8. Считаем сравнения и перестановки у трёх сортировок.
 *
 * Секундомер зависит от машины, а число сравнений — нет. Поэтому
 * здесь каждая сортировка сама считает, сколько раз сравнила два
 * элемента и сколько раз передвинула элемент (обмен у пузырька и
 * выбора, сдвиг у вставок). Прогоняем их на трёх входах по 100 чисел:
 * уже отсортированном, отсортированном задом наперёд и перемешанном.
 *
 *   cc -std=c11 -Wall -Wextra -O2 08-count.c -o 08-count
 *   ./08-count
 */

#include <stdio.h>

#define N 100

long cmps = 0;                  /* сколько раз сравнили два элемента */
long moves = 0;                 /* сколько раз передвинули элемент */

void bubble_sort(int a[], int n)
{
    for (int pass = 0; pass < n - 1; pass++) {
        int swapped = 0;
        for (int i = 0; i < n - 1 - pass; i++) {
            cmps++;
            if (a[i] > a[i + 1]) {
                int t = a[i];
                a[i] = a[i + 1];
                a[i + 1] = t;
                moves++;        /* один обмен соседей */
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
            cmps++;
            if (a[j] < a[min_pos]) {
                min_pos = j;
            }
        }
        if (min_pos != i) {
            int t = a[i];
            a[i] = a[min_pos];
            a[min_pos] = t;
            moves++;            /* один обмен */
        }
    }
}

void insertion_sort(int a[], int n)
{
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0) {
            cmps++;             /* сравниваем a[j] с key */
            if (a[j] <= key) {
                break;          /* нашли место — дальше не идём */
            }
            a[j + 1] = a[j];
            moves++;            /* один сдвиг вправо */
            j--;
        }
        a[j + 1] = key;
    }
}

/* Заполняем массив одним из трёх способов. */
void fill(int a[], int n, int kind)
{
    for (int i = 0; i < n; i++) {
        if (kind == 0) {
            a[i] = i;                   /* 0 1 2 ... 99 */
        } else if (kind == 1) {
            a[i] = n - 1 - i;           /* 99 98 ... 0 */
        } else {
            a[i] = (i * 37) % n;        /* 0 37 74 11 48 ... */
        }
    }
}

/* Печатаем одну строку таблицы: сравнения, перемещения, подпись. */
void report(const char *title, const char *input)
{
    printf("%9ld %11ld   %s, %s\n", cmps, moves, title, input);
}

int main(void)
{
    const char *inputs[] = {"отсортирован", "задом наперёд",
                            "перемешан"};
    int a[N];

    printf("n = %d, n(n-1)/2 = %d\n\n", N, N * (N - 1) / 2);
    printf("сравнений перемещений   сортировка, вход\n");
    for (int kind = 0; kind < 3; kind++) {
        fill(a, N, kind);
        cmps = 0;
        moves = 0;
        bubble_sort(a, N);
        report("пузырёк", inputs[kind]);
    }
    for (int kind = 0; kind < 3; kind++) {
        fill(a, N, kind);
        cmps = 0;
        moves = 0;
        selection_sort(a, N);
        report("выбором", inputs[kind]);
    }
    for (int kind = 0; kind < 3; kind++) {
        fill(a, N, kind);
        cmps = 0;
        moves = 0;
        insertion_sort(a, N);
        report("вставками", inputs[kind]);
    }
    return 0;
}
