#include <stdio.h>

#define MAXN 100

int main(void)
{
    int n;
    printf("Сколько чисел? ");
    if (scanf("%d", &n) != 1) {
        printf("Это не число.\n");
        return 1;
    }
    if (n <= 0 || n > MAXN) {
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

    int left = 0;          /* левый палец — на первом элементе */
    int right = n - 1;     /* правый палец — на последнем */
    while (left < right) {
        int t = a[left];
        a[left] = a[right];
        a[right] = t;
        left++;
        right--;
    }

    printf("Развёрнутый массив:");
    for (int i = 0; i < n; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");
    return 0;
}
