/* Глава 11. Очередь (FIFO) на кольцевом буфере.
 *
 *   cc -std=c11 -Wall -Wextra -O2 11-queue.c -o 11-queue
 *   ./11-queue
 *
 * Храним head (откуда забираем) и count (сколько стоит). Хвост
 * вычисляем: (head + count) % CAPACITY. Так пустая очередь никогда не
 * путается с полной: пуста при count == 0, полна при count == CAPACITY.
 */

#include <stdio.h>

#define CAPACITY 5      /* длина кольцевого буфера */

struct Queue {
    int data[CAPACITY];
    int head;           /* индекс первого в очереди */
    int count;          /* сколько элементов сейчас лежит */
};

/* Индекс первой свободной ячейки — туда встанет следующий. */
int queue_tail(const struct Queue *q)
{
    return (q->head + q->count) % CAPACITY;
}

/* Встаём в хвост. 1 — встали, 0 — очередь полна. */
int enqueue(struct Queue *q, int value)
{
    if (q->count == CAPACITY) {
        return 0;
    }
    q->data[queue_tail(q)] = value;
    q->count++;
    return 1;
}

/* Забираем из головы в *out. 1 — забрали, 0 — очередь пуста. */
int dequeue(struct Queue *q, int *out)
{
    if (q->count == 0) {
        return 0;
    }
    *out = q->data[q->head];
    q->head = (q->head + 1) % CAPACITY;
    q->count--;
    return 1;
}

/* Ставит value в очередь и печатает, что получилось. */
void try_enqueue(struct Queue *q, int value)
{
    int cell = queue_tail(q);           /* куда встанет — до вставки */
    if (enqueue(q, value)) {
        printf("enqueue %d -> ячейка %d (в очереди %d)\n",
               value, cell, q->count);
    } else {
        printf("enqueue %d: очередь полна\n", value);
    }
}

int main(void)
{
    struct Queue q;
    q.head = 0;
    q.count = 0;

    /* Шесть человек на пять мест — последний не влезет. */
    for (int value = 1; value <= 6; value++) {
        try_enqueue(&q, value);
    }

    /* Обслуживаем троих: голова уезжает вправо, слева пусто. */
    int x;
    for (int k = 0; k < 3; k++) {
        if (dequeue(&q, &x)) {
            printf("dequeue %d (осталось %d, head=%d)\n", x, q.count, q.head);
        }
    }

    /* Кольцо в деле: новые встают на освободившиеся ячейки в начале. */
    for (int value = 6; value <= 8; value++) {
        try_enqueue(&q, value);
    }

    while (dequeue(&q, &x)) {
        printf("dequeue %d (осталось %d, head=%d)\n", x, q.count, q.head);
    }

    /* Ещё один dequeue на пустой очереди: отказ, а не падение. */
    if (dequeue(&q, &x) == 0) {
        printf("dequeue на пустой очереди: вернул 0\n");
    }

    return 0;
}
