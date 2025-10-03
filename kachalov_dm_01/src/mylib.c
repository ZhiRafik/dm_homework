#include "../include/mylib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// бинарный инкремент массива digits длиной n; возвращает 1, если удалось (без переполнения)
static int digits_increment(unsigned char *digits, unsigned n) {
    for (unsigned i = 0; i < n; i++) {
        if (digits[i] == 0) { digits[i] = 1; return 1; }
        digits[i] = 0;
    }
    return 0; // переполнение: достигнут конец перечисления
}

void iterator_init(iterator_t *it, unsigned n) {
    it->value.n = n;
    it->value.digits = n ? (unsigned char*)calloc(n, 1) : NULL; // с помощью calloc сразу зануляем выделенную память
    it->state = n ? (unsigned char*)calloc(n, 1) : NULL;
    it->finished = 0;
}

const set_t *iterator_value(const iterator_t *it) {
    return &it->value;
}

int iterator_has_next(const iterator_t *it) {
    return it->finished ? 0 : 1;
}

void iterator_next(iterator_t *it) {
    if (it->finished) return;
    if (it->value.n && it->value.digits && it->state) {
        memcpy(it->value.digits, it->state, it->value.n);
    }
    if (!digits_increment(it->state, it->value.n)) {
        it->finished = 1;
    }
}

void iterator_destroy(iterator_t *it) {
    free(it->value.digits);
    free(it->state);
    it->value.digits = NULL;
    it->state = NULL;
    it->value.n = 0;
    it->finished = 1;
}

void print_set(const set_t *s) {
    printf("{");
    int first = 1;
    for (unsigned i = 0; i < s->n; i++) {
        if (s->digits && s->digits[i]) {
            if (!first) printf(", ");
            printf("%u", i);
            first = 0;
        }
    }
    printf("}");
}