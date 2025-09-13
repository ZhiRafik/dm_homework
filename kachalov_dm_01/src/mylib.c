#include "../include/mylib.h"
#include <stdlib.h>
#include <stdio.h>


void iterator_init(iterator_t *it , unsigned n) {
    it->value.n = n;
    it->value.mask = 0;
    it->counter = 0;
    it->limit = 1ULL << n; // всего 2^n подмножеств
}

const set_t *iterator_value(const iterator_t *it) {
    return &(it->value); 
}

int iterator_has_next(const iterator_t *it) {
    return it->counter < it->limit; // текущий элемент меньше максимального
}

void iterator_next(iterator_t *it) { // только при has_next
    it->value.mask = it->counter;
    it->counter++;
}

void iterator_destroy(iterator_t *it) {
    free(it);
}

void print_set(const set_t *s) {
    printf("{");
    int first = 1;
    for (unsigned i = 0; i < s->n; i++) {
        if ((s->mask >> i) & 1ULL) {
            if (!first) {
                printf(", ");
            }
            printf("%u", i);
            first = 0;
        }
    }
    printf("}");
}