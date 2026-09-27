#include <stdio.h>

/* Наивный popcount: проверяем младший бит и сдвигаем вправо. */
int popcount_naive(unsigned int n)
{
    int count = 0;
    while (n != 0) {
        count += n & 1u;    /* младший бит: 0 или 1 */
        n >>= 1;            /* сдвинули число на один бит вправо */
    }
    return count;
}

/* Приём Брайана Кернигана: n &= n - 1 гасит младший единичный бит. */
int popcount_kernighan(unsigned int n)
{
    int count = 0;
    while (n != 0) {
        n &= n - 1;         /* убрали одну единицу */
        count++;            /* итераций ровно столько, сколько единиц */
    }
    return count;
}

/* Степень двойки: ровно один единичный бит, и число положительное. */
int is_power_of_two(unsigned int n)
{
    return n != 0 && (n & (n - 1)) == 0;
}

int main(void)
{
    unsigned int values[] = {0u, 1u, 2u, 7u, 8u, 255u, 1024u};
    int len = (int)(sizeof values / sizeof values[0]);

    for (int i = 0; i < len; i++) {
        unsigned int v = values[i];
        printf("%5u: бит=%d (Керниган=%d), степень_двойки=%d\n",
               v, popcount_naive(v), popcount_kernighan(v),
               is_power_of_two(v));
    }
    return 0;
}
