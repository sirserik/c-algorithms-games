#include <stdio.h>
#include <limits.h>

/* Строка -> int. Обрабатываем пробелы, знак и переполнение.
   Возвращает 1 при успехе (результат в *out), 0 при ошибке
   (нет цифр или число не влезает в int). */
int my_atoi(const char *s, int *out)
{
    while (*s == ' ' || *s == '\t') {   /* пропускаем ведущие пробелы */
        s++;
    }

    int sign = 1;
    if (*s == '+' || *s == '-') {
        if (*s == '-') {
            sign = -1;
        }
        s++;
    }

    if (*s < '0' || *s > '9') {          /* ни одной цифры — это не число */
        return 0;
    }

    long long result = 0;                /* копим в long long: он шире int */
    while (*s >= '0' && *s <= '9') {
        result = result * 10 + (*s - '0');
        if (sign == 1 && result > INT_MAX) {       /* вышли за +предел */
            return 0;
        }
        if (sign == -1 && -result < INT_MIN) {     /* вышли за -предел */
            return 0;
        }
        s++;
    }

    *out = (int)(sign * result);
    return 1;
}

/* int -> строка. buf должен быть не меньше 12 байт:
   знак + 10 цифр + завершающий '\0'. */
void my_itoa(int n, char *buf)
{
    char tmp[12];
    int i = 0;
    int neg = 0;

    /* Берём значение в long long: -INT_MIN в int не влезает. */
    long long value = n;
    if (value < 0) {
        neg = 1;
        value = -value;
    }

    do {                                  /* цифры выходят в обратном порядке */
        tmp[i++] = (char)('0' + value % 10);
        value /= 10;
    } while (value > 0);

    int j = 0;
    if (neg) {
        buf[j++] = '-';
    }
    while (i > 0) {                        /* разворачиваем в buf */
        buf[j++] = tmp[--i];
    }
    buf[j] = '\0';
}

int main(void)
{
    const char *tests[] = {
        "42", "  -17", "+5", "2147483647",
        "2147483648", "-2147483648", "abc", "-2147483649"
    };
    int n = (int)(sizeof tests / sizeof tests[0]);
    for (int i = 0; i < n; i++) {
        int v;
        if (my_atoi(tests[i], &v)) {
            printf("my_atoi(\"%s\") = %d\n", tests[i], v);
        } else {
            printf("my_atoi(\"%s\") = ошибка\n", tests[i]);
        }
    }

    printf("---\n");

    char buf[12];
    int vals[] = {0, 42, -17, INT_MAX, INT_MIN};
    int m = (int)(sizeof vals / sizeof vals[0]);
    for (int i = 0; i < m; i++) {
        my_itoa(vals[i], buf);
        printf("my_itoa(%d) = \"%s\"\n", vals[i], buf);
    }
    return 0;
}
