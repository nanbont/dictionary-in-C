#include "dict.h"
#include <stdio.h>

int main(void) {
    Dict *d = dict_new();

    dict_set(d, "name",    "Nanbon");
    dict_set(d, "country", "Ethiopia");
    dict_set(d, "city",    "Messina");
    dict_set(d, "role",    "Software Engineer");

    dict_print(d);

    printf("\nLookup 'city': %s\n", dict_get(d, "city"));

    dict_delete(d, "country");
    printf("After deleting 'country':\n");
    dict_print(d);

    /* update existing key */
    dict_set(d, "city", "Rijeka");
    printf("After updating 'city':\n");
    dict_print(d);

    dict_free(d);
    /* run with: valgrind --leak-check=full ./dict_demo */
    return 0;
}