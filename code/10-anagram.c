#include <stdio.h>

int my_strlen(const char *s)
{
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

/* Убирает хвостовой '\n' от fgets. */
void chomp(char *s)
{
    int len = my_strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }
}

int main(void)
{
    char a[200];
    char b[200];

    printf("Первое слово (латиницей): ");
    if (fgets(a, sizeof a, stdin) == NULL) {
        return 0;
    }
    printf("Второе слово (латиницей): ");
    if (fgets(b, sizeof b, stdin) == NULL) {
        return 0;
    }

    chomp(a);
    chomp(b);

    /* По счётчику на каждый из 256 возможных байтов. */
    int count[256] = {0};

    for (int i = 0; a[i] != '\0'; i++) {
        count[(unsigned char)a[i]]++;  /* в первом слове — прибавляем */
    }
    for (int i = 0; b[i] != '\0'; i++) {
        count[(unsigned char)b[i]]--;  /* во втором — вычитаем */
    }

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
