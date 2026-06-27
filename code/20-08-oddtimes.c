#include <stdio.h>

/* Число, встречающееся нечётное число раз.
   XOR всех элементов: парные гасят друг друга, остаётся одиночка. */
int find_odd(const int *a, int n)
{
    int x = 0;
    for (int i = 0; i < n; i++) {
        x ^= a[i];      /* a ^ a == 0, x ^ 0 == x */
    }
    return x;
}

int main(void)
{
    /* 4 встречается 3 раза (нечётно), остальные — парами. */
    int a[] = {2, 3, 5, 4, 5, 3, 2, 4, 4};
    int n = (int)(sizeof a / sizeof a[0]);

    printf("Нечётное число раз встречается: %d\n", find_odd(a, n));
    return 0;
}
