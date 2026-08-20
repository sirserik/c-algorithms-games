#include <stdio.h>

/* Принимает КОПИИ — снаружи ничего не изменится. */
void bad_swap(int a, int b)
{
    int t = a;
    a = b;
    b = t;
    printf("  внутри bad_swap: a=%d b=%d\n", a, b);
}

/* Принимает АДРЕСА — меняет сами переменные. */
void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int main(void)
{
    int x = 42;
    int *p = &x;

    printf("значение x:        %d\n", x);
    printf("адрес x:           %p\n", (void *)&x);
    printf("p хранит адрес:    %p\n", (void *)p);
    printf("значение по p:     %d\n", *p);

    *p = 100;
    printf("теперь x = %d\n", x);

    int a = 1, b = 2;
    bad_swap(a, b);
    printf("после bad_swap: a=%d b=%d\n", a, b);
    swap(&a, &b);
    printf("после swap: a=%d b=%d\n", a, b);

    int *q = NULL;
    printf("q = %p\n", (void *)q);
    if (q == NULL) {
        printf("q никуда не указывает, *q писать нельзя\n");
    }
    return 0;
}
