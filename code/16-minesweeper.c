#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define H 9        /* высота поля (строк)   */
#define W 9        /* ширина поля (столбцов) */
#define MINES 10   /* сколько мин на поле    */

int mines[H][W];   /* 1 — здесь мина, 0 — пусто           */
int opened[H][W];  /* 1 — клетка открыта игроком, 0 — нет */

/* Сколько мин стоит вокруг клетки (r, c) — среди 8 соседей. */
int count_neighbors(int r, int c)
{
    int total = 0;
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) {
                continue;            /* саму клетку не считаем */
            }
            int nr = r + dr;         /* строка соседа  */
            int nc = c + dc;         /* столбец соседа */
            if (nr < 0 || nr >= H || nc < 0 || nc >= W) {
                continue;            /* сосед за краем поля */
            }
            if (mines[nr][nc] == 1) {
                total++;
            }
        }
    }
    return total;
}

/* Рекурсивная заливка: открыть клетку (r, c) и, если она пустая,
   открыть всех её соседей, и так дальше. */
void reveal(int r, int c)
{
    if (r < 0 || r >= H || c < 0 || c >= W) {
        return;                      /* база: вышли за поле */
    }
    if (opened[r][c] == 1) {
        return;                      /* база: уже открыто */
    }

    opened[r][c] = 1;                /* открываем эту клетку */

    if (count_neighbors(r, c) != 0) {
        return;                      /* база: рядом есть мины — стоп */
    }

    /* Клетка пустая (0 соседей) — продолжаем заливку на 8 соседей. */
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) {
                continue;
            }
            reveal(r + dr, c + dc);
        }
    }
}

/* Расставить MINES мин по случайным клеткам без повторов.
   Клетку (safe_r, safe_c) — первый ход игрока — не минируем,
   иначе можно проиграть на первом же ходу, ничего не зная о поле. */
void place_mines(int safe_r, int safe_c)
{
    int placed = 0;
    while (placed < MINES) {
        int r = rand() % H;
        int c = rand() % W;
        if (r == safe_r && c == safe_c) {
            continue;                /* сюда игрок сейчас ткнёт */
        }
        if (mines[r][c] == 0) {      /* клетка ещё свободна */
            mines[r][c] = 1;
            placed++;
        }
    }
}

/* Напечатать поле. Если show_mines != 0 — показать и мины (при проигрыше). */
void print_board(int show_mines)
{
    printf("   ");
    for (int c = 0; c < W; c++) {
        printf("%d ", c + 1);        /* шапка со столбцами 1..W */
    }
    printf("\n");

    for (int r = 0; r < H; r++) {
        printf("%2d ", r + 1);       /* номер строки слева */
        for (int c = 0; c < W; c++) {
            if (show_mines && mines[r][c] == 1) {
                printf("* ");
            } else if (opened[r][c] == 0) {
                printf(". ");        /* закрытая клетка */
            } else {
                int n = count_neighbors(r, c);
                if (n == 0) {
                    printf("  ");    /* открыто и пусто */
                } else {
                    printf("%d ", n);
                }
            }
        }
        printf("\n");
    }
}

/* Сколько клеток уже открыто. */
int count_opened(void)
{
    int total = 0;
    for (int r = 0; r < H; r++) {
        for (int c = 0; c < W; c++) {
            if (opened[r][c] == 1) {
                total++;
            }
        }
    }
    return total;
}

int main(void)
{
    srand((unsigned int)time(NULL));

    int mines_placed = 0;            /* мины ставим после первого хода */
    int safe_cells = H * W - MINES;  /* столько клеток без мин */

    printf("Сапёр %dx%d, мин: %d\n", H, W, MINES);
    printf("Вводи координаты: строка 1..%d и столбец 1..%d.\n", H, W);

    while (1) {
        print_board(0);

        if (count_opened() == safe_cells) {
            printf("Победа! Все безопасные клетки открыты.\n");
            break;
        }

        printf("Ход (строка столбец): ");
        int row, col;
        int rc = scanf("%d %d", &row, &col);
        if (rc == EOF) {
            printf("\nИгра прервана.\n");
            break;
        }
        if (rc != 2) {
            printf("Нужно ввести два числа. Попробуй ещё раз.\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* выбрасываем мусорный ввод до конца строки */
            }
            continue;
        }

        row--;                       /* игрок считает с 1, массив — с 0 */
        col--;

        if (row < 0 || row >= H || col < 0 || col >= W) {
            printf("Координаты вне поля: строка 1..%d, столбец 1..%d.\n",
                   H, W);
            continue;
        }
        if (opened[row][col] == 1) {
            printf("Эта клетка уже открыта.\n");
            continue;
        }

        if (!mines_placed) {         /* самый первый ход задаёт поле */
            place_mines(row, col);
            mines_placed = 1;
        }

        if (mines[row][col] == 1) {
            print_board(1);          /* показываем все мины */
            printf("Бабах! Ты открыл мину. Игра окончена.\n");
            break;
        }

        reveal(row, col);            /* безопасный ход — заливаем */
    }

    return 0;
}
