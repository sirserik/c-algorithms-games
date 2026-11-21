#include <stdio.h>

/* Печать поля 3x3 с разделителями. */
void print_board(char board[3][3])
{
    printf("\n");
    for (int row = 0; row < 3; row++) {
        printf(" %c | %c | %c \n",
               board[row][0], board[row][1], board[row][2]);
        if (row < 2) {
            printf("---+---+---\n");
        }
    }
    printf("\n");
}

/* Проверка: занял ли игрок player три клетки в ряд?
   Возвращает 1 (победа) или 0. */
int has_won(char board[3][3], char player)
{
    /* три строки */
    for (int row = 0; row < 3; row++) {
        if (board[row][0] == player &&
            board[row][1] == player &&
            board[row][2] == player) {
            return 1;
        }
    }
    /* три столбца */
    for (int col = 0; col < 3; col++) {
        if (board[0][col] == player &&
            board[1][col] == player &&
            board[2][col] == player) {
            return 1;
        }
    }
    /* главная диагональ */
    if (board[0][0] == player &&
        board[1][1] == player &&
        board[2][2] == player) {
        return 1;
    }
    /* побочная диагональ */
    if (board[0][2] == player &&
        board[1][1] == player &&
        board[2][0] == player) {
        return 1;
    }
    return 0;
}

/* Дочитывает строку до конца (как в главе 12): 1 - в остатке
   были только пробелы, 0 - там нашлось что-то ещё. */
int rest_is_empty(void)
{
    int empty = 1;
    int c = getchar();
    while (c != '\n' && c != EOF) {
        if (c != ' ' && c != '\t') {
            empty = 0;
        }
        c = getchar();
    }
    return empty;
}

int main(void)
{
    char board[3][3];

    /* Заполняем поле цифрами '1'..'9' - подсказками номеров. */
    char digit = '1';
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            board[row][col] = digit;
            digit++;
        }
    }

    char player = 'X';   /* первым ходит X */
    int moves = 0;       /* сколько ходов сделано */

    printf("Крестики-нолики. Игроки X и O ходят по очереди.\n");
    printf("Выбирай клетку числом от 1 до 9.\n");

    while (1) {
        print_board(board);
        printf("Ход игрока %c. Клетка (1-9): ", player);

        int cell;
        int rc = scanf("%9d", &cell);
        if (rc == EOF) {                 /* ввод закончился (Ctrl+D) */
            printf("\nИгра прервана.\n");
            return 0;
        }

        int empty = rest_is_empty();
        if (rc != 1 || empty == 0) {
            printf("Не понял. Введи одно число от 1 до 9.\n");
            continue;
        }
        if (cell < 1 || cell > 9) {
            printf("Нет такой клетки. Нужно число от 1 до 9.\n");
            continue;
        }

        /* Переводим номер 1..9 в индексы строки и столбца. */
        int row = (cell - 1) / 3;
        int col = (cell - 1) % 3;

        if (board[row][col] == 'X' || board[row][col] == 'O') {
            printf("Клетка %d уже занята. Выбери другую.\n", cell);
            continue;
        }

        board[row][col] = player;        /* ставим фигуру */
        moves++;

        if (has_won(board, player) == 1) {
            print_board(board);
            printf("Победил игрок %c! Ходов: %d.\n", player, moves);
            return 0;
        }
        if (moves == 9) {                /* девять ходов - поле полное */
            print_board(board);
            printf("Ничья! Поле заполнено.\n");
            return 0;
        }

        /* Меняем игрока: X -> O, O -> X. */
        if (player == 'X') {
            player = 'O';
        } else {
            player = 'X';
        }
    }
}
