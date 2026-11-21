/* Глава 17. Игра «Змейка» в терминале.
 *
 * Управление: W A S D или стрелки, q — выход.
 * Компиляция (macOS/Linux):
 *   cc -std=c11 -Wall -Wextra -O2 17-snake.c -o snake
 *
 * Программа интерактивная: запускать в обычном терминале, не в пайпе.
 */

/* usleep не из стандарта Си, а из POSIX. На Linux строгий -std=c11
   прячет её объявление, и эта строка просит его показать. На macOS
   она ничего не меняет, но и не мешает. Ставить ДО #include. */
#define _DEFAULT_SOURCE

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
int snake_x[MAX_LEN];   /* координаты сегментов: [0] — голова */
int snake_y[MAX_LEN];
int snake_len;          /* сколько сегментов сейчас */
int dir_x;              /* куда голова поедет в следующем кадре */
int dir_y;
int moved_x;            /* куда она реально поехала в прошлом кадре */
int moved_y;
int food_x;             /* где лежит еда */
int food_y;
int score;              /* счёт */
int game_over;          /* 0 — играем, 1 — конец */
int game_won;           /* 1 — змейка заняла всё поле */

/* ---- Сырой режим терминала ---- */
struct termios orig_termios;   /* сюда сохраним исходные настройки */
int raw_saved;                 /* 1 — orig_termios заполнен */

/* Вернуть терминал в нормальный режим. */
void disable_raw_mode(void)
{
    if (raw_saved) {
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
    }
}

/* Применить игровой режим поверх сохранённых настроек. */
void apply_raw_mode(void)
{
    struct termios raw = orig_termios;          /* копия для правки */

    raw.c_lflag = raw.c_lflag & ~(ICANON | ECHO); /* без Enter и эха */
    raw.c_cc[VMIN] = 0;                         /* read не ждёт символов */
    raw.c_cc[VTIME] = 0;                        /* и не ждёт по таймеру */

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);     /* применить сейчас же */
}

/* Включить «сырой» неблокирующий ввод: без ожидания Enter и без эха.
   Возвращает 1 при успехе, 0 — если ввод не терминал (пайп, файл). */
int enable_raw_mode(void)
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
        return 0;                               /* это не терминал */
    }
    raw_saved = 1;                              /* запомнили как было */
    apply_raw_mode();
    return 1;
}

/* Ctrl+C, kill, закрытие окна: чиним терминал и выходим сами. */
void on_fatal_signal(int sig)
{
    disable_raw_mode();
    _exit(128 + sig);    /* 128 + номер сигнала — так принято в шеллах */
}

/* Нас разбудили командой fg — снова включаем игровой режим. */
void on_sigcont(int sig)
{
    (void)sig;
    apply_raw_mode();
}

/* Прочитать одну клавишу. Если её нет — вернуть -1 (не ждём ввода). */
int read_key(void)
{
    unsigned char c;                        /* байт 0..255 */
    if (read(STDIN_FILENO, &c, 1) == 1) {   /* прочитали ровно 1 байт */
        return c;
    }
    return -1;                              /* клавиши нет */
}

/* Положить еду в случайную свободную клетку.
   Возвращает 0, если свободных клеток не осталось: змейка заняла всё
   поле. Без этой проверки цикл ниже крутился бы вечно. */
int place_food(void)
{
    if (snake_len >= MAX_LEN) {
        food_x = -1;              /* свободных клеток нет: убираем еду */
        food_y = -1;              /* с поля, чтобы не закрыла голову */
        return 0;
    }
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
    return 1;
}

/* Нарисовать поле, змейку, еду и счёт. */
void draw(void)
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

    printf("Счёт: %d   (WASD или стрелки, q — выход)\n", score);
}

/* Сменить направление, не разрешая разворот на 180 градусов.
   Сравниваем с тем, куда змейка ПОЕХАЛА в прошлом кадре (moved_x/moved_y),
   а не с уже накопленным dir_x/dir_y: иначе два нажатия за один кадр
   (вверх, потом влево, пока ползём вправо) сложились бы в разворот. */
void set_dir(int nx, int ny)
{
    if (nx == -moved_x && ny == -moved_y) {
        return;   /* противоположное реальному движению — игнорируем */
    }
    dir_x = nx;
    dir_y = ny;
}

