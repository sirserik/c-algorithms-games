/* Глава 17. Игра «Змейка» в терминале.
 *
 * Управление: W A S D — вверх/влево/вниз/вправо, q — выход.
 * Компиляция (macOS/Linux):
 *   cc -std=c11 -Wall -Wextra -O2 17-snake.c -o snake
 *
 * Программа интерактивная: запускать в обычном терминале, не в пайпе.
 */

#define _DEFAULT_SOURCE   /* открыть доступ к usleep, read, termios в C11 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

/* ---- Размеры игрового поля (без учёта рамки) ---- */
#define WIDTH   20
#define HEIGHT  10
#define MAX_LEN (WIDTH * HEIGHT)   /* змейка не длиннее всех клеток поля */

/* ---- Состояние игры ---- */
static int snake_x[MAX_LEN];   /* координаты сегментов: [0] — голова */
static int snake_y[MAX_LEN];
static int snake_len;          /* сколько сегментов сейчас */
static int dir_x, dir_y;       /* куда голова поедет в следующем кадре */
static int moved_x, moved_y;   /* куда она реально поехала в прошлом кадре */
static int food_x, food_y;     /* где лежит еда */
static int score;              /* счёт */
static int game_over;          /* 0 — играем, 1 — конец */

/* ---- Сырой режим терминала ---- */
static struct termios orig_termios;   /* сюда сохраним исходные настройки */

/* Вернуть терминал в нормальный режим. */
static void disable_raw_mode(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

/* Ctrl+C во время игры: чиним терминал и только потом умираем.
   Сам по себе сигнал убивает процесс мимо atexit, поэтому перехватываем. */
static void on_sigint(int sig)
{
    (void)sig;
    disable_raw_mode();
    _exit(130);          /* 128 + номер SIGINT — так принято в шеллах */
}

/* Включить «сырой» неблокирующий ввод: без ожидания Enter и без эха. */
static void enable_raw_mode(void)
{
    tcgetattr(STDIN_FILENO, &orig_termios);   /* запомнили как было */
    struct termios raw = orig_termios;        /* копия для правки */

    raw.c_lflag &= ~(ICANON | ECHO);          /* снять каноническ. режим и эхо */
    raw.c_cc[VMIN]  = 0;                       /* read не ждёт символов */
    raw.c_cc[VTIME] = 0;                       /* и не ждёт по таймеру */

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);    /* применить сейчас же */
}

/* Прочитать одну клавишу. Если её нет — вернуть -1 (не ждём ввода). */
static int read_key(void)
{
    char c;
    ssize_t n = read(STDIN_FILENO, &c, 1);
    if (n == 1) {
        return (unsigned char)c;
    }
    return -1;
}

/* Положить еду в случайную свободную клетку. */
static void place_food(void)
{
    int ok = 0;
    while (!ok) {
        food_x = rand() % WIDTH;
        food_y = rand() % HEIGHT;
        ok = 1;
        for (int i = 0; i < snake_len; i++) {
            if (snake_x[i] == food_x && snake_y[i] == food_y) {
                ok = 0;   /* попали в тело — выбираем заново */
                break;
            }
        }
    }
}

/* Нарисовать поле, змейку, еду и счёт. */
static void draw(void)
{
    printf("\033[H\033[2J");   /* курсор в угол + очистка экрана */

    /* верхняя рамка */
    for (int x = 0; x < WIDTH + 2; x++) {
        putchar('#');
    }
    putchar('\n');

    for (int y = 0; y < HEIGHT; y++) {
        putchar('#');                       /* левая стенка */
        for (int x = 0; x < WIDTH; x++) {
            char ch = ' ';
            if (x == food_x && y == food_y) {
                ch = '*';                   /* еда */
            } else {
                for (int i = 0; i < snake_len; i++) {
                    if (snake_x[i] == x && snake_y[i] == y) {
                        ch = (i == 0) ? 'O' : 'o';   /* голова / тело */
                        break;
                    }
                }
            }
            putchar(ch);
        }
        putchar('#');                       /* правая стенка */
        putchar('\n');
    }

    /* нижняя рамка */
    for (int x = 0; x < WIDTH + 2; x++) {
        putchar('#');
    }
    putchar('\n');

    printf("Счёт: %d   (WASD — движение, q — выход)\n", score);
}

