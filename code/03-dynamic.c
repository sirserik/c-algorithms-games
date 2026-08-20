#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n;
    printf("Сколько чисел? ");
    /* Верхняя граница нужна не для красоты: ниже считается n * 2,
       а переполнение int — это неопределённое поведение. */
    if (scanf("%d", &n) != 1 || n <= 0 || n > 1000000) {
        printf("Нужно число от 1 до 1000000\n");
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
    printf("квадраты (%d шт.):", n);
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
        a[i] = i * i;
    }
    printf("после realloc (%d шт.):", bigger_n);
    for (int i = 0; i < bigger_n; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");

    free(a);
    return 0;
}
