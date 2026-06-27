#include <stdio.h>

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

    printf("Введи слово (латиницей): ");
    if (fgets(line, sizeof line, stdin) == NULL) {
        return 0;
    }

    int len = my_strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
        len--;
    }

    /* Два индекса идут навстречу друг другу. */
    int i = 0;
    int j = len - 1;
    int is_palindrome = 1;              /* пока считаем, что да */
    while (i < j) {
        if (line[i] != line[j]) {
            is_palindrome = 0;         /* нашли несовпадение */
            break;
        }
        i++;
        j--;
    }

    if (is_palindrome) {
        printf("\"%s\" — палиндром\n", line);
    } else {
        printf("\"%s\" — не палиндром\n", line);
    }
    return 0;
}
