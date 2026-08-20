/* 18-files.c — запись и чтение текстового файла.
   Пишем таблицу рекордов, читаем её обратно построчно и считаем сумму.
   Каждый вызов, который может провалиться, проверяем. */
#include <stdio.h>
#include <string.h>

int main(void)
{
    const char *path = "scores.txt";

    /* --- 1. Пишем файл --- */
    FILE *out = fopen(path, "w");
    if (out == NULL) {
        printf("Не могу открыть %s на запись\n", path);
        return 1;
    }

    fprintf(out, "anna 120\n");
    fprintf(out, "boris 95\n");
    fprintf(out, "vika 143\n");

    if (fclose(out) != 0) {
        printf("Ошибка при закрытии %s\n", path);
        return 1;
    }
    printf("Записал 3 строки в %s\n", path);

    /* --- 2. Читаем файл обратно --- */
    FILE *in = fopen(path, "r");
    if (in == NULL) {
        printf("Не могу открыть %s на чтение\n", path);
        return 1;
    }

    char line[128];
    int players = 0;
    int total = 0;

    while (fgets(line, sizeof line, in) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';       /* срезаем перевод строки */

        char name[64];
        int score;
        if (sscanf(line, "%63s %d", name, &score) == 2) {
            players++;
            total += score;
            printf("%d. %s — %d очков\n", players, name, score);
        } else {
            printf("не разобрал строку: \"%s\"\n", line);
        }
    }

    if (ferror(in))
        printf("Чтение оборвалось на ошибке\n");
    fclose(in);

    if (players > 0)
        printf("Игроков: %d, всего очков: %d\n", players, total);

    remove(path);                       /* убираем за собой */
    return 0;
}
