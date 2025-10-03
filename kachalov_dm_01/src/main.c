#include <stdio.h>
#include "../include/mylib.h"

static void run_iterator(unsigned n) {
    iterator_t it;
    iterator_init(&it, n);

    printf("{");
    int first = 1;
    while (iterator_has_next(&it)) {
        iterator_next(&it);
        if (!first) printf(", ");
        print_set(iterator_value(&it));
        first = 0;
    }
    printf("}\n");

    iterator_destroy(&it);
}

int main(void) {
    unsigned n;
    printf("Введите n (0..%u): ", (unsigned)~0u); // (unsigned)~0u (not 0) - все единицы для unsigned
    if (scanf("%u", &n) != 1) {
        printf("Некорректный ввод\n");
        return 1;
    }
    run_iterator(n);
    return 0;
}