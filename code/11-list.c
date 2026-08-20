/* Глава 11. Односвязный список: вставка в начало, удаление головы,
 * удаление последнего узла, освобождение всего списка.
 *
 *   cc -std=c11 -Wall -Wextra -O2 11-list.c -o 11-list
 *   ./11-list
 *
 * Проверка на утечки и на чтение освобождённой памяти:
 *   cc -std=c11 -Wall -Wextra -O1 -g -fsanitize=address,undefined \
 *      11-list.c -o 11-list-san && ./11-list-san
 */

#include <stdio.h>
#include <stdlib.h>

struct Node {
    int value;              /* полезные данные узла */
    struct Node *next;      /* адрес следующего узла (или NULL) */
};

/* Создаёт узел со значением value, который смотрит на next.
 * Возвращает NULL, если памяти не дали. */
struct Node *make_node(int value, struct Node *next)
{
    struct Node *node = malloc(sizeof(struct Node));
    if (node == NULL) {
        return NULL;
    }
    node->value = value;
    node->next = next;
    return node;
}

/* Печатает список: "список: 3 -> 2 -> 1 -> NULL". */
void print_list(const struct Node *head)
{
    printf("список: ");
    for (const struct Node *cur = head; cur != NULL; cur = cur->next) {
        printf("%d -> ", cur->value);
    }
    printf("NULL\n");
}

/* Удаляет первый узел. Возвращает новую голову. */
struct Node *pop_front(struct Node *head)
{
    if (head == NULL) {
        return NULL;                    /* удалять нечего */
    }
    struct Node *rest = head->next;     /* СНАЧАЛА запомнили хвост */
    free(head);                         /* потом освободили голову */
    return rest;
}

/* Удаляет последний узел. Возвращает голову (возможно, новую). */
struct Node *pop_back(struct Node *head)
{
    if (head == NULL) {
        return NULL;                    /* удалять нечего */
    }
    if (head->next == NULL) {           /* в списке всего один узел */
        free(head);
        return NULL;                    /* список опустел */
    }
    struct Node *cur = head;
    while (cur->next->next != NULL) {   /* ищем ПРЕДпоследний узел */
        cur = cur->next;
    }
    free(cur->next);                    /* убрали последний */
    cur->next = NULL;                   /* предпоследний стал последним */
    return head;
}

/* Освобождает весь список: на каждый malloc свой free. */
void free_list(struct Node *head)
{
    struct Node *cur = head;
    while (cur != NULL) {
        struct Node *next = cur->next;  /* СНАЧАЛА запомнили следующий */
        free(cur);                      /* потом удалили текущий */
        cur = next;                     /* перешли к запомненному */
    }
}

int main(void)
{
    struct Node *head = NULL;           /* пустой список */

    /* Добавляем 1..5 в начало списка. */
    for (int value = 1; value <= 5; value++) {
        struct Node *node = make_node(value, head);
        if (node == NULL) {
            printf("Не хватило памяти\n");
            free_list(head);            /* уже выделенное — вернуть */
            return 1;
        }
        head = node;                    /* новый узел стал головой */
        printf("добавили %d\n", value);
    }
    print_list(head);

    head = pop_front(head);
    printf("удалили голову:\n");
    print_list(head);

    printf("удаляем с конца, пока не опустеет:\n");
    while (head != NULL) {
        head = pop_back(head);
        print_list(head);
    }

    head = pop_back(head);              /* удаление из пустого списка */
    if (head == NULL) {
        printf("pop_back на пустом списке: голова так и осталась NULL\n");
    }

    /* Соберём список заново и освободим его одним махом. */
    for (int value = 10; value <= 30; value += 10) {
        struct Node *node = make_node(value, head);
        if (node == NULL) {
            printf("Не хватило памяти\n");
            free_list(head);
            return 1;
        }
        head = node;
    }
    print_list(head);

    free_list(head);
    head = NULL;                        /* адреса больше не действуют */
    printf("free_list: весь список освобождён\n");
    print_list(head);

    return 0;
}
