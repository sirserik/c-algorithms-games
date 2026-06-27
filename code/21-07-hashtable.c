#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NBUCKETS 16   /* число корзин (степень двойки — удобно) */

/* Запись в цепочке: ключ-строка, счётчик и указатель на следующую
   запись в той же корзине (разрешение коллизий цепочками). */
typedef struct Entry {
    char *key;
    int count;
    struct Entry *next;
} Entry;

typedef struct {
    Entry *buckets[NBUCKETS];
} HashTable;

/* Хеш строки djb2 — простой и хороший для коротких ключей. */
unsigned long hash_str(const char *s)
{
    unsigned long h = 5381;
    int c;
    while ((c = (unsigned char)*s++) != 0) {
        h = h * 33u + (unsigned long)c;
    }
    return h;
}

void table_init(HashTable *t)
{
    for (int i = 0; i < NBUCKETS; i++) {
        t->buckets[i] = NULL;
    }
}

/* Увеличить счётчик слова на 1 (или добавить новое). 0 — нет памяти. */
int table_inc(HashTable *t, const char *key)
{
    size_t idx = hash_str(key) % NBUCKETS;

    /* проходим цепочку этой корзины — нет ли уже такого ключа */
    for (Entry *e = t->buckets[idx]; e != NULL; e = e->next) {
        if (strcmp(e->key, key) == 0) {
            e->count++;
            return 1;
        }
    }

    /* нового ключа нет — создаём запись и вешаем в начало цепочки */
    Entry *e = malloc(sizeof(Entry));
    if (e == NULL) {
        return 0;
    }
    e->key = malloc(strlen(key) + 1);   /* +1 под '\0' */
    if (e->key == NULL) {
        free(e);
        return 0;
    }
    strcpy(e->key, key);
    e->count = 1;
    e->next = t->buckets[idx];
    t->buckets[idx] = e;
    return 1;
}

int table_get(HashTable *t, const char *key)
{
    size_t idx = hash_str(key) % NBUCKETS;
    for (Entry *e = t->buckets[idx]; e != NULL; e = e->next) {
        if (strcmp(e->key, key) == 0) {
            return e->count;
        }
    }
    return 0;   /* нет такого слова */
}

void table_free(HashTable *t)
{
    for (int i = 0; i < NBUCKETS; i++) {
        Entry *e = t->buckets[i];
        while (e != NULL) {
            Entry *next = e->next;   /* запомнили до free */
            free(e->key);            /* строка ключа */
            free(e);                 /* сама запись */
            e = next;
        }
        t->buckets[i] = NULL;
    }
}

int main(void)
{
    HashTable t;
    table_init(&t);

    const char *words[] = {"cat", "dog", "cat", "bird", "dog", "cat"};
    int n = (int)(sizeof words / sizeof words[0]);
    for (int i = 0; i < n; i++) {
        if (!table_inc(&t, words[i])) {
            printf("Не хватило памяти\n");
            table_free(&t);
            return 1;
        }
    }

    printf("cat  = %d\n", table_get(&t, "cat"));
    printf("dog  = %d\n", table_get(&t, "dog"));
    printf("bird = %d\n", table_get(&t, "bird"));
    printf("fish = %d\n", table_get(&t, "fish"));

    table_free(&t);
    return 0;
}
