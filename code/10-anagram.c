/* Глава 10. Анаграммы: состоят ли две фразы из одних и тех же букв.
 *
 *   cc -std=c11 -Wall -Wextra -O2 10-anagram.c -o 10-anagram
 *   ./10-anagram
 *
 * Считаем только буквы, регистр игнорируем: "Dirty Room" и "dormitory"
 * — анаграммы. Только латиница: в UTF-8 русская буква занимает два
 * байта, и по буквам её не посчитать.
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

/* Проходит по строке и меняет счётчики букв на delta:
 * +1 для первой фразы, -1 для второй. Не-буквы пропускает. */
void tally(const char *s, int count[], int delta)
{
    for (int i = 0; s[i] != '\0'; i++) {
        unsigned char c = (unsigned char)s[i];
        if (isalpha(c)) {
            count[tolower(c)] += delta;
        }
    }
}

int main(void)
{
    char a[200];
    char b[200];

    printf("Первая фраза (латиницей): ");
    if (fgets(a, sizeof a, stdin) == NULL) {
        return 0;
    }
    printf("Вторая фраза (латиницей): ");
    if (fgets(b, sizeof b, stdin) == NULL) {
        return 0;
    }

    chomp(a);
    chomp(b);

    /* По счётчику на каждый из 256 возможных байтов. */
    int count[256] = {0};
    tally(a, count, +1);        /* в первой фразе — прибавляем */
    tally(b, count, -1);        /* во второй — вычитаем */

    /* Если все счётчики нулевые — буквы совпали по составу. */
    int anagram = 1;
    for (int k = 0; k < 256; k++) {
        if (count[k] != 0) {
            anagram = 0;
            break;
        }
    }

    if (anagram) {
        printf("\"%s\" и \"%s\" — анаграммы\n", a, b);
    } else {
        printf("\"%s\" и \"%s\" — не анаграммы\n", a, b);
    }
    return 0;
}
