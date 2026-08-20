/* Глава 10. Подсчёт слов во фразе.
 *
 *   cc -std=c11 -Wall -Wextra -O2 10-words.c -o 10-words
 *   ./10-words
 *
 * Разделителем считаем любой пробельный символ: пробел, табуляцию,
 * возврат каретки. Лишние пробелы по краям и между словами не мешают.
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

    printf("Введи фразу (латиницей): ");
    if (fgets(line, sizeof line, stdin) == NULL) {
        return 0;
    }
    chomp(line);

    int words = 0;
    int longest = 0;                    /* длина самого длинного слова */
    int current = 0;                    /* длина слова, которое идём */
    int in_word = 0;                    /* сейчас мы внутри слова? */

    for (int i = 0; line[i] != '\0'; i++) {
        unsigned char c = (unsigned char)line[i];
        if (isspace(c)) {
            in_word = 0;                /* пробел — слово кончилось */
            current = 0;
        } else {
            if (in_word == 0) {         /* переход «пробел -> буква» */
                words++;                /* начало нового слова */
                in_word = 1;
            }
            current++;
            if (current > longest) {
                longest = current;
            }
        }
    }

    printf("Слов: %d\n", words);
    printf("Самое длинное слово: %d символов\n", longest);
    return 0;
}
