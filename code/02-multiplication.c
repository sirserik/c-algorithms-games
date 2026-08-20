#include <stdio.h>

int is_even(int x)
{
    return x % 2 == 0;
}

int main(void)
{
    int n;

    printf("Таблица умножения для какого числа? ");
    if (scanf("%d", &n) != 1) {
        printf("Это не число.\n");
        return 1;
    }

    for (int i = 1; i <= 10; i++) {
        int result = n * i;
        if (is_even(result)) {
            printf("%2d x %2d = %3d  (чётное)\n", n, i, result);
        } else {
            printf("%2d x %2d = %3d\n", n, i, result);
        }
    }

    return 0;
}
