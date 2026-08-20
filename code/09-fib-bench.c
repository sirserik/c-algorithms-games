/* Глава 9. Замер: насколько плоха наивная рекурсия для Фибоначчи.
 *
 * Считаем fib(n) наивной рекурсией при растущем n и засекаем время.
 * Отдельно считаем, СКОЛЬКО вызовов функции при этом происходит.
 *
 *   cc -std=c11 -Wall -Wextra -O2 09-fib-bench.c -o 09-fib-bench
 *   ./09-fib-bench
 *
 * Программа работает несколько секунд — последняя строка самая тяжёлая.
 */

#include <stdio.h>
#include <time.h>

/* Наивная рекурсия — та самая, что в 09-fib.c. */
long long fib_slow(int n)
{
    if (n < 2) {
        return n;
    }
    return fib_slow(n - 1) + fib_slow(n - 2);
}

/* Та же рекурсия, но со счётчиком вызовов. Счётчик глобальный:
   он объявлен вне всех функций, поэтому виден им всем и живёт
   всё время работы программы. */
long long calls = 0;

long long fib_counted(int n)
{
    calls++;                       /* нас вызвали — отмечаем */
    if (n < 2) {
        return n;
    }
    return fib_counted(n - 1) + fib_counted(n - 2);
}

/* Быстрый вариант из главы — для сравнения. */
long long fib_fast(int n)
{
    long long a = 0;
    long long b = 1;
    for (int i = 0; i < n; i++) {
        long long next = a + b;
        a = b;
        b = next;
    }
    return a;
}

int main(void)
{
    int ns[] = {25, 30, 35, 40, 45};
    int count = (int)(sizeof ns / sizeof ns[0]);

    printf("  n        fib(n)      секунды        вызовов\n");
    for (int i = 0; i < count; i++) {
        int n = ns[i];

        clock_t t0 = clock();
        long long value = fib_slow(n);
        double sec = (double)(clock() - t0) / CLOCKS_PER_SEC;

        calls = 0;
        fib_counted(n);

        printf("%3d %13lld %12.4f %14lld\n", n, value, sec, calls);
    }

    /* Для контраста: цикл на том же n=45 и сразу на n=90. */
    clock_t t0 = clock();
    long long v45 = fib_fast(45);
    long long v90 = fib_fast(90);
    double sec = (double)(clock() - t0) / CLOCKS_PER_SEC;
    printf("\nцикл: fib(45) = %lld и fib(90) = %lld за %.6f c\n",
           v45, v90, sec);

    return 0;
}
