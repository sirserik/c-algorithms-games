#include <stdio.h>

#define CAP 4   /* размер кольца */

/* Кольцевой буфер на массиве фиксированного размера.
   Храним голову и количество; хвост вычисляем. */
typedef struct {
    int data[CAP];
    int head;     /* индекс первого элемента */
    int count;    /* сколько элементов лежит */
} Ring;

/* Кладём в хвост. 0 — буфер полон. */
int ring_push(Ring *r, int value)
{
    if (r->count == CAP) {
        return 0;
    }
    int tail = (r->head + r->count) % CAP;   /* первое свободное место */
    r->data[tail] = value;
    r->count++;
    return 1;
}

/* Забираем из головы в *out. 0 — буфер пуст. */
int ring_pop(Ring *r, int *out)
{
    if (r->count == 0) {
        return 0;
    }
    *out = r->data[r->head];
    r->head = (r->head + 1) % CAP;           /* голова заворачивается */
    r->count--;
    return 1;
}

int main(void)
{
    Ring r = {{0}, 0, 0};

    for (int v = 1; v <= 6; v++) {           /* кладём 6 в кольцо на 4 */
        if (ring_push(&r, v)) {
            printf("push %d: ok\n", v);
        } else {
            printf("push %d: полон\n", v);
        }
    }

    int x;
    ring_pop(&r, &x);
    printf("pop %d\n", x);
    ring_pop(&r, &x);
    printf("pop %d\n", x);

    ring_push(&r, 7);    /* место освободилось — кольцо завернулось в начало */
    ring_push(&r, 8);

    while (ring_pop(&r, &x)) {
        printf("pop %d\n", x);
    }
    return 0;
}
