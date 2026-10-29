# dictionary-in-C

A hash-table dictionary implemented in C with chaining for collision resolution.

## The memory leak bug (and how it was found)

While stress-testing with 10,000 insert/delete cycles, RSS kept climbing. Running Valgrind revealed the root cause:

```
LEAK SUMMARY:
   definitely lost: 48,000 bytes in 10,000 blocks
```

Every call to `dict_delete()` was freeing the `Entry` node but **not** the `key` and `value` strings that were heap-allocated by `strdup()` when the entry was inserted.

### Buggy code

```c
int dict_delete(Dict *d, const char *key) {
    ...
    *pp = e->next;
    free(e);        /* node freed, but e->key and e->value are leaked */
    d->size--;
    return 1;
}
```

### Fix

```c
int dict_delete(Dict *d, const char *key) {
    ...
    *pp = e->next;
    free(e->key);    /* free the strdup'd key   */
    free(e->value);  /* free the strdup'd value */
    free(e);
    d->size--;
    return 1;
}
```

After the fix, Valgrind reports:

```
All heap blocks were freed -- no leaks are possible
```

## Build and run

```bash
make
./dict_demo

# Check for leaks
make valgrind
```

## Structure

| File | Purpose |
|------|---------|
| `dict.h` | Public API and type definitions |
| `dict.c` | Hash table implementation |
| `main.c` | Demo / manual test |
| `Makefile` | Build + Valgrind target |