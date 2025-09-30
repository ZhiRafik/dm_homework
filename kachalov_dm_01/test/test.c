#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/mylib.h"

// Тест: n = 0 -> ровно одно подмножество: {}
static void test_iterator_zero(void) {
    iterator_t *it = malloc(sizeof *it);
    assert(it != NULL);

    iterator_init(it, 0);

    assert(iterator_has_next(it)); // одно значение есть
    iterator_next(it); // текущее множество = {}
    const set_t *s = iterator_value(it);
    assert(s->n == 0);
    assert(s->digits == NULL);

    assert(!iterator_has_next(it)); // больше нет

    iterator_destroy(it);
    free(it);
}

// Тест: init корректно заполняет поля (на примере n = 5)
static void test_iterator_init_basic(void) {
    iterator_t *it = malloc(sizeof *it);
    assert(it != NULL);

    iterator_init(it, 5);

    const set_t *s = iterator_value(it);
    assert(s->n == 5);
    assert(s->digits != NULL);
    for (unsigned i = 0; i < s->n; i++) {
        assert(s->digits[i] == 0); // пустое множество
        assert(it->state[i]  == 0); // и внутреннее состояние тоже нули
    }
    assert(iterator_has_next(it)); // перечисление ещё не закончено

    iterator_destroy(it);
    free(it);
}

// Тест: перебор для n = 2 даёт маски 0,1,2,3 (в порядке бинарного счёта)
static void test_iterator_enumeration_n2(void) {
    iterator_t *it = malloc(sizeof *it);
    assert(it != NULL);

    unsigned n = 2;
    iterator_init(it, n);

    unsigned expected = 0;
    unsigned count = 0;

    while (iterator_has_next(it)) {
        iterator_next(it);
        const set_t *s = iterator_value(it);
        assert(s->n == n);

        unsigned mask = 0;
        for (unsigned i = 0; i < n; i++) {
            if (s->digits[i]) mask |= (1u << i);
        }
        assert(mask == expected);
        expected++;
        count++;
    }
    assert(count == (1u << n));

    iterator_destroy(it);
    free(it);
}

// Тест: общее количество подмножеств = 2^n (на примере n = 3)
static void test_iterator_count_n3(void) {
    iterator_t *it = malloc(sizeof *it);
    assert(it != NULL);

    unsigned n = 3;
    iterator_init(it, n);

    unsigned count = 0;
    while (iterator_has_next(it)) {
        iterator_next(it);
        (void)iterator_value(it); // доступ к текущему значению
        count++;
    }
    assert(count == (1u << n));

    iterator_destroy(it);
    free(it);
}

// Тест: большое n — проверяем первые 4 подмножества на n = 1000
static void test_iterator_large_n_prefix(void) {
    iterator_t *it = malloc(sizeof *it);
    assert(it != NULL);

    unsigned n = 1000;
    iterator_init(it, n);

    assert(iterator_has_next(it));
    iterator_next(it); // {}
    const set_t *s0 = iterator_value(it);
    for (unsigned i = 0; i < 8; i++) {
        assert(s0->digits[i] == 0);
    }

    assert(iterator_has_next(it));
    iterator_next(it); // {0}
    const set_t *s1 = iterator_value(it);
    assert(s1->digits[0] == 1);
    for (unsigned i = 1; i < 8; i++) {
        assert(s1->digits[i] == 0);
    }

    assert(iterator_has_next(it));
    iterator_next(it); // {1}
    const set_t *s2 = iterator_value(it);
    assert(s2->digits[0] == 0 && s2->digits[1] == 1);

    assert(iterator_has_next(it));
    iterator_next(it); // {0,1}
    const set_t *s3 = iterator_value(it);
    assert(s3->digits[0] == 1 && s3->digits[1] == 1);

    iterator_destroy(it);
    free(it);
}

// Тест: защита iterator_next после завершения перечисления (n = 1)
static void test_iterator_next_after_finish(void) {
    iterator_t *it = malloc(sizeof *it);
    assert(it != NULL);

    iterator_init(it, 1);

    assert(iterator_has_next(it)); // {}
    iterator_next(it);
    const set_t *s0 = iterator_value(it);
    assert(s0->n == 1 && s0->digits[0] == 0);

    assert(iterator_has_next(it)); // {0}
    iterator_next(it);
    const set_t *s1 = iterator_value(it);
    assert(s1->digits[0] == 1);

    assert(!iterator_has_next(it)); // дальше значений нет

    // Дополнительные вызовы next не должны падать и менять состояние
    iterator_next(it);
    iterator_next(it);
    const set_t *s2 = iterator_value(it);
    assert(s2->digits[0] == 1); // без изменений

    iterator_destroy(it);
    free(it);
}

int main(void) {
    test_iterator_zero();
    test_iterator_init_basic();
    test_iterator_enumeration_n2();
    test_iterator_count_n3();
    test_iterator_large_n_prefix();
    test_iterator_next_after_finish();
    
    printf("Все тесты прошли!\n");
    return 0;
}