#include <stdio.h>

/* Наивная рекурсия: красиво, но медленно.
   База: fib(0) = 0, fib(1) = 1.
   Шаг: fib(n) = fib(n-1) + fib(n-2). */
long long fib_slow(int n)
{
    if (n < 2) {                 /* база: fib(0)=0, fib(1)=1 */
        return n;
    }
    return fib_slow(n - 1) + fib_slow(n - 2);
}

/* Быстрый вариант: один проход циклом, две переменные. */
long long fib_fast(int n)
{
    long long a = 0;             /* fib(0) */
    long long b = 1;             /* fib(1) */
    for (int i = 0; i < n; i++) {
        long long next = a + b;  /* следующее число */
        a = b;                   /* сдвигаем окно вправо */
        b = next;
    }
    return a;
}

int main(void)
{
    int n;
    printf("Какое число Фибоначчи посчитать? ");
    if (scanf("%d", &n) != 1) {
        printf("Это не число.\n");
        return 1;
    }
    if (n < 0) {
        printf("Нужно n >= 0.\n");
        return 1;
    }
    if (n > 90) {
        printf("Слишком большое: long long переполнится при n > 90.\n");
        return 1;
    }

    /* Наивную рекурсию зовём только на малых n: на n = 50 она уже
       считает полминуты, а на n = 60 — часами. Не даём программе
       зависнуть у читателя. */
    if (n <= 35) {
        printf("Рекурсия:  fib(%d) = %lld\n", n, fib_slow(n));
    } else {
        printf("Рекурсия:  пропущена, при n > 35 она слишком "
               "медленная\n");
    }
    printf("Цикл:      fib(%d) = %lld\n", n, fib_fast(n));
    return 0;
}
