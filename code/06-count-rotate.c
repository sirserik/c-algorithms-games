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

    int target;
    printf("Какое значение посчитать? ");
    scanf("%d", &target);

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
