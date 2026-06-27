#include <stdio.h>

/* Своя длина строки: считаем символы до '\0'. */
int my_strlen(const char *s)
{
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

int main(void)
{
    char line[100];

    printf("Введи строку (латиницей): ");
    if (fgets(line, sizeof line, stdin) == NULL) {
        return 0;                       /* ввода не было */
    }

    /* Убираем хвостовой '\n', который оставил fgets. */
    int len = my_strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
        len--;
    }

    /* Переворот на месте: меняем местами края, идём к центру. */
    int i = 0;
    int j = len - 1;
    while (i < j) {
        char tmp = line[i];
        line[i] = line[j];
        line[j] = tmp;
        i++;
        j--;
    }

    printf("Длина: %d\n", len);
    printf("Перевёрнутая: %s\n", line);
    return 0;
}
