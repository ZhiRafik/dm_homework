#ifndef MYLIB_H
#define MYLIB_H

#include <stddef.h>

// Пара целых чисел (a, b)
typedef struct Pair {
    int first;
    int second;
} Pair;

// Отношение: динамический массив пар
typedef struct Relation {
    Pair *pairs; 
    size_t size;
} Relation;

// Инициализация отношения
void initRelation(Relation *r);

/* Печать отношения в формате:
 *   {(a b), (c d), ...}
 * Корректно обрабатывает пустое множество */
void printRelation(Relation *r);

// Композиция бинарных отношений:
Relation *join_binary(Relation *r, Relation *s);

#endif