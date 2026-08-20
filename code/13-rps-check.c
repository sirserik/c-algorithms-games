/* Проверка к главе 13: сверяем два способа определить победителя
   в «камень, ножницы, бумага» на всех девяти парах ходов.
   Наивные if из шага 4 против формулы с остатком из шага 5. */

#include <stdio.h>

enum Move { ROCK, SCISSORS, PAPER };

const char *move_name(int m)
{
    if (m == ROCK)     return "камень";
    if (m == SCISSORS) return "ножницы";
    return "бумага";
}

/* Наивный способ: 0 - ничья, 1 - игрок, 2 - компьютер. */
int winner_if(int player, int computer)
{
    if (player == computer) {
        return 0;
    }
    if ((player == ROCK     && computer == SCISSORS) ||
        (player == SCISSORS && computer == PAPER)    ||
        (player == PAPER    && computer == ROCK)) {
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
    const char *who[3] = { "ничья", "игрок", "компьютер" };

    for (int p = 0; p < 3; p++) {
        for (int c = 0; c < 3; c++) {
            int a = winner_if(p, c);
            int b = winner_mod(p, c);
            printf("игрок %d (%s) против %d (%s): "
                   "if=%d формула=%d -> %s%s\n",
                   p, move_name(p), c, move_name(c), a, b, who[b],
                   (a == b) ? "" : "   <-- РАСХОЖДЕНИЕ!");
        }
    }
    return 0;
}
