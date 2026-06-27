#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum Move { ROCK, SCISSORS, PAPER };

const char *move_name(int m)
{
    if (m == ROCK)     return "камень";
    if (m == SCISSORS) return "ножницы";
    return "бумага";
}

int main(void)
{
    srand((unsigned int)time(NULL));

    int player_score = 0;
    int computer_score = 0;
    int rounds = 0;
    char buf[16];

    printf("Камень, ножницы, бумага!\n");
    printf("Выбор: 0 - камень, 1 - ножницы, 2 - бумага, q - выход.\n");

    while (1) {
        printf("\nТвой ход: ");

        if (scanf("%15s", buf) != 1) {
            break;              /* ввод кончился (Ctrl+D) - выходим */
        }

        if (buf[0] == 'q' || buf[0] == 'Q') {
            break;
        }

        if (buf[0] < '0' || buf[0] > '2' || buf[1] != '\0') {
            printf("Не понял. Введи 0, 1, 2 или q.\n");
            continue;
        }

        int player = buf[0] - '0';
        int computer = rand() % 3;

        printf("Ты: %s. Компьютер: %s. ",
               move_name(player), move_name(computer));

        int result = (computer - player + 3) % 3;

        if (result == 0) {
            printf("Ничья.\n");
        } else if (result == 1) {
            printf("Ты выиграл раунд!\n");
            player_score++;
        } else {
            printf("Компьютер выиграл раунд.\n");
            computer_score++;
        }

        rounds++;
    }

    printf("\n=== Итог ===\n");
    printf("Сыграно раундов: %d\n", rounds);
    printf("Ты: %d, компьютер: %d.\n", player_score, computer_score);

    if (player_score > computer_score) {
        printf("Поздравляю, ты победил в матче!\n");
    } else if (player_score < computer_score) {
        printf("Компьютер оказался сильнее. В другой раз!\n");
    } else {
        printf("Ничья по матчу.\n");
    }

    return 0;
}
