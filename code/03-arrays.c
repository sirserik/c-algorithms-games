#include <stdio.h>

/* Длину массива передаём отдельным аргументом: внутри функции
   имя массива превращается в указатель, и sizeof тут не поможет. */
int sum(const int a[], int n)
{
    int s = 0;
    for (int i = 0; i < n; i++) {
        s = s + a[i];
    }
    return s;
}

int main(void)
{
    int marks[5] = {4, 5, 3, 5, 2};
    int n = (int)(sizeof marks / sizeof marks[0]);

    printf("marks[0] (первая)    = %d\n", marks[0]);
    printf("marks[2] (третья)    = %d\n", marks[2]);
    printf("marks[%d] (последняя) = %d\n", n - 1, marks[n - 1]);

    printf("весь массив:");
    for (int i = 0; i < n; i++) {
        printf(" %d", marks[i]);
    }
    printf("\n");

    printf("sizeof marks    = %zu байт\n", sizeof marks);
    printf("sizeof marks[0] = %zu байт\n", sizeof marks[0]);
    printf("элементов       = %d\n", n);

    int s = sum(marks, n);
    printf("сумма   = %d\n", s);
    printf("среднее = %.2f\n", (double)s / n);
    return 0;
}
