#include <stdio.h>

int main(void)
{
    int n;
    printf("Введи целое число: ");
    if (scanf("%d", &n) != 1) {
        printf("Это не число.\n");
        return 1;
    }

    if (n < 0) {
        n = -n;   /* работаем с модулем, знак отбросим */
    }

    int sum = 0;        /* сюда копим сумму цифр */
    int reversed = 0;   /* сюда собираем перевёрнутое число */
    int original = n;   /* запомним исходное для печати */

    if (n == 0) {
        sum = 0;
        reversed = 0;
    }

    while (n > 0) {
        int digit = n % 10;          /* последняя цифра */
        sum = sum + digit;           /* добавили к сумме */
        reversed = reversed * 10 + digit;  /* приклеили к перевёрнутому */
        n = n / 10;                  /* отбросили последнюю цифру */
    }

    printf("Число:           %d\n", original);
    printf("Сумма цифр:      %d\n", sum);
    printf("Задом наперёд:   %d\n", reversed);
    return 0;
}
