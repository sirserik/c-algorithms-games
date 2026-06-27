#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Обобщённый стек: хранит элементы ЛЮБОГО типа.
   Секрет — байтовый буфер и запомненный размер одного элемента.
   Сам стек не знает, int там, double или struct: он копирует байты. */
typedef struct {
    char *data;          /* буфер из байтов */
    size_t elem_size;    /* размер одного элемента в байтах */
    size_t count;        /* сколько элементов лежит */
    size_t capacity;     /* сколько элементов влезает */
} Stack;

/* Готовим стек под элементы размера elem_size. 1 — успех, 0 — нет памяти. */
int stack_init(Stack *s, size_t elem_size)
{
    s->elem_size = elem_size;
    s->count = 0;
    s->capacity = 4;
    s->data = malloc(s->capacity * elem_size);
    return s->data != NULL;
}

/* Кладём элемент: копируем elem_size байт по адресу elem на вершину. */
int stack_push(Stack *s, const void *elem)
{
    if (s->count == s->capacity) {                 /* места нет — растём */
        size_t new_cap = s->capacity * 2;
        char *bigger = realloc(s->data, new_cap * s->elem_size);
        if (bigger == NULL) {
            return 0;
        }
        s->data = bigger;
        s->capacity = new_cap;
    }
    memcpy(s->data + s->count * s->elem_size, elem, s->elem_size);
    s->count++;
    return 1;
}

/* Снимаем верхний элемент в out (если out != NULL). 0 — стек пуст. */
int stack_pop(Stack *s, void *out)
{
    if (s->count == 0) {
        return 0;
    }
    s->count--;
    if (out != NULL) {
        memcpy(out, s->data + s->count * s->elem_size, s->elem_size);
    }
    return 1;
}

void stack_free(Stack *s)
{
    free(s->data);
    s->data = NULL;
    s->count = 0;
    s->capacity = 0;
}

int main(void)
{
    /* стек целых */
    Stack si;
    if (!stack_init(&si, sizeof(int))) {
        printf("Не хватило памяти\n");
        return 1;
    }
    for (int i = 10; i <= 50; i += 10) {
        stack_push(&si, &i);
    }
    int value;
    printf("int:    ");
    while (stack_pop(&si, &value)) {
        printf("%d ", value);
    }
    printf("\n");
    stack_free(&si);

    /* тот же стек, но для double — код стека не изменился ни на строку */
    Stack sd;
    if (!stack_init(&sd, sizeof(double))) {
        return 1;
    }
    double d = 3.14;
    stack_push(&sd, &d);
    d = 2.71;
    stack_push(&sd, &d);
    printf("double: ");
    while (stack_pop(&sd, &d)) {
        printf("%.2f ", d);
    }
    printf("\n");
    stack_free(&sd);
    return 0;
}
