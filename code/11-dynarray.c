/* Глава 11. Растущий массив: кончилось место — удваиваем вместимость.
 *
 *   cc -std=c11 -Wall -Wextra -O2 11-dynarray.c -o 11-dynarray
 *   ./11-dynarray
 *
 * Проверка памяти:
 *   cc -std=c11 -Wall -Wextra -O1 -g -fsanitize=address,undefined \
 *      11-dynarray.c -o 11-dynarray-san && ./11-dynarray-san
 */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int capacity = 2;                       /* сколько влезает сейчас */
    int count = 0;                          /* сколько реально лежит */
    int *data = malloc((size_t)capacity * sizeof(int));
    if (data == NULL) {
        printf("Не хватило памяти\n");
        return 1;
    }

    for (int value = 1; value <= 10; value++) {
        if (count == capacity) {            /* заполнили — растём */
            capacity = capacity * 2;
            int *bigger = realloc(data,
                                  (size_t)capacity * sizeof(int));
            if (bigger == NULL) {           /* не дали — старое цело */
                printf("Не хватило памяти при росте\n");
                free(data);
                return 1;
            }
            data = bigger;                  /* теперь работаем с новым */
            printf("[рост: вместимость стала %d]\n", capacity);
        }
        data[count] = value;                /* кладём в свободную ячейку */
        count = count + 1;                  /* занятых стало больше */
    }

    printf("Всего элементов: %d, вместимость: %d\n", count, capacity);
    for (int i = 0; i < count; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    free(data);
    return 0;
}
