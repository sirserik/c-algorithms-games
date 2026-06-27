#include <stdio.h>

void print_array(const int a[], int n)
{
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

/* Сортировка вставками по возрастанию.
   Слева растёт отсортированная часть. Берём очередной элемент key
   и вставляем его в нужное место этой части, сдвигая большие
   элементы вправо. Так раскладывают карты в руке. */
void insertion_sort(int a[], int n)
{
    for (int i = 1; i < n; i++) {
        int key = a[i];                /* элемент, который вставляем */
        int j = i - 1;                 /* идём по отсортированной части */

        /* сдвигаем вправо всё, что больше key */
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;                /* нашлось место — кладём key */
    }
}

int main(void)
{
    int a[] = {5, 2, 4, 1};
    int n = (int)(sizeof a / sizeof a[0]);

    printf("До:    ");
    print_array(a, n);

    insertion_sort(a, n);

    printf("После: ");
    print_array(a, n);
    return 0;
}
