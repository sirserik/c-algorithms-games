/* Глава 10. Палиндром: читается ли строка одинаково с двух концов.
 *
 *   cc -std=c11 -Wall -Wextra -O2 10-palindrome.c -o 10-palindrome
 *   ./10-palindrome
 *
 * Считаем только буквы: регистр и знаки препинания игнорируем, поэтому
 * "A man, a plan, a canal: Panama" — тоже палиндром.
 * Только латиница: в UTF-8 русская буква занимает два байта.
 */

#include <stdio.h>
#include <ctype.h>

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
    char line[200];

    printf("Введи строку (латиницей): ");
    if (fgets(line, sizeof line, stdin) == NULL) {
        return 0;
    }
    chomp(line);

    /* Шаг 1: оставляем только буквы, приводя их к нижнему регистру.
     * clean не меньше line, значит переполнить его нечем. */
    char clean[200];
    int m = 0;
    for (int k = 0; line[k] != '\0'; k++) {
        unsigned char c = (unsigned char)line[k];
        if (isalpha(c)) {
            clean[m] = (char)tolower(c);
            m++;
        }
    }
    clean[m] = '\0';

    /* Шаг 2: два индекса идут навстречу друг другу. */
    int i = 0;
    int j = m - 1;
    int is_palindrome = 1;              /* пока считаем, что да */
    while (i < j) {
        if (clean[i] != clean[j]) {
            is_palindrome = 0;          /* нашли несовпадение */
            break;
        }
        i++;
        j--;
    }

    printf("Только буквы: \"%s\" (%d шт.)\n", clean, m);
    if (is_palindrome) {
        printf("\"%s\" — палиндром\n", line);
    } else {
        printf("\"%s\" — не палиндром\n", line);
    }
    return 0;
}
