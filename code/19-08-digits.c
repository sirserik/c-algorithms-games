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
        n = -n;   /* знак на количество и сумму цифр не влияет */
    }

    int count = 0;   /* сколько цифр */
    int sum = 0;     /* сумма цифр */

    if (n == 0) {
        count = 1;   /* у нуля одна цифра */
    }
    while (n > 0) {
        int digit = n % 10;
        sum += digit;
        count++;
        n /= 10;
    }

    printf("Цифр: %d, сумма цифр: %d\n", count, sum);
    return 0;
}
