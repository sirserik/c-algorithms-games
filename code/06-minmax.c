#include <stdio.h>

#define MAXN 100

int main(void)
{
    int n;
    printf("Сколько чисел? ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAXN) {
        printf("Нужно от 1 до %d чисел.\n", MAXN);
        return 1;
    }

    int a[MAXN];
    for (int i = 0; i < n; i++) {
        printf("a[%d] = ", i);
        if (scanf("%d", &a[i]) != 1) {
            printf("\nЖдал число, а получил что-то другое.\n");
            return 1;
        }
    }

    int min = a[0];
    int max = a[0];
    long long sum = 0;   /* сто чисел по два миллиарда int не выдержит */
    for (int i = 0; i < n; i++) {
        if (a[i] < min) {
            min = a[i];
        }
        if (a[i] > max) {
            max = a[i];
        }
        sum += a[i];
    }

    double avg = (double)sum / n;

    printf("Минимум: %d\n", min);
    printf("Максимум: %d\n", max);
    printf("Сумма:    %lld\n", sum);
    printf("Среднее:  %.2f\n", avg);
    return 0;
}
