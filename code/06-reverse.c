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
