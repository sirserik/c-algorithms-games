/* Глава 8. Стабильность сортировки.
 *
 * Сортировка стабильная, если элементы с ОДИНАКОВЫМ ключом остаются
 * в том же порядке, в каком лежали до сортировки.
 *
 * Чтобы увидеть это на обычных int, зашьём в каждое число сразу две
 * вещи: десятки — ключ, по которому сортируем, единицы — метка
 * исходного места. Число 42 читается как «ключ 4, метка 2».
 *
 *   cc -std=c11 -Wall -Wextra -O2 08-stable.c -o 08-stable
 *   ./08-stable
 */

#include <stdio.h>

static int key_of(int x)
{
    return x / 10;              /* десятки — то, по чему сортируем */
}

static int tag_of(int x)
{
    return x % 10;              /* единицы — откуда элемент пришёл */
}

void print_tagged(const char *title, const int a[], int n)
{
    printf("%s", title);
    for (int i = 0; i < n; i++) {
        printf("%d/%d  ", key_of(a[i]), tag_of(a[i]));
    }
    printf("\n");
}

/* Все три сортировки сравнивают ТОЛЬКО ключи. */
void bubble_sort(int a[], int n)
{
    for (int pass = 0; pass < n - 1; pass++) {
        int swapped = 0;
        for (int i = 0; i < n - 1 - pass; i++) {
            if (key_of(a[i]) > key_of(a[i + 1])) {
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
            if (key_of(a[j]) < key_of(a[min_pos])) {
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
        while (j >= 0 && key_of(a[j]) > key_of(key)) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

int main(void)
{
    /* Три элемента с ключом 4 (метки 1, 2, 3) и один с ключом 2. */
    int start[] = {41, 42, 43, 24};
    int n = (int)(sizeof start / sizeof start[0]);
    int a[4];

    print_tagged("до:        ", start, n);

    for (int i = 0; i < n; i++) a[i] = start[i];
    bubble_sort(a, n);
    print_tagged("пузырёк:   ", a, n);

    for (int i = 0; i < n; i++) a[i] = start[i];
    insertion_sort(a, n);
    print_tagged("вставки:   ", a, n);

    for (int i = 0; i < n; i++) a[i] = start[i];
    selection_sort(a, n);
    print_tagged("выбор:     ", a, n);

    return 0;
}
