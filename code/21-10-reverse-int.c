#include <stdio.h>
#include <limits.h>

/* Развернуть цифры знакового int с защитой от переполнения.
   1 и результат в *out — если влезает; 0 — если разворот не
   помещается в int (например, разворот 1000000003 = 3000000001). */
int reverse_int(int n, int *out)
{
    int result = 0;
    while (n != 0) {
        int digit = n % 10;   /* знак сохраняется: -123 % 10 == -3 (C99) */
        n /= 10;

        /* Проверяем ДО умножения: влезет ли result*10 + digit.
           Для положительной части сравниваем с INT_MAX, для
           отрицательной — с INT_MIN. */
        if (result > INT_MAX / 10 ||
            (result == INT_MAX / 10 && digit > INT_MAX % 10)) {
            return 0;
        }
        if (result < INT_MIN / 10 ||
            (result == INT_MIN / 10 && digit < INT_MIN % 10)) {
            return 0;
        }
        result = result * 10 + digit;
    }
    *out = result;
    return 1;
}

int main(void)
{
    int tests[] = {123, -456, 120, 0, 1534236469, 1000000003, INT_MIN};
    int n = (int)(sizeof tests / sizeof tests[0]);
    for (int i = 0; i < n; i++) {
        int r;
        if (reverse_int(tests[i], &r)) {
            printf("reverse(%d) = %d\n", tests[i], r);
        } else {
            printf("reverse(%d) = переполнение\n", tests[i]);
        }
    }
    return 0;
}
