#include <stdio.h>

void print_array(const int a[], int n)
{
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

/* Сортировка выбором по возрастанию.
   Делим массив на две части: слева — уже отсортированная,
   справа — ещё нет. На каждом шаге находим минимум в правой части
   и ставим его в начало этой части. */
void selection_sort(int a[], int n)
{
    for (int i = 0; i < n - 1; i++) {
        int min_pos = i;               /* пока считаем минимумом a[i] */

        /* ищем настоящий минимум среди a[i+1 .. n-1] */
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[min_pos]) {
                min_pos = j;           /* запомнили позицию меньшего */
            }
        }

        if (min_pos != i) {            /* минимум не на своём месте */
            int t = a[i];              /* меняем его с началом части */
            a[i] = a[min_pos];
            a[min_pos] = t;
        }
    }
}

int main(void)
{
    int a[] = {5, 2, 4, 1};
    int n = (int)(sizeof a / sizeof a[0]);

    printf("До:    ");
    print_array(a, n);

    selection_sort(a, n);

    printf("После: ");
    print_array(a, n);
    return 0;
}
