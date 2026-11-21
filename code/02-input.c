#include <stdio.h>

/* выбрасывает остаток строки; возвращает 0, если ввод кончился */
int skip_line(void)
{
    int c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
    if (c == EOF) {
        return 0;
    }
    return 1;
}

int main(void)
{
    int n;

    printf("Введи целое число: ");
    while (scanf("%d", &n) != 1) {
        if (skip_line() == 0) {
            printf("Ввод кончился, выхожу.\n");
            return 1;
        }
        printf("Это не число. Попробуй ещё раз: ");
    }

    printf("Принято: %d\n", n);
    return 0;
}
