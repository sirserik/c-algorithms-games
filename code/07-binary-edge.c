/* Глава 7. Бинарный поиск на краевых случаях.
 *
 * Проверяем функцию там, где новички обычно ломаются: пустой массив,
 * массив из одного элемента, самый первый и самый последний элемент,
 * число вне диапазона и массив с повторами.
 *
 *   cc -std=c11 -Wall -Wextra -O2 07-binary-edge.c -o 07-binary-edge
 *   ./07-binary-edge
 */

#include <stdio.h>

/* Тот же бинарный поиск, что и в 07-binary.c.
   ВАЖНО: если target встречается несколько раз, функция вернёт
   индекс ОДНОГО из совпадений — не обязательно первого. */
int binary_search(const int a[], int n, int target)
{
    int low = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
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
    int empty[1] = {0};        /* массив есть, но искать будем в нуле */
    int one[] = {5};
    int a[] = {1, 3, 5, 7, 9, 11};
    int dup[] = {2, 4, 4, 4, 4, 7, 9};

    printf("пустой массив, ищем 5:        %d\n",
           binary_search(empty, 0, 5));
    printf("один элемент {5}, ищем 5:     %d\n",
           binary_search(one, 1, 5));
    printf("один элемент {5}, ищем 4:     %d\n",
           binary_search(one, 1, 4));
    printf("1 3 5 7 9 11, ищем 1:         %d\n",
           binary_search(a, 6, 1));
    printf("1 3 5 7 9 11, ищем 11:        %d\n",
           binary_search(a, 6, 11));
    printf("1 3 5 7 9 11, ищем 0:         %d\n",
           binary_search(a, 6, 0));
    printf("1 3 5 7 9 11, ищем 12:        %d\n",
           binary_search(a, 6, 12));
    printf("2 4 4 4 4 7 9, ищем 4:        %d\n",
           binary_search(dup, 7, 4));

    /* А теперь запретный приём: неотсортированный массив.
       Каждое число в нём ЕСТЬ, но бинарный поиск это не спасёт. */
    int mess[] = {17, 4, 42, 8, 15, 23, 16};
    int m = (int)(sizeof mess / sizeof mess[0]);

    printf("\nнеотсортированный 17 4 42 8 15 23 16:\n");
    for (int i = 0; i < m; i++) {
        printf("  ищем %2d (лежит на %d) -> %d\n",
               mess[i], i, binary_search(mess, m, mess[i]));
    }

    return 0;
}
