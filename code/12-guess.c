#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

        int next = getchar();      /* что стоит сразу за числом */
        while (next == ' ' || next == '\t') {
            next = getchar();      /* пробелы в конце прощаем */
        }
        if (rc != 1 || (next != '\n' && next != EOF)) {
            printf("Не понял. Введи одно целое число от 1 до 100.\n");
            while (next != '\n' && next != EOF) {
                next = getchar();  /* выбрасываем остаток строки */
            }
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
