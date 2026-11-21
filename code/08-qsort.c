#include <stdio.h>
#include <stdlib.h>     /* здесь живёт qsort */

void print_array(const int a[], int n)
{
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

/* Компаратор: говорит qsort, какой из двух элементов «раньше».
   Получает адреса двух элементов как const void *.
   Возвращает < 0, если первый меньше; 0, если равны; > 0, если больше. */
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

int main(void)
{
    int a[] = {5, 2, 4, 1, 9, 3, 7};
    int n = (int)(sizeof a / sizeof a[0]);

    /* qsort(база, число_элементов, размер_элемента, компаратор) */
    qsort(a, (size_t)n, sizeof a[0], cmp_int);

    print_array(a, n);
    return 0;
}
