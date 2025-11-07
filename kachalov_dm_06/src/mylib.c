#include "../include/mylib.h"
#include <stdlib.h>
#include <stdio.h>

// создать первую правильную скобочную последовательность "(((...)))"
static char *first_psp(unsigned n) {
    char *s = (char *) malloc(2 * n + 1);
    if (!s) {
        return NULL;
    }
    for (unsigned i = 0; i < n; i++) {
        s[i] = '(';
    }
    for (unsigned i = n; i < 2 * n; i++) {
        s[i] = ')';
    }
    s[2 * n] = '\0';
    
    return s;
}

// можно ли построить следующее значение из текущей строки
static int will_find_next(const char *s) {
    unsigned len = 0;
    while (s[len] != '\0') {
        len++;
    }

    int balance = 0; // число лишних ')' на уже просмотренном суффиксе
    for (int i = (int)len - 1; i >= 0; i--) {
        if (s[i] == ')') {
            balance++; // встретили ')', на суффиксе стало ещё на одну больше
        } else {
            balance--; // встретили '('
            if (balance > 0) { // справа есть хотя бы одна лишняя ')'
                return 1; // эту '(' можно превратить в ')' и построить следующий хвост
            }
        }
    }
    return 0;  // не нашли позицию — текущее значение последнее
}

/*
Алгоритм генерации следующей псп:
1. идём справа налево и ищем '(', после которого стоит хотя бы одна ')'
2. заменяем эту '(' на ')'
3. после неё делаем минимально возможный правильный хвост — то есть
ставим столько '(', сколько нужно, а затем все оставшиеся ')'
*/
static int next_psp(char *s) {
    unsigned len = 0;
    while (s[len] != '\0') {
        len++;
    }

    int balance = 0;
    int i = (int)len - 1;

    // ищем справа позицию i, где стоит '(' и на суффиксе уже больше закрывающих чем открывающих
    for (; i >= 0; i--) {
        if (s[i] == ')') {
            balance++;
        } else {
            balance--;
            if (balance > 0) {
                break; // нашли подходящую '('
            }
        }
    }
    if (i < 0) {
        return 0; // больше последовательностей нет
    }

    // в префиксе 0..i теперь символ s[i] будет ')', поэтому считаем только '(' на диапазоне [0, i)
    unsigned opens_prefix = 0;
    for (unsigned j = 0; j < (unsigned)i; ++j) {
        if (s[j] == '(') {
            opens_prefix++;
        }
    }
    s[i] = ')'; // заменяем найденную '(' на ')'

    // вычисляем сколько ещё нужно открыть и закрыть
    unsigned total = len / 2;
    unsigned remaining_open  = total - opens_prefix; // все '(' которые ещё можно поставить

    // минимальный корректный хвост: сначала все оставшиеся '(' затем все оставшиеся ')'
    unsigned pos = (unsigned)i + 1;
    while (remaining_open > 0) {
        s[pos++] = '(';
        remaining_open--;
    }
    while (pos < len) {
        s[pos++] = ')';
    }

    s[len] = '\0';
    return 1;
}

// инициализация итератора для заданного n, n >= 0
void iterator_init(iterator_t *it, unsigned n) {
    if (!it) {
        return;
    }

    State *st = (State *) malloc(sizeof(State));
    if (!st) {
        it->state = NULL;
        it->value.data = NULL;
        it->value.n = n;
        return;
    }

    st->n = n;
    st->done = 0;

    it->state = st;
    it->value.n = n;
    it->value.data = first_psp(n);
    if (!it->value.data) {
        st->done = 1; // нет даже первого значения
    } else {
        st->done = will_find_next(it->value.data) ? 0 : 1;
    }
}

// указатель на текущее значение, валидно до destroy
const cbs_t *iterator_value(const iterator_t *it) {
    return it ? &it->value : NULL;
}

// возвращает 1 если можно построить следующее значение иначе 0
int iterator_has_next(const iterator_t *it) {
    if (!it || !it->value.data) {
        return 0;
    }

    return will_find_next(it->value.data);
}

// переход к следующему значению
void iterator_next(iterator_t *it) {
    if (!it || !it->state || !it->value.data) {
        return;
    }

    // если следующего нет, просто помечаем конец
    if (!next_psp(it->value.data)) {
        ((State *)it->state)->done = 1;
        return;
    }

    // после успешного перехода пересчитываем флаг конца
    ((State *)it->state)->done = will_find_next(it->value.data) ? 0 : 1;
}

// освобождение всех ресурсов
void iterator_destroy(iterator_t *it) {
    if (!it) {
        return;
    }

    State *st = (State *) it->state;
    free(it->value.data);
    free(st);

    it->state = NULL;
    it->value.data = NULL;
    it->value.n = 0;
}

void print_current(const iterator_t *it) {
    if (!it || !it->value.data) {
        return;
    }

    printf("%s\n", it->value.data);
}