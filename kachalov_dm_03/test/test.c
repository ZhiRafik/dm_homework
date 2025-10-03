#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/mylib.h"

// Вспомогалки всякие
static unsigned fact(unsigned n) {
    unsigned r = 1;
    for (unsigned i = 2; i <= n; ++i) {
        r *= i;
    }
    return r;
}

static void assert_perm_valid(const permutation_t *p) {
    assert(p);
    unsigned n = p->n;
    // проверяем, что внутри ровно числа 0..n-1 без повторов 
    unsigned char seen_buf[256];
    unsigned char *seen = seen_buf;
    int need_free = 0;

    if (n > sizeof(seen_buf)) {
        seen = (unsigned char*)calloc(n, 1);
        need_free = 1;
    } else {
        for (unsigned i = 0; i < n; ++i) {
            seen[i] = 0;
        }
    }

    for (unsigned i = 0; i < n; ++i) {
        unsigned v = p->digits[i];
        assert(v < n);
        assert(seen[v] == 0);
        seen[v] = 1;
    }

    for (unsigned i = 0; i < n; ++i) {
        assert(seen[i] == 1);
    }

    if (need_free) {
        free(seen);
    }
}

// Сами тесты
static void test_null_safety_calls(void) {
    // Должны просто не падать и ничего не делать 
    iterator_init(NULL, 3);
    iterator_next(NULL);
    iterator_destroy(NULL);

    assert(iterator_value(NULL) == NULL);
    assert(iterator_has_next(NULL) == 0);

    print_permutation(NULL);
}

static void test_n0(void) {
    iterator_t it;
    iterator_init(&it, 0);

    // После init есть "текущая" перестановка (пустая)
    assert(iterator_has_next(&it) == 1);
    const permutation_t *p0 = iterator_value(&it);
    assert(p0 == &it.value);
    assert(p0->n == 0);
    assert(p0->digits != NULL);

    // Вызов next для n <= 1 должен сразу завершить итерацию 
    iterator_next(&it);
    assert(iterator_has_next(&it) == 0);
    assert(it.finished == 1);

    iterator_destroy(&it);
    assert(it.value.digits == NULL);
    assert(it.state == NULL);
    assert(it.value.n == 0);
    assert(it.finished == 1);
}

static void test_n1(void) {
    iterator_t it;
    iterator_init(&it, 1);

    // Первая перестановка существует
    assert(iterator_has_next(&it) == 1);
    const permutation_t *p = iterator_value(&it);
    assert(p == &it.value);
    assert(p->n == 1);
    assert(p->digits[0] == 0);
    assert_perm_valid(p);

    iterator_next(&it);
    assert(iterator_has_next(&it) == 0);
    assert(it.finished == 1);

    iterator_destroy(&it);
}

static void test_n2(void) {
    iterator_t it;
    iterator_init(&it, 2);

    unsigned cnt = 0;
    while (iterator_has_next(&it)) {
        const permutation_t *p = iterator_value(&it);
        assert(p == &it.value);
        assert_perm_valid(p);
        cnt++;
        iterator_next(&it);
    }
    assert(cnt == fact(2));
    assert(it.finished == 1);

    iterator_destroy(&it);
}

static void test_n3(void) {
    iterator_t it;
    iterator_init(&it, 3);

    // Первая — тождественная
    const permutation_t *p0 = iterator_value(&it);
    assert(p0->n == 3);
    assert(p0->digits[0] == 0 && p0->digits[1] == 1 && p0->digits[2] == 2);

    unsigned cnt = 0;
    while (iterator_has_next(&it)) {
        const permutation_t *p = iterator_value(&it);
        assert_perm_valid(p);
        cnt++;
        iterator_next(&it);
    }
    // Должны получить 3! = 6 перестановок
    assert(cnt == 6);
    assert(it.finished == 1);

    // После завершения дополнительные next не должны ломать состояние 
    iterator_next(&it);
    assert(it.finished == 1);
    assert(iterator_has_next(&it) == 0);

    iterator_destroy(&it);
}

static void test_big_n8(void) {
    iterator_t it;
    unsigned n = 8; // 40320 перестановок
    iterator_init(&it, n);

    unsigned cnt = 0;
    while (iterator_has_next(&it)) {
        const permutation_t *p = iterator_value(&it);
        assert_perm_valid(p); // проверяем корректность каждой перестановки
        cnt++;
        iterator_next(&it);
    }
    assert(cnt == fact(n));
    assert(it.finished == 1);

    iterator_destroy(&it);
}

/* Дополнительно покроем print_permutation нормальным вызовом,
   чтобы пройти тело цикла печати */
static void test_print_permutation_body(void) {
    iterator_t it;
    iterator_init(&it, 3);
    const permutation_t *p = iterator_value(&it);
    print_permutation(p);
    iterator_destroy(&it);
}

int main(void) {
    test_null_safety_calls();
    test_n0();
    test_n1();
    test_n2();
    test_n3();
    test_big_n8();
    test_print_permutation_body();

    printf("Все тесты прошли успешно!");
    return 0;
}