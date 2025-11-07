#include <stdio.h>
#include "../include/mylib.h"

int main(void) {
    unsigned n;
    
    printf("Введите n: ");
    if (scanf("%u", &n) != 1) {
        return 1;
    }

    if (n < 0) {
        printf("Неожиданный ввод: отрицательное число. Невозможно построить ПСП.");
        return 1;
    }

    iterator_t it;
    iterator_init(&it, n);

    if (!iterator_value(&it) || !iterator_value(&it)->data) {
        printf("Нет данных\n");
        return 0;
    }

    // печатаем первую и все последующие последовательности
    print_current(&it);
    while (iterator_has_next(&it)) {
        iterator_next(&it);
        print_current(&it);
    }

    iterator_destroy(&it);
    return 0;
}