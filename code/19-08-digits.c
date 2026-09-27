#include <stdio.h>

int main(void)
{
    int n;
    printf("Введи целое число: ");
    if (scanf("%d", &n) != 1) {
        printf("Это не число.\n");
        return 1;
    }

    long long m = n;   /* в long long модуль INT_MIN помещается */
    if (m < 0) {
        m = -m;        /* знак на количество и сумму цифр не влияет */
    }

    int count = 0;     /* сколько цифр */
    int sum = 0;       /* сумма цифр */

    if (m == 0) {
        count = 1;     /* у нуля одна цифра */
    }
    while (m > 0) {
        int digit = (int)(m % 10);
        sum += digit;
        count++;
        m /= 10;
    }

    printf("Цифр: %d, сумма цифр: %d\n", count, sum);
    return 0;
}
