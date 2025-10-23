#ifndef MYLIB_H
#define MYLIB_H

// правильная скобочная последовательность длины 2*n
// data — строка из '(' и ')'
// память под data управляется итератором
typedef struct CBS {
    char *data; // текущее значение псп
    unsigned n; // число пар скобок
} cbs_t;

// внутреннее состояние итератора
typedef struct State {
    unsigned n; // число пар скобок
    int done;  // текущее значение — последнее, следующего нет
} State;

// итератор по всем псп длины 2*n
// state — внутреннее состояние итерации
typedef struct CbsIterator {
    cbs_t value; // текущее значение
    State *state; // внутреннее состояние
} iterator_t;

// инициализация итератора для заданного n, n >= 0
void iterator_init(iterator_t *i, unsigned n);

// указатель на текущее значение, валидно до destroy
const cbs_t *iterator_value(const iterator_t *i);

// возвращает 1 если существует следующее значение иначе 0
int iterator_has_next(const iterator_t *i);

// переход к следующему значению, вызывать только если has_next(i) != 0
void iterator_next(iterator_t *i);

// освобождение всех ресурсов
void iterator_destroy(iterator_t *i);

// печатаем
void print_current(const iterator_t *it);

#endif