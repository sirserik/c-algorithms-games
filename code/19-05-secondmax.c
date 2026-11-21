#include <stdio.h>

int main(void)
{
    int a[] = {3, 9, 9, 1, 7, 9, 4};
    int n = (int)(sizeof(a) / sizeof(a[0]));

    int max = a[0];        /* самое большое: пока это первый элемент */
    int second = 0;        /* второе по величине (пока не найдено) */
    int have_second = 0;   /* флажок «второй найден»: 0 — нет, 1 — да */

    for (int i = 1; i < n; i++) {
        if (a[i] > max) {
            second = max;  /* бывший максимум становится вторым */
            have_second = 1;
            max = a[i];
        } else if (a[i] < max) {
            if (!have_second || a[i] > second) {
                second = a[i];   /* меньше max и лучше прежнего второго */
                have_second = 1;
            }
        }
    }

    if (have_second) {
        printf("Максимум: %d, второй: %d\n", max, second);
    } else {
        printf("Второго по величине нет.\n");
    }
    return 0;
}
