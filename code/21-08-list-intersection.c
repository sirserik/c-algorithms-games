#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

/* Создать узел со значением value и ссылкой next. */
Node *make(int value, Node *next)
{
    Node *n = malloc(sizeof(Node));
    if (n == NULL) {
        fprintf(stderr, "Не хватило памяти\n");
        exit(1);
    }
    n->value = value;
    n->next = next;
    return n;
}

int length(Node *head)
{
    int len = 0;
    for (Node *p = head; p != NULL; p = p->next) {
        len++;
    }
    return len;
}

/* Узел пересечения двух списков или NULL.
   Идея: выравниваем старты. Длинный список проматываем вперёд на
   разницу длин — теперь до конца обоим идти поровну. Дальше шагаем
   параллельно: где адреса совпадут, там и общий узел. */
Node *intersection(Node *a, Node *b)
{
    int la = length(a);
    int lb = length(b);

    while (la > lb) { a = a->next; la--; }
    while (lb > la) { b = b->next; lb--; }

    while (a != b) {        /* сравниваем АДРЕСА узлов, не значения */
        a = a->next;
        b = b->next;
    }
    return a;               /* общий узел или NULL */
}

int main(void)
{
    /* общий хвост: 7 -> 8 -> 9 -> NULL */
    Node *tail = make(7, make(8, make(9, NULL)));

    /* A: 1 -> 2 -> 3 -> [хвост] */
    Node *a = make(1, make(2, make(3, tail)));
    /* B: 4 -> 5 -> [хвост] */
    Node *b = make(4, make(5, tail));

    Node *meet = intersection(a, b);
    if (meet != NULL) {
        printf("Пересечение в узле со значением %d\n", meet->value);
    } else {
        printf("Списки не пересекаются\n");
    }

    /* Освобождаем аккуратно: общий хвост — РОВНО один раз.
       Сначала уникальные части A и B (до tail), затем сам хвост. */
    Node *p = a;
    while (p != tail) { Node *nx = p->next; free(p); p = nx; }
    p = b;
    while (p != tail) { Node *nx = p->next; free(p); p = nx; }
    p = tail;
    while (p != NULL) { Node *nx = p->next; free(p); p = nx; }

    return 0;
}
