/* Глава 11. Почему растущий массив удваивают, а не расширяют на один.
 *
 *   cc -std=c11 -Wall -Wextra -O2 11-growth.c -o 11-growth
 *   ./11-growth
 *
 * Копируем при переезде руками (malloc + цикл + free), чтобы видеть
 * настоящую цену роста: сколько элементов пришлось перетащить.
 * Абсолютные секунды зависят от машины, а разница в числе копирований
 * — нет, она чисто математическая.
 *
 * Счётчики — long long: при +1 копирований почти пять миллиардов, это
 * больше 2^31. Тип long на Windows 32-битный и переполнился бы.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Стратегия «расширяем ровно на один элемент».
 * Возвращает, сколько элементов всего было скопировано. */
long long grow_by_one(int n)
{
    int *data = NULL;
    int capacity = 0;
    long long copied = 0;

    for (int value = 0; value < n; value++) {
        int *bigger = malloc((size_t)(capacity + 1) * sizeof(int));
        if (bigger == NULL) {
            free(data);
            return -1;
        }
        for (int i = 0; i < capacity; i++) {    /* переезд старых данных */
            bigger[i] = data[i];
            copied++;
        }
        free(data);
        data = bigger;
        capacity = capacity + 1;
        data[value] = value;
    }

    free(data);
    return copied;
}

/* Стратегия «кончилось место — удваиваем». */
long long grow_doubling(int n)
{
    int *data = NULL;
    int capacity = 0;
    int count = 0;
    long long copied = 0;

    for (int value = 0; value < n; value++) {
        if (count == capacity) {
            int new_capacity = capacity * 2;
            if (new_capacity == 0) {
                new_capacity = 1;               /* самый первый рост */
            }
            int *bigger = malloc((size_t)new_capacity * sizeof(int));
            if (bigger == NULL) {
                free(data);
                return -1;
            }
            for (int i = 0; i < count; i++) {   /* переезд старых данных */
                bigger[i] = data[i];
                copied++;
            }
            free(data);
            data = bigger;
            capacity = new_capacity;
        }
        data[count] = value;
        count = count + 1;
    }

    free(data);
    return copied;
}

int main(void)
{
    int n = 100000;

    clock_t t0 = clock();
    long long one = grow_by_one(n);
    double one_sec = (double)(clock() - t0) / CLOCKS_PER_SEC;

    t0 = clock();
    long long twice = grow_doubling(n);
    double twice_sec = (double)(clock() - t0) / CLOCKS_PER_SEC;

    if (one < 0 || twice < 0) {
        printf("Не хватило памяти\n");
        return 1;
    }

    printf("вставляем %d элементов\n", n);
    printf("рост на +1:   скопировано %lld элементов, %.3f c\n",
           one, one_sec);
    printf("удвоение:     скопировано %lld элементов, %.3f c\n",
           twice, twice_sec);
    printf("копирований меньше в %.0f раз\n", (double)one / (double)twice);
    return 0;
}
