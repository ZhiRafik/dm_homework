#include <stdio.h>
#include "../include/mylib.h"

int main(void) {
    iterator_t iterator;
    unsigned n;
    printf("Введите n: \n");
    scanf("%u", &n);

    iterator_init(&iterator, n);

    while(iterator_has_next(&iterator)) {
        const permutation_t *p = iterator_value(&iterator);
        print_permutation(p);
        iterator_next(&iterator);
    }

    iterator_destroy(&iterator);
    return 0;
}