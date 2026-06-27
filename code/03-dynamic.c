#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n;
    printf("Сколько чисел? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Нужно положительное число\n");
        return 1;
    }

    int *a = malloc(n * sizeof(int));
    if (a == NULL) {
        printf("Не хватило памяти\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        a[i] = i * i;
    }
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    free(a);
    return 0;
}
