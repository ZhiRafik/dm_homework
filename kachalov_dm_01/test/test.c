#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/mylib.h"

/* Тест: n = 0 -> ровно одно подмножество: {} (mask = 0) */
static void test_iterator_zero(void) {
    iterator_t *it = malloc(sizeof *it);
    assert(it != NULL);

    iterator_init(it, 0);

    assert(it->value.n == 0);
    assert(it->limit == 1);
    assert(it->counter == 0);

    assert(iterator_has_next(it)); // одно значение есть
    iterator_next(it); // текущее множество = {}
    const set_t *s = iterator_value(it);
    assert(s->n == 0);
    assert(s->mask == 0);

    assert(!iterator_has_next(it)); // больше нет

    iterator_destroy(it);
}

/* Тест: init корректно заполняет поля (на примере n = 5) */
static void test_iterator_init_basic(void) {
    iterator_t *it = malloc(sizeof *it);
    assert(it != NULL);

    iterator_init(it, 5);

    assert(it->value.n == 5);
    assert(it->value.mask == 0);
    assert(it->counter == 0);
    assert(it->limit == (1ULL << 5));

    iterator_destroy(it);
}

/* Тест: перебор для n = 2 даёт маски 0,1,2,3 */
static void test_iterator_enumeration_n2(void) {
    iterator_t *it = malloc(sizeof *it);
    assert(it != NULL);

    unsigned n = 2;
    iterator_init(it, n);

    unsigned long long expected = 0;
    unsigned count = 0;

    while (iterator_has_next(it)) {
        iterator_next(it);
        const set_t *s = iterator_value(it);
        assert(s->n == n);
        assert(s->mask == expected);
        expected++;
        count++;
    }
    assert(count == (1U << n));

    iterator_destroy(it);
}

/* Тест: общее количество подмножеств = 2^n (на примере n = 3) */
static void test_iterator_count_n3(void) {
    iterator_t *it = malloc(sizeof *it);
    assert(it != NULL);

    unsigned n = 3;
    iterator_init(it, n);

    unsigned count = 0;
    while (iterator_has_next(it)) {
        iterator_next(it);
        (void)iterator_value(it); // просто доступ к текущему значению
        count++;
    }
    assert(count == (1U << n));

    iterator_destroy(it);
}

int main(void) {
    test_iterator_zero();
    test_iterator_init_basic();
    test_iterator_enumeration_n2();
    test_iterator_count_n3();

    printf("Все тесты прошли!\n");
    return 0;
}