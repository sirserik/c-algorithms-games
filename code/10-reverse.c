/* Глава 10. Своя длина строки и переворот строки на месте.
 *
 *   cc -std=c11 -Wall -Wextra -O2 10-reverse.c -o 10-reverse
 *   ./10-reverse
 *
 * Только латиница: в UTF-8 русская буква занимает два байта, и
 * посимвольный переворот её разрежет пополам.
 */

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

/* Срезает хвостовой '\n', который оставляет после себя fgets. */
void chomp(char *s)
{
    int len = my_strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }
}

int main(void)
{
    char line[100];

    printf("Введи строку (латиницей): ");
    if (fgets(line, sizeof line, stdin) == NULL) {
        return 0;                       /* ввода не было */
    }
    chomp(line);

    int len = my_strlen(line);

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
