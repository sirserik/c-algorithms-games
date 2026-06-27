#include <stdio.h>
#include <stdlib.h>

struct Node {
    int value;              // полезные данные узла
    struct Node *next;      // адрес следующего узла (или NULL)
};

int main(void)
{
    struct Node *head = NULL;   // пустой список: головы ещё нет

    // добавляем 1, 2, 3 в начало списка
    for (int value = 1; value <= 3; value++) {
        struct Node *node = malloc(sizeof(struct Node));
        if (node == NULL) {
            printf("Не хватило памяти\n");
            return 1;
        }
        node->value = value;    // заполнили данные
        node->next = head;      // новый смотрит на старую голову
        head = node;            // новый узел становится головой
        printf("добавили %d\n", value);
    }

    // обход: идём от головы по стрелкам next, пока не упрёмся в NULL
    printf("список: ");
    for (struct Node *cur = head; cur != NULL; cur = cur->next) {
        printf("%d -> ", cur->value);
    }
    printf("NULL\n");

    // освобождение: идём по списку и удаляем каждый узел
    struct Node *cur = head;
    while (cur != NULL) {
        struct Node *next = cur->next;  // СНАЧАЛА запомнили следующий
        free(cur);                      // потом удалили текущий
        cur = next;                     // перешли к запомненному
    }
    head = NULL;                        // список пуст

    return 0;
}
