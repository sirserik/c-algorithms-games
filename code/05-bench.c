/* Глава 5. Замер: во сколько раз проверка «до корня» быстрее наивной.
 *
 * Берём большое простое число — худший случай для обеих проверок:
 * делителей нет, значит цикл придётся прокрутить целиком.
 *
 *   cc -std=c11 -Wall -Wextra -O2 05-bench.c -o 05-bench
 *   ./05-bench
 *
 * Абсолютное время зависит от машины, а число проверок — нет.
 */

#include <stdio.h>
#include <time.h>

long long steps = 0;   /* сколько раз проверили «делится ли n на i» */

/* Наивно: пробуем все делители от 2 до n-1. */
int is_prime_slow(int n)
{
    if (n < 2) {
        return 0;
    }
    for (int i = 2; i < n; i++) {
        steps++;
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

/* До корня: i <= n / i — та же граница, но без переполнения. */
int is_prime(int n)
{
    if (n < 2) {
        return 0;
    }
    for (int i = 2; i <= n / i; i++) {
        steps++;
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main(void)
{
    int n = 2147483647;   /* самое большое int, и оно простое */

    steps = 0;
    clock_t start = clock();                 /* засекли */
    int slow = is_prime_slow(n);
    clock_t ticks = clock() - start;         /* сколько тиков прошло */
    double ms_slow = 1000.0 * ticks / CLOCKS_PER_SEC;
    long long steps_slow = steps;

    steps = 0;
    start = clock();
    int fast = is_prime(n);
    ticks = clock() - start;
    double ms_fast = 1000.0 * ticks / CLOCKS_PER_SEC;
    long long steps_fast = steps;

    printf("проверяем n = %d\n", n);
    printf("наивно   (i < n):    %10lld проверок, %9.3f мс, ответ %d\n",
           steps_slow, ms_slow, slow);
    printf("до корня (i <= n/i): %10lld проверок, %9.3f мс, ответ %d\n",
           steps_fast, ms_fast, fast);
    printf("проверок меньше в %lld раз(а)\n", steps_slow / steps_fast);
    return 0;
}
