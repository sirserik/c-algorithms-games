#include <stdio.h>
#include <stdlib.h>

#define CAPACITY 5      // длина кольцевого буфера

int main(void)
{
    int data[CAPACITY];     // сама очередь (кольцо)
    int head = 0;           // откуда забираем (начало очереди)
    int count = 0;          // сколько человек сейчас стоит

    // enqueue: встаём в конец очереди
    for (int value = 1; value <= 6; value++) {
        if (count == CAPACITY) {                    // мест нет
            printf("enqueue %d: очередь полна\n", value);
            continue;
        }
        int tail = (head + count) % CAPACITY;       // первое свободное место
        data[tail] = value;                         // встали в хвост
        count = count + 1;                          // людей стало больше
        printf("enqueue %d (в очереди %d)\n", value, count);
    }

    // dequeue: обслуживаем тех, кто пришёл раньше
    while (count > 0) {
        int value = data[head];                     // берём первого
        head = (head + 1) % CAPACITY;               // голова сдвинулась
        count = count - 1;                          // людей стало меньше
        printf("dequeue %d (осталось %d)\n", value, count);
    }

    if (count == 0) {
        printf("dequeue: очередь пуста\n");
    }

    return 0;
}
