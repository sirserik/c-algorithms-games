#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WRONG 6   /* сколько ошибок позволяем до проигрыша */

/* Рисует слово: открытые буквы — как есть, закрытые — подчёркиванием.
   Возвращает, сколько букв уже открыто. */
int draw_word(const char *word, const int *guessed)
{
    int revealed = 0;
    for (int i = 0; word[i] != '\0'; i++) {
        char ch = word[i];
        if (guessed[ch - 'a']) {
            printf("%c ", ch);
            revealed++;
        } else {
            printf("_ ");
        }
    }
    printf("\n");
    return revealed;
}

int main(void)
{
    const char *words[] = {
        "apple", "banana", "orange", "computer", "program",
        "keyboard", "function", "pointer", "rainbow", "elephant"
    };
    int word_count = (int)(sizeof words / sizeof words[0]);

    /* Стадии виселицы: индекс = число ошибок (0..MAX_WRONG). */
    const char *gallows[MAX_WRONG + 1] = {
        "  +---+\n  |   |\n      |\n      |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n      |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n /|   |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n /|\\  |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n /|\\  |\n /    |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n /|\\  |\n / \\  |\n      |\n========="
    };

    srand((unsigned int)time(NULL));

    const char *secret = words[rand() % word_count];
    int len = (int)strlen(secret);

    int guessed[26] = {0};   /* guessed[c - 'a'] == 1, если букву уже называли */
    int wrong = 0;

    printf("Игра \"Виселица\". Угадай слово по буквам!\n");

    while (wrong < MAX_WRONG) {
        printf("\n%s\n\n", gallows[wrong]);

        int revealed = draw_word(secret, guessed);
        if (revealed == len) {
            printf("Победа! Слово: %s\n", secret);
            return 0;
        }

        printf("Ошибок: %d из %d\n", wrong, MAX_WRONG);
        printf("Твоя буква: ");

        char c;
        if (scanf(" %c", &c) != 1) {
            break;   /* ввод кончился (EOF) */
        }

        if (c >= 'A' && c <= 'Z') {
            c = (char)(c - 'A' + 'a');   /* в нижний регистр */
        }
        if (c < 'a' || c > 'z') {
            printf("Это не английская буква. Попробуй ещё.\n");
            continue;
        }
        if (guessed[c - 'a']) {
            printf("Букву '%c' ты уже называл.\n", c);
            continue;
        }
        guessed[c - 'a'] = 1;

        int found = 0;
        for (int i = 0; i < len; i++) {
            if (secret[i] == c) {
                found = 1;
                break;
            }
        }
        if (found) {
            printf("Есть такая буква!\n");
        } else {
            wrong++;
            printf("Мимо.\n");
        }
    }

    printf("\n%s\n\n", gallows[MAX_WRONG]);
    printf("Ты проиграл. Слово было: %s\n", secret);
    return 0;
}
