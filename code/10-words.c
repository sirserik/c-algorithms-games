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
    char line[200];

    printf("Введи фразу (латиницей): ");
    if (fgets(line, sizeof line, stdin) == NULL) {
        return 0;
    }

    int len = my_strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
        len--;
    }

    int words = 0;
    int in_word = 0;                   /* сейчас мы внутри слова? */
    for (int i = 0; i < len; i++) {
        char c = line[i];
        if (c == ' ' || c == '\t') {
            in_word = 0;               /* пробел — слово кончилось */
        } else {
            if (in_word == 0) {        /* переход «пробел -> буква» */
                words++;               /* начало нового слова */
                in_word = 1;
            }
        }
    }

    printf("Слов: %d\n", words);
    return 0;
}
