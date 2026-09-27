#include <stdio.h>

int main(void)
{
    int n;

    printf("Введи целое число: ");
    while (scanf("%d", &n) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        if (c == EOF) {
            printf("Ввод кончился, выхожу.\n");
            return 1;
        }
        printf("Это не число. Попробуй ещё раз: ");
    }

    long long twice = n;    /* копия в широкую коробку */
    printf("Принято: %d\n", n);
    printf("Удвоенное: %lld\n", twice * 2);
    return 0;
}
