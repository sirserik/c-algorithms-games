/* Глава 11. Стек (LIFO) на массиве фиксированного размера.
 *
 *   cc -std=c11 -Wall -Wextra -O2 11-stack.c -o 11-stack
 *   ./11-stack
 *
 * push/pop/peek возвращают 1 при успехе и 0 при отказе (стек полон или
 * пуст). Само значение выдаётся через указатель out — так отказ не
 * путается ни с каким «специальным» числом.
 */

#include <stdio.h>

#define CAPACITY 5      /* сколько тарелок влезает в стопку */

struct Stack {
    int data[CAPACITY];
    int top;            /* индекс первой СВОБОДНОЙ ячейки = размер */
};

/* Кладём на вершину. 1 — положили, 0 — стек полон. */
int push(struct Stack *s, int value)
{
    if (s->top == CAPACITY) {
        return 0;
    }
    s->data[s->top] = value;
    s->top++;
    return 1;
}

/* Снимаем с вершины в *out. 1 — сняли, 0 — стек пуст. */
int pop(struct Stack *s, int *out)
{
    if (s->top == 0) {
        return 0;
    }
    s->top--;
    *out = s->data[s->top];
    return 1;
}

/* Подсматриваем вершину, не снимая. 1 — есть что смотреть, 0 — пусто. */
int peek(const struct Stack *s, int *out)
{
    if (s->top == 0) {
        return 0;
    }
    *out = s->data[s->top - 1];
    return 1;
}

int main(void)
{
    struct Stack s;
    s.top = 0;                  /* пустой стек */

    /* Кладём шесть значений в стопку на пять — последнее не влезет. */
    for (int value = 10; value <= 60; value += 10) {
        if (push(&s, value)) {
            printf("push %d (в стеке %d)\n", value, s.top);
        } else {
            printf("push %d: стек полон, не кладу\n", value);
        }
    }

    int x;
    if (peek(&s, &x)) {
        printf("peek: наверху %d (в стеке всё те же %d)\n", x, s.top);
    }

    /* Снимаем, пока pop возвращает 1. */
    while (pop(&s, &x)) {
        printf("pop %d (осталось %d)\n", x, s.top);
    }

    /* Ещё один pop на пустом стеке: не падаем, а честно отказываем. */
    if (pop(&s, &x) == 0) {
        printf("pop на пустом стеке: вернул 0, x не тронут (там %d)\n", x);
    }

    return 0;
}
