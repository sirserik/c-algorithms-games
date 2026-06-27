#include <stdio.h>
#include <stdlib.h>

struct Node {
    int value;
    struct Node *next;
};

/* Добавляем узел в начало списка (для сборки тестового списка). */
struct Node *push_front(struct Node *head, int value)
{
    struct Node *node = malloc(sizeof(struct Node));
    if (node == NULL) {
        printf("Не хватило памяти\n");
        exit(1);
    }
    node->value = value;
    node->next = head;
    return node;
}

/* Разворот списка через три указателя: prev, cur, next. */
struct Node *reverse(struct Node *head)
{
    struct Node *prev = NULL;       /* позади ничего нет */
    struct Node *cur = head;        /* бегунок по списку */
    while (cur != NULL) {
        struct Node *next = cur->next;  /* запомнили хвост */
        cur->next = prev;               /* развернули стрелку назад */
        prev = cur;                     /* prev сдвинулся вперёд */
        cur = next;                     /* cur сдвинулся вперёд */
    }
    return prev;                    /* prev — новая голова */
}

void print_list(struct Node *head)
{
    for (struct Node *cur = head; cur != NULL; cur = cur->next) {
        printf("%d -> ", cur->value);
    }
    printf("NULL\n");
}

void free_list(struct Node *head)
{
    while (head != NULL) {
        struct Node *next = head->next;
        free(head);
        head = next;
    }
}

int main(void)
{
    struct Node *head = NULL;
    for (int i = 1; i <= 5; i++) {  /* получится 5 -> 4 -> 3 -> 2 -> 1 */
        head = push_front(head, i);
    }

    printf("До:    ");
    print_list(head);

    head = reverse(head);

    printf("После: ");
    print_list(head);

    free_list(head);
    return 0;
}