/* Сменить направление, не разрешая разворот на 180 градусов.
   Сравниваем с тем, куда змейка ПОЕХАЛА в прошлом кадре (moved_x/moved_y),
   а не с уже накопленным dir_x/dir_y: иначе два нажатия за один кадр
   (вверх, потом влево, пока ползём вправо) сложились бы в разворот. */
static void set_dir(int nx, int ny)
{
    if (nx == -moved_x && ny == -moved_y) {
        return;   /* противоположное реальному движению — игнорируем */
    }
    dir_x = nx;
    dir_y = ny;
}

/* Один шаг змейки: сдвиг тела, новая голова, столкновения, еда. */
static void update(void)
{
    /* фиксируем направление этого кадра — на него будет смотреть set_dir */
    moved_x = dir_x;
    moved_y = dir_y;

    /* запомним хвост — он пригодится, если змейка вырастет */
    int tail_x = snake_x[snake_len - 1];
    int tail_y = snake_y[snake_len - 1];

    /* сдвигаем тело: каждый сегмент встаёт на место предыдущего */
    for (int i = snake_len - 1; i > 0; i--) {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }

    /* голова делает шаг в текущем направлении */
    snake_x[0] += dir_x;
    snake_y[0] += dir_y;

    /* столкновение со стеной */
    if (snake_x[0] < 0 || snake_x[0] >= WIDTH ||
        snake_y[0] < 0 || snake_y[0] >= HEIGHT) {
        game_over = 1;
        return;
    }

    /* столкновение с собственным телом (со 2-го сегмента) */
    for (int i = 1; i < snake_len; i++) {
        if (snake_x[i] == snake_x[0] && snake_y[i] == snake_y[0]) {
            game_over = 1;
            return;
        }
    }

    /* съели еду? */
    if (snake_x[0] == food_x && snake_y[0] == food_y) {
        if (snake_len < MAX_LEN) {
            snake_x[snake_len] = tail_x;   /* отрастили хвост обратно */
            snake_y[snake_len] = tail_y;
            snake_len++;
        }
        score += 10;
        place_food();
    }
}

int main(void)
{
    srand((unsigned)time(NULL));   /* разный «случай» при каждом запуске */

    enable_raw_mode();             /* переключаем терминал в игровой режим */
    atexit(disable_raw_mode);      /* вернуть режим при любом обычном выходе */
    signal(SIGINT, on_sigint);     /* ...и при Ctrl+C тоже */

    /* начальная змейка из 3 сегментов в центре, ползёт вправо */
    snake_len = 3;
    int cx = WIDTH / 2;
    int cy = HEIGHT / 2;
    snake_x[0] = cx;     snake_y[0] = cy;
    snake_x[1] = cx - 1; snake_y[1] = cy;
    snake_x[2] = cx - 2; snake_y[2] = cy;
    dir_x = 1;
    dir_y = 0;
    moved_x = dir_x;
    moved_y = dir_y;
    score = 0;
    game_over = 0;

    place_food();
    draw();                        /* показываем стартовое поле сразу */

    while (!game_over) {
        /* считываем все накопившиеся нажатия */
        int key;
        while ((key = read_key()) != -1) {
            if (key == 'w' || key == 'W') set_dir(0, -1);
            else if (key == 's' || key == 'S') set_dir(0, 1);
            else if (key == 'a' || key == 'A') set_dir(-1, 0);
            else if (key == 'd' || key == 'D') set_dir(1, 0);
            else if (key == 'q' || key == 'Q') game_over = 1;
        }

        if (game_over) {
            break;             /* нажали q */
        }

        update();
        draw();

        usleep(150000);        /* пауза 0,15 с между кадрами */
    }

    disable_raw_mode();        /* ОБЯЗАТЕЛЬНО вернуть терминал как было */

    printf("\nИгра окончена! Итоговый счёт: %d\n", score);
    return 0;
}
