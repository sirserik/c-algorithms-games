#include <stdio.h>

/* Наибольший общий делитель двумя способами:
   1) наивный перебор делителей;
   2) алгоритм Евклида через остаток. */

int gcd_slow(int a, int b)
{
    int result = 1;
    for (int d = 1; d <= a && d <= b; d++) {
        if (a % d == 0 && b % d == 0) {
            result = d;
        }
    }
    return result;
}

int gcd(int a, int b)
{
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int main(void)
{
    int a, b;
    printf("Введи два числа через пробел: ");
    if (scanf("%d %d", &a, &b) != 2) {
        printf("Нужно два целых числа.\n");
        return 1;
    }

    printf("Перебором: НОД(%d, %d) = %d\n", a, b, gcd_slow(a, b));
    printf("Евклидом:  НОД(%d, %d) = %d\n", a, b, gcd(a, b));
    return 0;
}
