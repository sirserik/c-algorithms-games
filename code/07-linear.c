#include <stdio.h>

/* Линейный поиск: ищем значение target в первых n элементах массива a.
   Возвращаем индекс первого совпадения или -1, если ничего не нашли. */
int linear_search(const int a[], int n, int target)
{
    for (int i = 0; i < n; i++) {
        if (a[i] == target) {
            return i;       /* нашли — сразу отдаём индекс */
        }
    }
    return -1;              /* дошли до конца и не встретили target */
}

int main(void)
{
    int a[] = {17, 4, 42, 8, 15, 23, 16};
    int n = (int)(sizeof a / sizeof a[0]);

    int target = 15;
    int index = linear_search(a, n, target);

    if (index != -1) {
        printf("Число %d найдено на позиции %d\n", target, index);
    } else {
        printf("Число %d в массиве не найдено\n", target);
    }

    int missing = 99;
    int index2 = linear_search(a, n, missing);
    if (index2 != -1) {
        printf("Число %d найдено на позиции %d\n", missing, index2);
    } else {
        printf("Число %d в массиве не найдено\n", missing);
    }

    return 0;
}
