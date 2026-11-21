#include <stdio.h>

/* Наивная рекурсия: красиво, но медленно.
   База: fib(0) = 0, fib(1) = 1.
   Шаг: fib(n) = fib(n-1) + fib(n-2). */
long long fib_slow(int n)
{
    if (n == 0) {
        return 0;                /* база: fib(0) = 0 */
    }
    if (n == 1) {
        return 1;                /* база: fib(1) = 1 */
    }
    return fib_slow(n - 1) + fib_slow(n - 2);
}

/* Быстрый вариант: один проход циклом, две переменные. */
long long fib_fast(int n)
{
    if (n == 0) {
        return 0;
    }
    long long prev = 0;              /* fib(0) */
    long long curr = 1;              /* fib(1) */
    for (int i = 2; i <= n; i++) {
        long long next = prev + curr;    /* следующее число */
        prev = curr;                     /* сдвигаем окно вправо */
        curr = next;
    }
    return curr;                     /* fib(n) */
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
    if (n > 92) {
        printf("Слишком большое: long long переполнится при n > 92.\n");
        return 1;
    }

    /* Наивную рекурсию зовём только на малых n: на n = 50 она
       считает больше полуминуты, а на n = 60 — больше часа.
       Не даём программе зависнуть у читателя. */
    if (n <= 35) {
        printf("Рекурсия:  fib(%d) = %lld\n", n, fib_slow(n));
    } else {
        printf("Рекурсия:  пропущена, при n > 35 она слишком "
               "медленная\n");
    }
    printf("Цикл:      fib(%d) = %lld\n", n, fib_fast(n));
    return 0;
}
