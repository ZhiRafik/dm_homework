#include <stdio.h>
#include <stdlib.h>
#include "../include/mylib.h"


int main(void) {
    unsigned n, m;
    printf("Введите два числа: n и количество целочисленных троек подмножества Х\n");
    if (scanf("%u %u", &n, &m) != 2) {
        printf("ошибка ввода: ожидалось 'n m'\n");
        return 1;
    }

    clause_t *clauses = (clause_t*)malloc(m * (unsigned)sizeof(clause_t));
    if (!clauses) {
        printf("Ошибка выделения памяти\n");
        return 1;
    }

    printf("Построчно ввведите по три целочисленных элемента\n");
    for (unsigned t = 0; t < m; t++) {
        int a, b, c;
        if (scanf("%d %d %d", &a, &b, &c) != 3) {
            printf("Ошибка ввода: ожидалось по 3 элемента на строку %u\n", t + 1);
            free(clauses);
            return 1;
        }
        clauses[t].i = a;
        clauses[t].j = b;
        clauses[t].k = c;
    }

    int sat = is_sat(clauses, m, n);
    if (sat) {
        printf("Кортеж существует\n");
    } else {
        printf("Такого кортежа не существует\n");
    }

    free(clauses);
    return 0;
}