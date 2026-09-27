#include <stdio.h>
#include <limits.h>

int main(void)
{
    printf("char      %zu байт\n", sizeof(char));
    printf("int       %zu байта\n", sizeof(int));
    printf("long      %zu байт\n", sizeof(long));
    printf("long long %zu байт\n", sizeof(long long));
    printf("double    %zu байт\n", sizeof(double));
    printf("char вмещает от %d до %d\n", CHAR_MIN, CHAR_MAX);
    printf("int  вмещает от %d до %d\n", INT_MIN, INT_MAX);

    unsigned int u = UINT_MAX;
    printf("u     = %u\n", u);
    u = u + 1;
    printf("u + 1 = %u\n", u);

    char c = 'A';
    printf("символ %c — это число %d\n", c, c);
    printf("а %c — это %d\n", c + 1, c + 1);

    return 0;
}
