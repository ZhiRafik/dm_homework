#include "../include/mylib.h"
#include <stdio.h>
#include <stdlib.h>

void iterator_init(iterator_t *it, unsigned n) {
    if (!it) {
        return;
    }
    it->value.n = n;
    it->value.digits = (unsigned char*)calloc(n ? n : 1, sizeof(unsigned char));
    for (unsigned i = 0; i < n; ++i) {
        it->value.digits[i] = (unsigned char)i;
    }
    it->state = (unsigned char*)calloc(n ? n : 1, sizeof(unsigned char)); 
    it->finished = 0; // первая перестановка готова
}

const permutation_t *iterator_value(const iterator_t *it) {
    return it ? &it->value : NULL;
}

int iterator_has_next(const iterator_t *it) {
    return it && !it->finished;
}

void iterator_next(iterator_t *it) {
    if (!it || it->finished) {
        return;
    }

    unsigned n = it->value.n;
    unsigned char *a = it->value.digits;
    unsigned char *c = it->state;

    if (n <= 1) {
        it->finished = 1;
        return;
    }

    // Итеративный алгоритм Хипа
    unsigned i = 0;
    while (i < n) {
        if (c[i] < i) {
            unsigned swap_with = (i % 2 == 0) ? 0u : (unsigned)c[i];
            unsigned char tmp = a[swap_with];
            a[swap_with] = a[i];
            a[i] = tmp;
            c[i] += 1;
            i = 0; // готова следующая перестановка
            return;
        } else {
            c[i] = 0;
            i++;
        }
    }

    it->finished = 1; // перестановок больше нет
}

void iterator_destroy(iterator_t *it) {
    if (!it) {
        return;
    }
    free(it->value.digits);
    free(it->state);
    it->value.digits = NULL;
    it->state = NULL;
    it->value.n = 0;
    it->finished = 1;
}

void print_permutation(const permutation_t *s) {
    if (!s) {
        return;
    }
    printf("{");
    for (unsigned i = 0; i < s->n; ++i) {
        printf("%u", (unsigned)s->digits[i]);
        if (i + 1 < s->n) {
            printf(", ");
        }
    }
    printf("}");
    printf("\n");
}