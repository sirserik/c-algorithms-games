#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n;
    printf("Сколько чисел? ");
    if (scanf("%d", &n) != 1) {
        printf("Это не число\n");
        return 1;
    }
    /* потолок 1000 — чтобы вывод помещался на экран */
    if (n < 1 || n > 1000) {
        printf("Нужно число от 1 до 1000\n");
        return 1;
    }

    int *a = malloc(n * sizeof(int));
    if (a == NULL) {
        printf("Не хватило памяти\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        a[i] = i * 10;
    }
    printf("числа (%d шт.):", n);
    for (int i = 0; i < n; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");

    /* Мало места? Просим ещё — realloc сохраняет то, что уже лежит. */
    int bigger_n = n * 2;
    int *bigger = realloc(a, bigger_n * sizeof(int));
    if (bigger == NULL) {
        printf("Не хватило памяти на расширение\n");
        free(a);
        return 1;
    }
    a = bigger;

    for (int i = n; i < bigger_n; i++) {
        a[i] = i * 10;
    }
    printf("после realloc (%d шт.):", bigger_n);
    for (int i = 0; i < bigger_n; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");

    free(a);
    return 0;
}
