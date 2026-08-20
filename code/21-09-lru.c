#include <stdio.h>
#include <stdlib.h>

#define NBUCKETS 16

/* Узел LRU-кэша живёт сразу в двух структурах:
   - в двусвязном списке «по свежести» (prev/next): голова — самый
     свежий, хвост — самый старый кандидат на вытеснение;
   - в цепочке хеш-корзины (hnext) — для быстрого поиска по ключу. */
typedef struct Node {
    int key;
    int value;
    struct Node *prev;    /* соседи в списке свежести */
    struct Node *next;
    struct Node *hnext;   /* следующий в хеш-корзине */
} Node;

typedef struct {
    int capacity;
    int size;
    Node *head;           /* самый свежий */
    Node *tail;           /* самый старый */
    Node *buckets[NBUCKETS];
} LRU;

void lru_init(LRU *c, int capacity)
{
    c->capacity = capacity;
    c->size = 0;
    c->head = NULL;
    c->tail = NULL;
    for (int i = 0; i < NBUCKETS; i++) {
        c->buckets[i] = NULL;
    }
}

static unsigned bucket_of(int key)
{
    return (unsigned)key % NBUCKETS;
}

/* убрать узел из списка свежести (хеш не трогаем) */
static void list_unlink(LRU *c, Node *n)
{
    if (n->prev != NULL) {
        n->prev->next = n->next;
    } else {
        c->head = n->next;
    }
    if (n->next != NULL) {
        n->next->prev = n->prev;
    } else {
        c->tail = n->prev;
    }
}

/* поставить узел в голову — пометить как самый свежий */
static void list_push_front(LRU *c, Node *n)
{
    n->prev = NULL;
    n->next = c->head;
    if (c->head != NULL) {
        c->head->prev = n;
    }
    c->head = n;
    if (c->tail == NULL) {
        c->tail = n;
    }
}

static Node *hash_find(LRU *c, int key)
{
    for (Node *e = c->buckets[bucket_of(key)]; e != NULL; e = e->hnext) {
        if (e->key == key) {
            return e;
        }
    }
    return NULL;
}

/* get: 1 и значение в *out, если ключ есть. Обращение освежает запись. */
int lru_get(LRU *c, int key, int *out)
{
    Node *n = hash_find(c, key);
    if (n == NULL) {
        return 0;
    }
    list_unlink(c, n);
    list_push_front(c, n);   /* подняли наверх как самый свежий */
    *out = n->value;
    return 1;
}

void lru_put(LRU *c, int key, int value)
{
    Node *n = hash_find(c, key);
    if (n != NULL) {                 /* ключ уже есть — обновим и освежим */
        n->value = value;
        list_unlink(c, n);
        list_push_front(c, n);
        return;
    }

    if (c->size == c->capacity) {    /* кэш полон — вытесняем хвост */
        Node *old = c->tail;
        list_unlink(c, old);

        /* выдёргиваем old из его хеш-корзины */
        unsigned b = bucket_of(old->key);
        Node **pp = &c->buckets[b];
        while (*pp != old) {
            pp = &(*pp)->hnext;
        }
        *pp = old->hnext;

        free(old);
        c->size--;
    }

    n = malloc(sizeof(Node));
    if (n == NULL) {
        return;
    }
    n->key = key;
    n->value = value;
    n->hnext = c->buckets[bucket_of(key)];   /* в начало хеш-цепочки */
    c->buckets[bucket_of(key)] = n;
    list_push_front(c, n);                    /* и в голову списка свежести */
    c->size++;
}

void lru_free(LRU *c)
{
    Node *p = c->head;
    while (p != NULL) {
        Node *nx = p->next;
        free(p);
        p = nx;
    }
    c->head = NULL;
    c->tail = NULL;
    c->size = 0;
    for (int i = 0; i < NBUCKETS; i++) {   /* корзины тоже обнуляем: */
        c->buckets[i] = NULL;              /* иначе останутся висячие */
    }                                      /* указатели на free-нутые узлы */
}

int main(void)
{
    LRU c;
    lru_init(&c, 2);     /* кэш всего на 2 записи */

    lru_put(&c, 1, 100);
    lru_put(&c, 2, 200);

    int v;
    if (lru_get(&c, 1, &v)) {
        printf("get 1 = %d (теперь 1 свежий)\n", v);
    }

    lru_put(&c, 3, 300); /* вытеснит ключ 2 — он самый старый */

    if (!lru_get(&c, 2, &v)) {
        printf("get 2 = промах (вытеснен)\n");
    }
    if (lru_get(&c, 1, &v)) {
        printf("get 1 = %d\n", v);
    }
    if (lru_get(&c, 3, &v)) {
        printf("get 3 = %d\n", v);
    }

    lru_free(&c);
    return 0;
}
