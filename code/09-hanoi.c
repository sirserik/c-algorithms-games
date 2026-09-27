#include <stdio.h>

/* Перенести n дисков со стержня from на стержень to,
   используя via как промежуточный.
   База: 0 дисков — ничего не делаем.
   Шаг: n-1 на via, большой на to, n-1 с via на to. */
void hanoi(int n, char from, char to, char via)
{
    if (n == 0) {                 /* база: переносить нечего */
        return;
    }
    hanoi(n - 1, from, via, to);  /* верхние n-1 уводим на промежуточный */
    printf("Переложить диск %d с %c на %c\n", n, from, to);
    hanoi(n - 1, via, to, from);  /* возвращаем n-1 на цель */
}

int main(void)
{
    int n;
    printf("Сколько дисков? ");
    if (scanf("%d", &n) != 1) {
        printf("Это не число.\n");
        return 1;
    }
    if (n < 0) {
        printf("Нужно n >= 0.\n");
        return 1;
    }
    if (n > 30) {
        printf("Больше 30 не надо: это уже миллиард ходов.\n");
        return 1;
    }

    hanoi(n, 'A', 'C', 'B');      /* с A на C, B — запасной */
    return 0;
}
