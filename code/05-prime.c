#include <stdio.h>

/* Проверка, простое ли число.
   is_prime_slow — делим на всё подряд до n-1.
   is_prime      — делим только до корня из n.

   Границу «до корня» пишем как i <= n / i, а НЕ как i * i <= n:
   при n около двух миллиардов произведение i * i уже не влезает в
   int, и проверка начинает врать (см. главу 4 про переполнение). */

int is_prime_slow(int n)
{
    if (n < 2) {
        return 0;
    }
    for (int i = 2; i < n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int is_prime(int n)
{
    if (n < 2) {
        return 0;
    }
    for (int i = 2; i <= n / i; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main(void)
{
    int n;
    printf("Введи число: ");
    if (scanf("%d", &n) != 1) {
        printf("Это не число.\n");
        return 1;
    }

    if (is_prime(n)) {
        printf("%d — простое число\n", n);
    } else if (n < 2) {
        printf("%d — ни простое, ни составное\n", n);
    } else {
        printf("%d — составное число\n", n);
    }
    return 0;
}
