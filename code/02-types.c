#include <stdio.h>
#include <limits.h>

int main(void)
{
    printf("char      %zu байт\n", sizeof(char));
    printf("int       %zu байта\n", sizeof(int));
    printf("long long %zu байт\n", sizeof(long long));
    printf("double    %zu байт\n", sizeof(double));
    printf("int вмещает от %d до %d\n", INT_MIN, INT_MAX);

    int big = INT_MAX;
    printf("big     = %d\n", big);
    big = big + 1;
    printf("big + 1 = %d\n", big);

    char c = 'A';
    printf("символ %c — это число %d\n", c, c);
    printf("а %c — это %d\n", c + 1, c + 1);

    return 0;
}
