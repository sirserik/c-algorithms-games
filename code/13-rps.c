#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum Move { ROCK, SCISSORS, PAPER };

const char *move_name(int move)
{
    if (move == ROCK) {
        return "камень";
    }
    if (move == SCISSORS) {
        return "ножницы";
    }
    return "бумага";
}

/* 0 - ничья, 1 - выиграл игрок, 2 - выиграл компьютер */
int round_winner(int player, int computer)
{
    if (player == computer) {
        return 0;
    }
    if (player == ROCK && computer == SCISSORS) {
        return 1;
    }
    if (player == SCISSORS && computer == PAPER) {
        return 1;
    }
    if (player == PAPER && computer == ROCK) {
        return 1;
    }
    return 2;
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

        int player = -1;        /* -1 - ход пока не распознан */
        if (buf[1] == '\0') {   /* введён ровно один символ */
            if (buf[0] == '0') {
                player = ROCK;
            }
            if (buf[0] == '1') {
                player = SCISSORS;
            }
            if (buf[0] == '2') {
                player = PAPER;
            }
        }
        if (player == -1) {
            printf("Не понял. Введи 0, 1, 2 или q.\n");
            continue;
        }

        int computer = rand() % 3;

        printf("Ты: %s. Компьютер: %s. ",
               move_name(player), move_name(computer));

        int result = round_winner(player, computer);

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
