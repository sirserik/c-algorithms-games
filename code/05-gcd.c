#include <stdio.h>
#include <limits.h>

/* Наибольший общий делитель двумя способами:
   1) наивный перебор делителей;
   2) алгоритм Евклида через остаток.
   Обе функции сначала берут модуль: у -12 и 12 делители одни и те же,
   а вот знак результата иначе получается случайным. */

int gcd_slow(int a, int b)
{
    if (a < 0) {
        a = -a;
    }
    if (b < 0) {
        b = -b;
    }
    if (a == 0) {
        return b;   /* НОД(0, b) = b: на b делится и b, и ноль */
    }
    if (b == 0) {
        return a;
    }

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
    if (a < 0) {
        a = -a;
    }
    if (b < 0) {
        b = -b;
    }

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

    /* У самого маленького int модуля в int не существует: -INT_MIN
       не влезает обратно. Честнее отказаться, чем соврать. */
    if (a == INT_MIN || b == INT_MIN) {
        printf("Число %d слишком маленькое: у него нет модуля в int.\n",
               INT_MIN);
        return 1;
    }

    printf("Перебором: НОД(%d, %d) = %d\n", a, b, gcd_slow(a, b));
    printf("Евклидом:  НОД(%d, %d) = %d\n", a, b, gcd(a, b));
    return 0;
}
