#include <stdio.h>

/* Своя длина строки: считаем символы до '\0'. */
size_t my_strlen(const char *s)
{
    size_t n = 0;
    while (s[n] != '\0') {  /* пока не дошли до конца строки */
        n++;
    }
    return n;
}

/* Своё копирование: льём src в dst посимвольно, вместе с '\0'. */
char *my_strcpy(char *dst, const char *src)
{
    char *start = dst;          /* запомнили начало, чтобы вернуть */
    while ((*dst = *src) != '\0') {
        dst++;
        src++;
    }
    return start;
}

/* Своё сравнение: 0 если равны, иначе разница первых отличий. */
int my_strcmp(const char *a, const char *b)
{
    while (*a != '\0' && *a == *b) {
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

int main(void)
{
    char buf[64];

    printf("my_strlen(\"Hello\") = %zu\n", my_strlen("Hello"));

    my_strcpy(buf, "skopiruem");
    printf("my_strcpy -> %s\n", buf);

    printf("my_strcmp(abc, abc) = %d\n", my_strcmp("abc", "abc"));
    printf("my_strcmp(abc, abd) = %d\n", my_strcmp("abc", "abd"));
    printf("my_strcmp(abd, abc) = %d\n", my_strcmp("abd", "abc"));
    return 0;
}