/* Разобрать одно нажатие: WASD, стрелки или q. */
void handle_key(int key)
{
    if (key == 27) {          /* Escape: так начинается стрелка */
        read_key();           /* второй байт '[' просто пропускаем */
        key = read_key();     /* третий байт — буква стрелки */
        if (key == 'A') {
            key = 'w';        /* стрелка вверх — то же, что W */
        } else if (key == 'B') {
            key = 's';        /* вниз */
        } else if (key == 'C') {
            key = 'd';        /* вправо */
        } else if (key == 'D') {
            key = 'a';        /* влево */
        } else {
            return;           /* какая-то другая клавиша — пропускаем */
        }
    }

    if (key == 'w' || key == 'W') {
        set_dir(0, -1);
    } else if (key == 's' || key == 'S') {
        set_dir(0, 1);
    } else if (key == 'a' || key == 'A') {
        set_dir(-1, 0);
    } else if (key == 'd' || key == 'D') {
        set_dir(1, 0);
    } else if (key == 'q' || key == 'Q') {
        game_over = 1;
    }
}

/* Один шаг змейки: сдвиг тела, новая голова, столкновения, еда. */
void update(void)
{
    /* фиксируем направление этого кадра — на него будет смотреть set_dir */
    moved_x = dir_x;
    moved_y = dir_y;

    /* куда голова СОБИРАЕТСЯ шагнуть */
    int nx = snake_x[0] + dir_x;
    int ny = snake_y[0] + dir_y;

    /* столкновение со стеной — проверяем ДО шага, чтобы на последнем
       кадре змейка осталась нарисованной целиком, а не без головы */
    if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) {
        game_over = 1;
        return;
    }

    /* столкновение с собственным телом. Хвост (последний сегмент) в этом
       кадре уползёт, поэтому его не считаем — вот почему i < snake_len-1. */
    for (int i = 1; i < snake_len - 1; i++) {
        if (snake_x[i] == nx && snake_y[i] == ny) {
            game_over = 1;
            return;
        }
    }

    /* запомним хвост — он пригодится, если змейка вырастет */
    int tail_x = snake_x[snake_len - 1];
    int tail_y = snake_y[snake_len - 1];

    /* сдвигаем тело: каждый сегмент встаёт на место предыдущего */
    for (int i = snake_len - 1; i > 0; i--) {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }

    /* голова делает шаг в текущем направлении */
    snake_x[0] = nx;
    snake_y[0] = ny;

    /* съели еду? */
    if (nx == food_x && ny == food_y) {
        if (snake_len < MAX_LEN) {
            snake_x[snake_len] = tail_x;   /* отрастили хвост обратно */
            snake_y[snake_len] = tail_y;
            snake_len++;
        }
        score += 10;
        if (!place_food()) {
            game_won = 1;                  /* поле заполнено целиком */
            game_over = 1;
        }
    }
}

int main(void)
{
    srand((unsigned int)time(NULL));   /* новый «случай» при каждом запуске */

    if (!enable_raw_mode()) {      /* переключаем терминал в игровой режим */
        fprintf(stderr, "Змейке нужен настоящий терминал: "
                        "запусти ./snake без пайпа и без < файла.\n");
        return 1;
    }
    atexit(disable_raw_mode);           /* вернуть режим при любом выходе */
    signal(SIGINT, on_fatal_signal);    /* Ctrl+C */
    signal(SIGTERM, on_fatal_signal);   /* kill */
    signal(SIGQUIT, on_fatal_signal);   /* Ctrl+\ */
    signal(SIGHUP, on_fatal_signal);    /* окно закрыли */
    signal(SIGCONT, on_sigcont);        /* fg после Ctrl+Z */

    /* начальная змейка из 3 сегментов в центре, ползёт вправо */
    snake_len = 3;
    int cx = WIDTH / 2;
    int cy = HEIGHT / 2;
    snake_x[0] = cx;
    snake_y[0] = cy;
    snake_x[1] = cx - 1;
    snake_y[1] = cy;
    snake_x[2] = cx - 2;
    snake_y[2] = cy;
    dir_x = 1;
    dir_y = 0;
    moved_x = dir_x;
    moved_y = dir_y;
    score = 0;
    game_over = 0;
    game_won = 0;

    place_food();
    draw();                        /* показываем стартовое поле сразу */

    while (!game_over) {
        /* разбираем все нажатия, накопившиеся за кадр */
        int key;
        while ((key = read_key()) != -1) {
            handle_key(key);
        }

        if (game_over) {
            break;             /* нажали q */
        }

        update();
        draw();

        usleep(150000);        /* пауза 0,15 с между кадрами */
    }

    disable_raw_mode();        /* счёт печатаем уже в нормальном режиме */

    if (game_won) {
        printf("\nПоле заполнено целиком — ты прошёл игру!\n");
    }
    printf("\nИгра окончена! Итоговый счёт: %d\n", score);
    return 0;
}
