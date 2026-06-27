#include <stdio.h>

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
    swap(&a, &b);
    printf("после swap: a=%d b=%d\n", a, b);
    return 0;
}
