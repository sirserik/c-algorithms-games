#include <stdio.h>

/* Слияние двух отсортированных массивов a и b в результат out.
   out должен вмещать na + nb элементов. */
void merge(const int *a, int na, const int *b, int nb, int *out)
{
    int i = 0, j = 0, k = 0;
    while (i < na && j < nb) {       /* пока есть в обоих */
        if (a[i] <= b[j]) {
            out[k++] = a[i++];       /* меньший идёт первым */
        } else {
            out[k++] = b[j++];
        }
    }
    while (i < na) {                 /* хвост a, если остался */
        out[k++] = a[i++];
    }
    while (j < nb) {                 /* хвост b, если остался */
        out[k++] = b[j++];
    }
}

int main(void)
{
    int a[] = {1, 3, 5, 7, 9};
    int b[] = {2, 4, 6, 8, 10, 12};
    int na = (int)(sizeof a / sizeof a[0]);
    int nb = (int)(sizeof b / sizeof b[0]);

    int out[11];                     /* na + nb = 5 + 6 = 11 */
    merge(a, na, b, nb, out);

    for (int i = 0; i < na + nb; i++) {
        printf("%d ", out[i]);
    }
    printf("\n");
    return 0;
}
