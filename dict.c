#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* djb2 hash */
static unsigned long hash(const char *key) {
    unsigned long h = 5381;
    int c;
    while ((c = (unsigned char)*key++))
        h = ((h << 5) + h) + c;
    return h % DICT_CAPACITY;
}

Dict *dict_new(void) {
    Dict *d = calloc(1, sizeof(Dict));
    if (!d) { perror("calloc"); exit(1); }
    return d;
}

void dict_set(Dict *d, const char *key, const char *value) {
    unsigned long idx = hash(key);
    Entry *e = d->buckets[idx];

    /* update existing key */
    while (e) {
        if (strcmp(e->key, key) == 0) {
            free(e->value);                 /* free old value before replacing */
            e->value = strdup(value);
            return;
        }
        e = e->next;
    }

    /* insert new entry */
    Entry *ne = malloc(sizeof(Entry));
    if (!ne) { perror("malloc"); exit(1); }
    ne->key   = strdup(key);               /* owned by the entry */
    ne->value = strdup(value);
    ne->next  = d->buckets[idx];
    d->buckets[idx] = ne;
    d->size++;
}

char *dict_get(const Dict *d, const char *key) {
    unsigned long idx = hash(key);
    Entry *e = d->buckets[idx];
    while (e) {
        if (strcmp(e->key, key) == 0) return e->value;
        e = e->next;
    }
    return NULL;
}

/*
 * BUG (original):  free(e) without free(e->key) or free(e->value).
 * Valgrind reported "definitely lost" blocks — one pair per deleted entry.
 *
 * FIX: free key and value BEFORE freeing the node itself.
 */
int dict_delete(Dict *d, const char *key) {
    unsigned long idx = hash(key);
    Entry **pp = &d->buckets[idx];

    while (*pp) {
        Entry *e = *pp;
        if (strcmp(e->key, key) == 0) {
            *pp = e->next;
            free(e->key);    /* FIX: was missing — caused memory leak */
            free(e->value);  /* FIX: was missing — caused memory leak */
            free(e);
            d->size--;
            return 1;
        }
        pp = &e->next;
    }
    return 0; /* not found */
}

void dict_free(Dict *d) {
    for (int i = 0; i < DICT_CAPACITY; i++) {
        Entry *e = d->buckets[i];
        while (e) {
            Entry *next = e->next;
            free(e->key);
            free(e->value);
            free(e);
            e = next;
        }
    }
    free(d);
}

void dict_print(const Dict *d) {
    printf("Dict (%zu entries):\n", d->size);
    for (int i = 0; i < DICT_CAPACITY; i++) {
        Entry *e = d->buckets[i];
        while (e) {
            printf("  [%s] => %s\n", e->key, e->value);
            e = e->next;
        }
    }
}