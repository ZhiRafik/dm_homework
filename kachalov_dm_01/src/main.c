#include <stdio.h>
#include <stdlib.h>
#include "../include/mylib.h"

void run_iterator(unsigned n) {
    iterator_t *it = malloc(sizeof(iterator_t));
    iterator_init(it, n);
    printf("{");
    int first = 1;
    while (iterator_has_next(it)) {
        iterator_next(it); // подготовили текущее множество
        if (!first) {
            printf(", ");    
        }      
        print_set(iterator_value(it));
        first = 0;
    }
    printf("}\n");
    iterator_destroy(it);
}

int main(void) {
    unsigned n;
    printf("Введите n от 0 до 64: ");
    scanf("%u", &n);
    while (n > 63) {
        printf("Введите n от 0 до 64 пожалуйста:\n");
        scanf("%u", &n);
    }
    run_iterator(n);
    return 0;
}