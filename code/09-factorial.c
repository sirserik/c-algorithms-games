#include <stdio.h>

/* Рекурсивный факториал.
   База: 0! = 1 и 1! = 1.
   Шаг: n! = n * (n-1)!. */
long long factorial(int n)
{
    if (n <= 1) {          /* база: дальше не спускаемся */
        return 1;
    }
    return n * factorial(n - 1);   /* шаг: сводим к меньшей задаче */
}

int main(void)
{
    int n;
    printf("Факториал какого числа? ");
    if (scanf("%d", &n) != 1) {
        printf("Это не число.\n");
        return 1;
    }

    if (n < 0) {
        printf("Факториал определён только для n >= 0.\n");
        return 1;
    }

    if (n > 20) {
        printf("Слишком большое: long long переполнится при n > 20.\n");
        return 1;
    }

    printf("%d! = %lld\n", n, factorial(n));
    return 0;
}
