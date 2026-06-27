#include <stdio.h>

/* Печать массива в одну строку — пригодится во всех трёх программах. */
void print_array(const int a[], int n)
{
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

/* Сортировка пузырьком по возрастанию.
   За каждый проход самый большой из оставшихся «всплывает» вправо.
   Оптимизация: если за проход не было ни одного обмена — массив
   уже отсортирован, выходим досрочно. */
void bubble_sort(int a[], int n)
{
    for (int pass = 0; pass < n - 1; pass++) {
        int swapped = 0;               /* были ли обмены в этом проходе */

        /* На каждом следующем проходе хвост уже отсортирован,
           поэтому не доходим до конца: -1 за i+1, -pass за хвост. */
        for (int i = 0; i < n - 1 - pass; i++) {
            if (a[i] > a[i + 1]) {     /* соседи стоят не по порядку */
                int t = a[i];          /* классический swap через temp */
                a[i] = a[i + 1];
                a[i + 1] = t;
                swapped = 1;
            }
        }

        if (swapped == 0) {            /* за проход ничего не меняли */
            break;                     /* значит, всё уже на местах */
        }
    }
}

int main(void)
{
    int a[] = {5, 2, 4, 1};
    int n = (int)(sizeof a / sizeof a[0]);

    printf("До:    ");
    print_array(a, n);

    bubble_sort(a, n);

    printf("После: ");
    print_array(a, n);
    return 0;
}
