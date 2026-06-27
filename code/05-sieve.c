#include <stdio.h>

/* Решето Эратосфена: печатает все простые числа до N. */

#define N 100

int main(void)
{
    int is_composite[N + 1] = {0};   /* 0 = считаем простым, 1 = вычеркнуто */

    for (int i = 2; i * i <= N; i++) {
        if (is_composite[i] == 0) {
            for (int j = i * i; j <= N; j += i) {
                is_composite[j] = 1;
            }
        }
    }

    printf("Простые числа до %d:\n", N);
    int count = 0;
    for (int i = 2; i <= N; i++) {
        if (is_composite[i] == 0) {
            printf("%d ", i);
            count++;
        }
    }
    printf("\nВсего: %d\n", count);
    return 0;
}
