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

    int i = 0;
    int j = n - 1;
    while (i < j) {
        int t = a[i];
        a[i] = a[j];
        a[j] = t;
        i++;
        j--;
    }

    printf("Развёрнутый массив:");
    for (int k = 0; k < n; k++) {
        printf(" %d", a[k]);
    }
    printf("\n");
    return 0;
}
