/* Глава 4. Где именно ломается факториал.
 *
 * Считаем n! в int и в long long и печатаем таблицу. Перед каждым
 * умножением проверяем, влезет ли результат, — так мы узнаём точку
 * поломки, ни разу не совершив само переполнение (переполнение
 * знакового типа — неопределённое поведение, его надо избегать, а не
 * «ловить постфактум»).
 *
 *   cc -std=c11 -Wall -Wextra -O2 04-overflow.c -o 04-overflow
 *   ./04-overflow
 */

#include <stdio.h>
#include <limits.h>

int main(void)
{
    int fact_int = 1;        /* факториал в int */
    long long fact_ll = 1;   /* факториал в long long */
    int int_ok = 1;          /* int ещё считает честно? */
    int ll_ok = 1;           /* long long ещё считает честно? */

    printf(" n |          n! в int |         n! в long long\n");
    printf("---+-------------------+-----------------------\n");

    for (int i = 1; i <= 22; i++) {
        /* fact * i влезет в int, только если fact <= INT_MAX / i */
        if (int_ok && fact_int > INT_MAX / i) {
            int_ok = 0;
        }
        if (int_ok) {
            fact_int = fact_int * i;
        }

        if (ll_ok && fact_ll > LLONG_MAX / i) {
            ll_ok = 0;
        }
        if (ll_ok) {
            fact_ll = fact_ll * i;
        }

        printf("%2d |", i);
        if (int_ok) {
            printf(" %17d |", fact_int);
        } else {
            printf("      переполнение |");   /* 1 + 5 пробелов + 12 букв */
        }
        if (ll_ok) {
            printf(" %22lld\n", fact_ll);
        } else {
            printf("           переполнение\n");   /* 1 + 10 пробелов + 12 */
        }
    }

    printf("\nINT_MAX   = %d\n", INT_MAX);
    printf("LLONG_MAX = %lld\n", LLONG_MAX);
    return 0;
}
