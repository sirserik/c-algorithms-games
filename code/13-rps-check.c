/* Проверка к главе 13: сверяем два способа определить победителя
   в «камень, ножницы, бумага» на всех девяти парах ходов.
   Явные if из шага 4 против формулы с остатком из шага 5. */

#include <stdio.h>

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

/* Тот же ответ одной формулой. */
int winner_mod(int player, int computer)
{
    return (computer - player + 3) % 3;
}

int main(void)
{
    for (int player = 0; player < 3; player++) {
        for (int computer = 0; computer < 3; computer++) {
            int by_if = round_winner(player, computer);
            int by_formula = winner_mod(player, computer);
            printf("%s против %s: if=%d формула=%d",
                   move_name(player), move_name(computer),
                   by_if, by_formula);
            if (by_if != by_formula) {
                printf("   <-- РАСХОЖДЕНИЕ!");
            }
            printf("\n");
        }
    }
    return 0;
}
