// test/test.c
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/mylib.h"

/* вспомогательные функции для создания/освобождения Relation без ввода/вывода */
static Relation make_rel(size_t n, const Pair *src) {
    Relation r;
    r.size = n;
    if (n == 0) {
        r.pairs = NULL;
        return r;
    }
    r.pairs = (Pair*)malloc(sizeof(Pair) * n);
    assert(r.pairs != NULL);
    memcpy(r.pairs, src, sizeof(Pair) * n);
    return r;
}

static void free_rel(Relation *r) {
    if (!r) return;
    free(r->pairs);
    r->pairs = NULL;
    r->size = 0;
}

/* --- ТЕСТЫ: проверяем ТОЛЬКО состояние структуры после join_binary --- */

static void test_join_empty_empty(void) {
    Relation r = make_rel(0, NULL);
    Relation s = make_rel(0, NULL);

    Relation *res = join_binary(&r, &s);
    assert(res != NULL);
    assert(res->size == 0);

    free_rel(&r);
    free_rel(&s);
    free(res->pairs);
    free(res);
}

static void test_join_left_empty(void) {
    Relation r = make_rel(0, NULL);
    const Pair s_arr[] = { {1,2}, {3,4} };
    Relation s = make_rel(2, s_arr);

    Relation *res = join_binary(&r, &s);
    assert(res != NULL);
    assert(res->size == 0);

    free_rel(&r);
    free_rel(&s);
    free(res->pairs);
    free(res);
}

static void test_join_right_empty(void) {
    const Pair r_arr[] = { {1,2}, {3,4} };
    Relation r = make_rel(2, r_arr);
    Relation s = make_rel(0, NULL);

    Relation *res = join_binary(&r, &s);
    assert(res != NULL);
    assert(res->size == 0);

    free_rel(&r);
    free_rel(&s);
    free(res->pairs);
    free(res);
}

static void test_join_no_matches(void) {
    const Pair r_arr[] = { {1,10}, {2,20} };
    const Pair s_arr[] = { {30,100}, {40,200} };
    Relation r = make_rel(2, r_arr);
    Relation s = make_rel(2, s_arr);

    Relation *res = join_binary(&r, &s);
    assert(res != NULL);
    assert(res->size == 0);

    free_rel(&r);
    free_rel(&s);
    free(res->pairs);
    free(res);
}

static void test_join_single_match(void) {
    const Pair r_arr[] = { {7, 3}, {8, 9} };
    const Pair s_arr[] = { {1, 11}, {3, 42} };
    Relation r = make_rel(2, r_arr);
    Relation s = make_rel(2, s_arr);

    Relation *res = join_binary(&r, &s);
    assert(res != NULL);
    assert(res->size == 1);
    assert(res->pairs[0].first  == 7);
    assert(res->pairs[0].second == 42);

    free_rel(&r);
    free_rel(&s);
    free(res->pairs);
    free(res);
}

static void test_join_multiple_matches_cartesian(void) {
    /* два элемента в R с b=2 и два элемента в S с b=2 → 2*2 = 4 результата
       порядок должен соответствовать вложенным циклам i->j */
    const Pair r_arr[] = { {1,2}, {3,2}, {5,7} };
    const Pair s_arr[] = { {2,50}, {2,60}, {9,90} };
    Relation r = make_rel(3, r_arr);
    Relation s = make_rel(3, s_arr);

    Relation *res = join_binary(&r, &s);
    assert(res != NULL);
    assert(res->size == 4);

    assert(res->pairs[0].first == 1 && res->pairs[0].second == 50);
    assert(res->pairs[1].first == 1 && res->pairs[1].second == 60);
    assert(res->pairs[2].first == 3 && res->pairs[2].second == 50);
    assert(res->pairs[3].first == 3 && res->pairs[3].second == 60);

    free_rel(&r);
    free_rel(&s);
    free(res->pairs);
    free(res);
}

static void test_join_duplicates_in_inputs_unacceptable(void) {
    /* в отношении же не может быть дубликатов - как-то не подумал:/ */
    const Pair r_arr[] = { {1,2}, {1,5}, {4, 8} };          /* различие элементы в отношениях  */
    const Pair s_arr[] = { {2,7}, {5,7}, {8, 13} };          /* но которые ведут "к дубликатам"*/
    Relation r = make_rel(3, r_arr);
    Relation s = make_rel(3, s_arr);

    Relation *res = join_binary(&r, &s);            /* в результате тогда должно быть {{1, 7}, {4, 13}}*/
    assert(res != NULL);
    assert(res->size == 2);
    
    assert(res->pairs[0].first == 1);
    assert(res->pairs[0].second == 7);
    assert(res->pairs[1].first == 4);
    assert(res->pairs[1].second == 13);

    free_rel(&r);
    free_rel(&s);
    free(res->pairs);
    free(res);
}

int main(void) {
    test_join_empty_empty();
    test_join_left_empty();
    test_join_right_empty();
    test_join_no_matches();
    test_join_single_match();
    test_join_multiple_matches_cartesian();
    test_join_duplicates_in_inputs_unacceptable();
    printf("Все тесты прошли успешно!\n");

    return 0;
}