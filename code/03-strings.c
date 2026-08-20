#include <stdio.h>
#include <string.h>

int main(void)
{
    char name[] = "Anna";

    printf("Привет, %s!\n", name);
    printf("strlen(name) = %zu\n", strlen(name));
    printf("sizeof name  = %zu\n", sizeof name);
    printf("байт после последней буквы: код %d\n", name[strlen(name)]);

    char copy[16];
    strcpy(copy, name);
    copy[0] = 'J';
    printf("копия после правки: %s, оригинал: %s\n", copy, name);

    /* Два разных массива с одинаковым текстом. */
    char a[] = "exit";
    char b[] = "exit";
    const char *pa = a;              /* через указатели — чтобы  */
    const char *pb = b;              /* компилятор не ругался    */
    printf("адреса равны? %d\n", pa == pb);
    printf("strcmp(a, b)  = %d\n", strcmp(a, b));
    printf("strcmp(\"abc\", \"abd\") = %d\n", strcmp("abc", "abd"));

    char line[64];
    printf("Введи команду: ");
    if (fgets(line, sizeof line, stdin) == NULL) {
        printf("\nВвод закончился\n");
        return 1;
    }
    printf("как есть:   [%s]\n", line);
    printf("длина: %zu\n", strlen(line));

    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }
    printf("без '\\n':   [%s]\n", line);
    printf("длина: %zu\n", strlen(line));

    if (strcmp(line, "exit") == 0) {
        printf("Это команда выхода. Пока!\n");
    } else {
        printf("Такой команды не знаю\n");
    }
    return 0;
}
