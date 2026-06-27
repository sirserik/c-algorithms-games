#include <stdio.h>

int main(void)
{
    int n;
    printf("Сколько чисел? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Нужно положительное количество.\n");
        return 1;
    }

    int a[100];
    if (n > 100) {
        n = 100;
    }
    for (int i = 0; i < n; i++) {
        printf("a[%d] = ", i);
        scanf("%d", &a[i]);
    }

    int min = a[0];
    int max = a[0];
    int sum = 0;
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
    printf("Сумма:    %d\n", sum);
    printf("Среднее:  %.2f\n", avg);
    return 0;
}
