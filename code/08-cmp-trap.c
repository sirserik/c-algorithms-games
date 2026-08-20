/* Глава 8. Ловушка компаратора: почему нельзя писать «return a - b».
 *
 * Коротко: разность двух int сама по себе может не влезть в int.
 * Тогда знак результата получается неверным, и qsort расставляет
 * элементы не по порядку. Никакой ошибки при этом не выводится.
 *
 *   cc -std=c11 -Wall -Wextra -O2 08-cmp-trap.c -o 08-cmp-trap
 *   ./08-cmp-trap
 *
 * А так видно и саму причину:
 *   cc -std=c11 -Wall -Wextra -g -fsanitize=undefined 08-cmp-trap.c \
 *      -o 08-cmp-trap-san
 *   ./08-cmp-trap-san
 */

#include <stdio.h>
#include <stdlib.h>

/* ПЛОХОЙ компаратор: короткий, красивый и неверный. */
int cmp_bad(const void *pa, const void *pb)
{
    int a = *(const int *)pa;
    int b = *(const int *)pb;
    return a - b;               /* разность может переполнить int */
}

/* ХОРОШИЙ компаратор: три сравнения, зато всегда правильный. */
int cmp_good(const void *pa, const void *pb)
{
    int a = *(const int *)pa;
    int b = *(const int *)pb;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

void print_array(const int a[], int n)
{
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int main(void)
{
    int bad[] = {2000000000, -2000000000, 7, -7};
    int n = (int)(sizeof bad / sizeof bad[0]);
    int good[4];

    for (int i = 0; i < n; i++) {
        good[i] = bad[i];       /* второй экземпляр тех же чисел */
    }

    printf("до сортировки:    ");
    print_array(bad, n);

    qsort(bad, (size_t)n, sizeof bad[0], cmp_bad);
    qsort(good, (size_t)n, sizeof good[0], cmp_good);

    printf("через a - b:      ");
    print_array(bad, n);
    printf("через сравнения:  ");
    print_array(good, n);

    return 0;
}
