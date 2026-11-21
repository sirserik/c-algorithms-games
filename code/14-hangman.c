#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WRONG 6   /* сколько ошибок позволяем до проигрыша */

/* Рисует слово: названные буквы - как есть, остальные - прочерком. */
void draw_word(const char word[], const int guessed[])
{
    for (int i = 0; word[i] != '\0'; i++) {
        char ch = word[i];
        if (ch < 'a' || ch > 'z') {   /* не строчная латиница:  */
            printf("%c ", ch);        /* показываем как есть    */
            continue;
        }
        if (guessed[ch - 'a'] == 1) {
            printf("%c ", ch);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

/* 1 - все буквы слова уже названы, 0 - ещё нет */
int word_is_open(const char word[], const int guessed[])
{
    for (int i = 0; word[i] != '\0'; i++) {
        char ch = word[i];
        if (ch >= 'a' && ch <= 'z' && guessed[ch - 'a'] == 0) {
            return 0;                 /* нашлась закрытая буква */
        }
    }
    return 1;
}

int main(void)
{
    const char *words[] = {
        "apple", "banana", "orange", "computer", "program",
        "keyboard", "function", "pointer", "rainbow", "elephant"
    };
    int word_count = (int)(sizeof(words) / sizeof(words[0]));

    /* Стадии виселицы: индекс = число ошибок (0..MAX_WRONG). */
    const char *gallows[MAX_WRONG + 1] = {
        "  +---+\n"
        "  |   |\n"
        "      |\n"
        "      |\n"
        "      |\n"
        "      |\n"
        "=========",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        "      |\n"
        "      |\n"
        "      |\n"
        "=========",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        "  |   |\n"
        "      |\n"
        "      |\n"
        "=========",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        " /|   |\n"
        "      |\n"
        "      |\n"
        "=========",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        " /|\\  |\n"
        "      |\n"
        "      |\n"
        "=========",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        " /|\\  |\n"
        " /    |\n"
        "      |\n"
        "=========",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        " /|\\  |\n"
        " / \\  |\n"
        "      |\n"
        "========="
    };

    srand((unsigned int)time(NULL));

    const char *secret = words[rand() % word_count];
    int len = (int)strlen(secret);

    /* guessed[номер буквы] == 1, если букву уже называли */
    int guessed[26] = {0};
    int wrong = 0;

    printf("Игра \"Виселица\". Угадай слово по буквам!\n");

    while (wrong < MAX_WRONG) {
        printf("\n%s\n\n", gallows[wrong]);

        draw_word(secret, guessed);
        if (word_is_open(secret, guessed) == 1) {
            printf("Победа! Слово: %s\n", secret);
            return 0;
        }

        printf("Ошибок: %d из %d\n", wrong, MAX_WRONG);
        printf("Твоя буква: ");

        char c;
        if (scanf(" %c", &c) != 1) {
            /* ввод кончился (Ctrl+D) - это не проигрыш, а выход */
            printf("\nИгра прервана. Слово было: %s\n", secret);
            return 0;
        }

        if (c >= 'A' && c <= 'Z') {
            c = (char)(c - 'A' + 'a');   /* в нижний регистр */
        }
        if (c < 'a' || c > 'z') {
            printf("Это не английская буква. Попробуй ещё.\n");
            continue;
        }

        int index = c - 'a';             /* номер буквы: 0..25 */
        if (guessed[index] == 1) {
            printf("Букву '%c' ты уже называл.\n", c);
            continue;
        }
        guessed[index] = 1;

        int found = 0;
        for (int i = 0; i < len; i++) {
            if (secret[i] == c) {
                found = 1;
                break;
            }
        }
        if (found == 1) {
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
