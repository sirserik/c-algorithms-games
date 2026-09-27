#include <stdio.h>
#include <stdlib.h>

struct Node {
    int value;
    struct Node *next;
};

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

/* Середина списка: медленный шагает на 1, быстрый на 2. */
struct Node *middle(struct Node *head)
{
    struct Node *slow = head;
    struct Node *fast = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;          /* +1 */
        fast = fast->next->next;    /* +2 */
    }
    return slow;            /* быстрый в конце — медленный в центре */
}

/* Обнаружение цикла: черепаха и заяц (Флойд). 1 если цикл есть. */
int has_cycle(struct Node *head)
{
    struct Node *slow = head;
    struct Node *fast = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {         /* заяц догнал черепаху */
            return 1;
        }
    }
    return 0;                       /* быстрый упёрся в NULL — цикла нет */
}

/* Начало цикла (вторая фаза Флойда) или NULL, если цикла нет. */
struct Node *cycle_start(struct Node *head)
{
    struct Node *slow = head;
    struct Node *fast = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {         /* фаза 1: встретились внутри кольца */
            struct Node *p = head;  /* фаза 2: второй бегунок — в голову */
            while (p != slow) {     /* оба шагают по одному узлу */
                p = p->next;
                slow = slow->next;
            }
            return p;               /* встретились ровно в начале кольца */
        }
    }
    return NULL;
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
    for (int i = 7; i >= 1; i--) {  /* список 1 -> 2 -> ... -> 7 -> NULL */
        head = push_front(head, i);
    }

    struct Node *mid = middle(head);
    printf("Середина списка из 7: %d\n", mid->value);
    printf("Цикл есть? %d\n", has_cycle(head));

    /* Закольцуем: последний узел смотрит на третий. */
    struct Node *last = head;
    struct Node *third = head->next->next;
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = third;             /* теперь в списке цикл */
    printf("После закольцовки цикл есть? %d\n", has_cycle(head));
    printf("Цикл начинается в узле %d\n", cycle_start(head)->value);

    last->next = NULL;              /* разорвали цикл — теперь можно free */
    free_list(head);
    return 0;
}
