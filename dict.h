#ifndef DICT_H
#define DICT_H

#include <stddef.h>

#define DICT_CAPACITY 128

typedef struct Entry {
    char        *key;    /* heap-allocated via strdup */
    char        *value;  /* heap-allocated via strdup */
    struct Entry *next;  /* chaining for collisions   */
} Entry;

typedef struct {
    Entry  *buckets[DICT_CAPACITY];
    size_t  size;
} Dict;

Dict  *dict_new(void);
void   dict_set(Dict *d, const char *key, const char *value);
char  *dict_get(const Dict *d, const char *key);
int    dict_delete(Dict *d, const char *key);
void   dict_free(Dict *d);
void   dict_print(const Dict *d);

#endif /* DICT_H */