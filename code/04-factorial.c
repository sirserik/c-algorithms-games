#include <stdio.h>

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
        printf("Слишком большое: даже long long переполнится при n > 20.\n");
        return 1;
    }

    long long fact = 1;   /* 1! = 1, и 0! = 1 — стартуем с единицы */
    for (int i = 2; i <= n; i++) {
        fact = fact * i;
    }

    printf("%d! = %lld\n", n, fact);
    return 0;
}
