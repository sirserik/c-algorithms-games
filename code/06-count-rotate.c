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

    int target;
    printf("Какое значение посчитать? ");
    if (scanf("%d", &target) != 1) {
        printf("\nЖдал число, а получил что-то другое.\n");
        return 1;
    }

    int count = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] == target) {
            count++;
        }
    }
    printf("Значение %d встречается %d раз(а)\n", target, count);

    int first = a[0];
    for (int i = 0; i < n - 1; i++) {
        a[i] = a[i + 1];
    }
    a[n - 1] = first;

    printf("После сдвига влево:");
    for (int i = 0; i < n; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");
    return 0;
}
