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

        int rc = scanf("%d", &guess);
        if (rc == EOF) {           /* ввод закончился (Ctrl+D) */
            printf("\nИгра прервана. Я загадывал %d.\n", secret);
            return 0;
        }
        if (rc != 1) {             /* ввели не число */
            printf("Это не число. Введи целое от 1 до 100.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
                /* выбрасываем мусорный ввод до конца строки */
            }
            continue;
        }

        if (guess < 1 || guess > 100) {   /* мимо правил игры */
            printf("Число должно быть от 1 до 100.\n");
            continue;
        }

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
