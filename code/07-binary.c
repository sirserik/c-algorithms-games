#include <stdio.h>

/* Бинарный поиск. ВАЖНО: массив a ДОЛЖЕН быть отсортирован по
   возрастанию. Ищем target, возвращаем его индекс или -1.
   Если target встречается несколько раз — вернётся индекс одного
   из совпадений, не обязательно первого. */
int binary_search(const int a[], int n, int target)
{
    int low = 0;            /* левая граница диапазона поиска */
    int high = n - 1;       /* правая граница диапазона поиска */

    while (low <= high) {
        int mid = low + (high - low) / 2;   /* середина без переполнения */

        if (a[mid] == target) {
            return mid;                     /* попали точно в середину */
        } else if (a[mid] < target) {
            low = mid + 1;                  /* target правее середины */
        } else {
            high = mid - 1;                 /* target левее середины */
        }
    }
    return -1;                              /* диапазон схлопнулся — нет */
}

int main(void)
{
    /* Массив уже отсортирован по возрастанию. */
    int a[] = {1, 3, 5, 7, 9, 11};
    int n = (int)(sizeof a / sizeof a[0]);

    int target = 7;
    int index = binary_search(a, n, target);

    if (index != -1) {
        printf("Число %d найдено на позиции %d\n", target, index);
    } else {
        printf("Число %d в массиве не найдено\n", target);
    }

    int missing = 8;
    int index2 = binary_search(a, n, missing);
    if (index2 != -1) {
        printf("Число %d найдено на позиции %d\n", missing, index2);
    } else {
        printf("Число %d в массиве не найдено\n", missing);
    }

    return 0;
}
