#ifndef MYLIB_H
#define MYLIB_H

#include <stddef.h>

typedef struct Set {
    unsigned n; // размер исходного множества {0..n-1}
    unsigned char *digits; // digits[i] == 1, если i принадлежит подмножеству, иначе 0
} set_t;

typedef struct SetIterator {
    set_t value; // текущее подмножество
    unsigned char *state; // внутреннее число в двоичной СС: state[i] — бит i
    int finished; // 0 — есть ещё значения; 1 — закончились
} iterator_t;

// Инициализация итератора для {0,...,n-1}. Стартовое значение — пустое {}
void iterator_init(iterator_t *it, unsigned n);

// Указатель на текущее подмножество (валидно до следующего iterator_next)
const set_t *iterator_value(const iterator_t *it);

// Есть ли ещё значения (текущее включительно): 1/0
int iterator_has_next(const iterator_t *it);

// Переход к следующему подмножеству (вызовать только если has_next(it))
void iterator_next(iterator_t *it);

// Освобождение внутренних буферов (сам it не освобождается)
void iterator_destroy(iterator_t *it);

void print_set(const set_t *s);

#endif