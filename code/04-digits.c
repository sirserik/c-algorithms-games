#include <stdio.h>
#include <limits.h>

int main(void)
{
    int n;
    printf("Введи целое число: ");
    if (scanf("%d", &n) != 1) {
        printf("Это не число.\n");
        return 1;
    }

    /* Работаем не с самим n, а с копией в long long. Две причины:
       1) -n для INT_MIN не влезает в int, а в long long влезает;
       2) перевёрнутое число бывает больше исходного и int переполняет. */
    long long m = n;
    if (m < 0) {
        m = -m;   /* берём модуль: знак цифрам не нужен */
    }

    int sum = 0;             /* сюда копим сумму цифр */
    long long reversed = 0;  /* сюда собираем перевёрнутое число */

    while (m > 0) {
        int digit = (int)(m % 10);         /* последняя цифра */
        sum = sum + digit;                 /* добавили к сумме */
        reversed = reversed * 10 + digit;  /* приклеили справа */
        m = m / 10;                        /* отбросили цифру */
    }

    printf("Число:           %d\n", n);
    printf("Сумма цифр:      %d\n", sum);
    printf("Задом наперёд:   %lld", reversed);
    if (reversed > INT_MAX) {
        printf("  (в int уже не влезает!)");
    }
    printf("\n");
    return 0;
}
