#ifndef MYLIB_H
#define MYLIB_H

#include <stdint.h>
#include <stddef.h>

typedef struct Set {
    unsigned n; // максимальный элемент
    uint64_t mask; // маска добавленных
} set_t;

typedef struct SetIterator {
    set_t value;
    uint64_t counter;
    uint64_t limit;
} iterator_t;

/* Инициализация итератора для множества {0,...,n-1}. 
Начальное значение — пустое множество. */
void iterator_init(iterator_t *it , unsigned n);

/* Указатель на текущее подмножество. 
Действителен до следующего вызова iterator_next(). */
const set_t *iterator_value(const iterator_t *it);

/* Есть ли ещё значения (текущее включительно). Возвращает 1/0. */
int iterator_has_next(const iterator_t *it);

/* Переход к следующему подмножеству. */
void iterator_next(iterator_t *it);

/* Удаление итератора */
void iterator_destroy(iterator_t *it);

/* Печать множества */
void print_set(const set_t *s);

#endif