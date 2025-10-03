#ifndef MYLIB_H
#define MYLIB_H

typedef struct Permutation {
    unsigned n; // размер исходного множества {0..n-1}
    unsigned char *digits;
} permutation_t;

typedef struct SetPermutation {
    permutation_t value; // текущая пермутация
    unsigned char *state;
    int finished;
} iterator_t;

// Инициализация итератора для {0,...,n-1}. Стартовое значение — по порядку
void iterator_init(iterator_t *it, unsigned n);

// Указатель на текущую пермутацию
const permutation_t *iterator_value(const iterator_t *it);

// Есть ли ещё пермутации: 1/0
int iterator_has_next(const iterator_t *it);

// Переход к следующей пермутации (вызовать только если has_next(it))
void iterator_next(iterator_t *it);

// Освобождение внутренних буферов (сам it не освобождается)
void iterator_destroy(iterator_t *it);

void print_permutation(const permutation_t *s);

#endif 