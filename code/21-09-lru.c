#include <stdio.h>
#include <stdlib.h>

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
    int capacity;         /* сколько записей помещается */
    int size;             /* сколько записей сейчас */
    Node *head;           /* самый свежий */
    Node *tail;           /* самый старый */
    Node **buckets;       /* массив корзин: их столько же, сколько мест */
    unsigned nbuckets;
} LRU;

/* 1 — готово, 0 — неверная вместимость или нет памяти. */
int lru_init(LRU *c, int capacity)
{
    if (capacity < 1) {
        return 0;
    }
    c->buckets = malloc((size_t)capacity * sizeof(Node *));
    if (c->buckets == NULL) {
        return 0;
    }
    c->nbuckets = (unsigned)capacity;
    for (unsigned i = 0; i < c->nbuckets; i++) {
        c->buckets[i] = NULL;
    }
    c->capacity = capacity;
    c->size = 0;
    c->head = NULL;
    c->tail = NULL;
    return 1;
}

static unsigned bucket_of(const LRU *c, int key)
{
    return (unsigned)key % c->nbuckets;
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

static Node *hash_find(const LRU *c, int key)
{
    Node *e = c->buckets[bucket_of(c, key)];
    for (; e != NULL; e = e->hnext) {
        if (e->key == key) {
            return e;
        }
    }
    return NULL;
}

/* убрать узел из его хеш-цепочки */
static void hash_remove(LRU *c, Node *n)
{
    Node **pp = &c->buckets[bucket_of(c, n->key)];
    while (*pp != n) {
        pp = &(*pp)->hnext;
    }
    *pp = n->hnext;
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

/* put: 1 — записано, 0 — не хватило памяти (кэш тогда не меняется). */
int lru_put(LRU *c, int key, int value)
{
    Node *n = hash_find(c, key);
    if (n != NULL) {                 /* ключ уже есть — обновим и освежим */
        n->value = value;
        list_unlink(c, n);
        list_push_front(c, n);
        return 1;
    }

    n = malloc(sizeof(Node));        /* сначала память, потом вытеснение */
    if (n == NULL) {
        return 0;
    }

    if (c->size == c->capacity) {    /* кэш полон — вытесняем хвост */
        Node *old = c->tail;
        list_unlink(c, old);
        hash_remove(c, old);
        free(old);
        c->size--;
    }

    n->key = key;
    n->value = value;
    unsigned b = bucket_of(c, key);
    n->hnext = c->buckets[b];        /* в начало хеш-цепочки */
    c->buckets[b] = n;
    list_push_front(c, n);           /* и в голову списка свежести */
    c->size++;
    return 1;
}

void lru_free(LRU *c)
{
    Node *p = c->head;
    while (p != NULL) {
        Node *nx = p->next;
        free(p);
        p = nx;
    }
    free(c->buckets);
    c->buckets = NULL;
    c->head = NULL;
    c->tail = NULL;
    c->size = 0;
}

int main(void)
{
    LRU c;
    if (!lru_init(&c, 2)) {          /* кэш всего на 2 записи */
        printf("Не хватило памяти\n");
        return 1;
    }

    int v;
    if (!lru_put(&c, 1, 100) || !lru_put(&c, 2, 200)) {
        printf("Не хватило памяти\n");
        lru_free(&c);
        return 1;
    }

    if (lru_get(&c, 1, &v)) {
        printf("get 1 = %d (теперь 1 свежий)\n", v);
    }

    if (!lru_put(&c, 3, 300)) {      /* вытеснит ключ 2 — он старейший */
        printf("Не хватило памяти\n");
        lru_free(&c);
        return 1;
    }

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
