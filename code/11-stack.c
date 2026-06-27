#include <stdio.h>
#include <stdlib.h>

#define CAPACITY 5      // сколько тарелок влезает в стопку

int main(void)
{
    int data[CAPACITY];     // сам стек
    int top = 0;            // индекс первой СВОБОДНОЙ ячейки = размер

    // push: кладём тарелку наверх
    for (int value = 10; value <= 60; value += 10) {
        if (top == CAPACITY) {                  // стопка полна
            printf("push %d: переполнение, не кладу\n", value);
            continue;
        }
        data[top] = value;                      // положили на вершину
        top = top + 1;                          // вершина поднялась
        printf("push %d (в стеке %d)\n", value, top);
    }

    // peek: смотрим на верхнюю тарелку, не снимая
    if (top > 0) {
        printf("peek: наверху %d\n", data[top - 1]);
    }

    // pop: снимаем тарелки сверху, пока не опустеет
    while (top > 0) {
        top = top - 1;                          // вершина опустилась
        int value = data[top];                  // взяли верхнюю
        printf("pop %d (осталось %d)\n", value, top);
    }

    // пытаемся снять из пустого
    if (top == 0) {
        printf("pop: стек пуст, снимать нечего\n");
    }

    return 0;
}
