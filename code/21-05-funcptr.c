#include <stdio.h>
#include <stdlib.h>

/* Три простые операции — все одной формы int(int,int). */
int op_add(int a, int b) { return a + b; }
int op_sub(int a, int b) { return a - b; }
int op_mul(int a, int b) { return a * b; }

/* Таблица обработчиков: символ операции -> указатель на функцию. */
typedef struct {
    char symbol;
    int (*fn)(int, int);   /* указатель на функцию (int, int) -> int */
} Handler;

/* Компаратор для qsort: сортировка по УБЫВАНИЮ, без вычитания
   (вычитание a-b может переполнить int, см. главу 8). */
int cmp_desc(const void *pa, const void *pb)
{
    int a = *(const int *)pa;
    int b = *(const int *)pb;
    return (a < b) - (a > b);   /* +1 если a<b (a идёт позже), -1 если a>b */
}

int main(void)
{
    Handler table[] = {
        {'+', op_add},
        {'-', op_sub},
        {'*', op_mul},
    };
    int n = (int)(sizeof table / sizeof table[0]);

    int x = 6, y = 4;
    char ops[] = {'+', '-', '*'};
    for (int k = 0; k < 3; k++) {
        char op = ops[k];
        for (int i = 0; i < n; i++) {
            if (table[i].symbol == op) {           /* нашли обработчик */
                int r = table[i].fn(x, y);         /* вызов через указатель */
                printf("%d %c %d = %d\n", x, op, y, r);
                break;
            }
        }
    }

    printf("---\n");

    int a[] = {3, 1, 4, 1, 5, 9, 2, 6};
    int m = (int)(sizeof a / sizeof a[0]);
    qsort(a, (size_t)m, sizeof a[0], cmp_desc);
    for (int i = 0; i < m; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}
