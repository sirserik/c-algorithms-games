#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* дочитывает строку до конца; возвращает 1, если в остатке
   были только пробелы, и 0, если там нашлось что-то ещё */
int rest_is_empty(void)
{
    int empty = 1;
    int c = getchar();
    while (c != '\n' && c != EOF) {
        if (c != ' ' && c != '\t') {
            empty = 0;             /* за числом лишний символ */
        }
        c = getchar();
    }
    return empty;
}

int main(void)
{
    srand((unsigned int)time(NULL));

    int secret = rand() % 100 + 1;
    int guess = 0;
    int attempts = 0;

    printf("Я загадал число от 1 до 100. Угадай!\n");

    while (guess != secret) {
        printf("Твой вариант: ");

        int value;
        int rc = scanf("%9d", &value);
        if (rc == EOF) {           /* ввод закончился (Ctrl+D) */
            printf("\nИгра прервана. Я загадывал %d.\n", secret);
            return 0;
        }

        int empty = rest_is_empty();
        if (rc != 1 || empty == 0) {
            printf("Не понял. Введи одно целое число от 1 до 100.\n");
            continue;
        }

        if (value < 1 || value > 100) {   /* мимо правил игры */
            printf("Число должно быть от 1 до 100.\n");
            continue;
        }

        guess = value;
        attempts++;

        if (guess < secret) {
            printf("Больше.\n");
        } else if (guess > secret) {
            printf("Меньше.\n");
        } else {
            printf("Угадал! Это было число %d.\n", secret);
            printf("Тебе понадобилось попыток: %d.\n", attempts);
        }
    }

    return 0;
}
