#include "../include/mylib.h"
#include <assert.h>
#include <stdio.h>

// работа функций с NULL
// граф с 0 вершинами
static void test_edge_case_null_and_zero() {
    assert(graph_vertex_count(NULL) == 0);
    assert(graph_degree(NULL, 0) == 0);
    assert(graph_has_edge(NULL, 0, 0) == 0);
    assert(graph_add_edge(NULL, 0, 0) == -1);
    graph_simplify(NULL);

    graph_t *g = graph_create(0);
    assert(g != NULL);
    assert(graph_vertex_count(g) == 0);
    graph_simplify(g);
    graph_free(g);
}

// выход за границы индексов
// петли
static void test_edge_case_invalid_and_self_loop() {
    graph_t *g = graph_create(3);
    assert(g != NULL);

    assert(graph_add_edge(g, 0, 3) == -1);
    assert(graph_add_edge(g, 5, 1) == -1);

    int r = graph_add_edge(g, 1, 1);
    assert(r == 0);
    assert(graph_degree(g, 1) == 0);
    assert(graph_has_edge(g, 1, 1) == 0);

    assert(graph_degree(g, 10) == 0);
    assert(graph_has_edge(g, 0, 10) == 0);

    graph_free(g);
}

// две вершины и одно ребро
// проверка дубликата ребра
static void test_simple_single_edge() {
    graph_t *g = graph_create(2);
    assert(g != NULL);

    assert(graph_add_edge(g, 0, 1) == 0);
    assert(graph_has_edge(g, 0, 1) == 1);
    assert(graph_has_edge(g, 1, 0) == 1);
    assert(graph_degree(g, 0) == 1);
    assert(graph_degree(g, 1) == 1);

    int r = graph_add_edge(g, 0, 1);
    assert(r == 0);
    assert(graph_degree(g, 0) == 1);
    assert(graph_degree(g, 1) == 1);

    graph_simplify(g);
    assert(graph_has_edge(g, 0, 1) == 1);
    assert(graph_degree(g, 0) == 1);
    assert(graph_degree(g, 1) == 1);

    graph_free(g);
}

// путь 0-1-2
// после упрощения должно быть только ребро 0-2
static void test_simple_path_len3() {
    graph_t *g = graph_create(3);
    assert(g != NULL);

    assert(graph_add_edge(g, 0, 1) == 0);
    assert(graph_add_edge(g, 1, 2) == 0);

    assert(graph_degree(g, 0) == 1);
    assert(graph_degree(g, 1) == 2);
    assert(graph_degree(g, 2) == 1);

    graph_simplify(g);

    assert(graph_has_edge(g, 0, 1) == 0);
    assert(graph_has_edge(g, 1, 2) == 0);
    assert(graph_has_edge(g, 0, 2) == 1);

    assert(graph_degree(g, 0) == 1);
    assert(graph_degree(g, 2) == 1);
    assert(graph_degree(g, 1) == 0);

    graph_free(g);
}

// путь 0-1-2-3-4
// после упрощения должен остаться только крайний ребро 0-4
static void test_medium_path_len5() {
    graph_t *g = graph_create(5);
    assert(g != NULL);

    assert(graph_add_edge(g, 0, 1) == 0);
    assert(graph_add_edge(g, 1, 2) == 0);
    assert(graph_add_edge(g, 2, 3) == 0);
    assert(graph_add_edge(g, 3, 4) == 0);

    graph_simplify(g);

    assert(graph_has_edge(g, 0, 4) == 1);
    assert(graph_degree(g, 0) == 1);
    assert(graph_degree(g, 4) == 1);

    assert(graph_degree(g, 1) == 0);
    assert(graph_degree(g, 2) == 0);
    assert(graph_degree(g, 3) == 0);

    assert(graph_has_edge(g, 0, 1) == 0);
    assert(graph_has_edge(g, 1, 2) == 0);
    assert(graph_has_edge(g, 2, 3) == 0);
    assert(graph_has_edge(g, 3, 4) == 0);

    graph_free(g);
}

// Средний случай 2:
// цикл 0-1-2-3-0
// все вершины степени 2
// после упрощения остаётся одно ребро между двумя вершинами
static void test_medium_cycle4() {
    graph_t *g = graph_create(4);
    assert(g != NULL);

    assert(graph_add_edge(g, 0, 1) == 0);
    assert(graph_add_edge(g, 1, 2) == 0);
    assert(graph_add_edge(g, 2, 3) == 0);
    assert(graph_add_edge(g, 3, 0) == 0);

    graph_simplify(g);

    size_t edges_count = 0;
    size_t u_last = 0;
    size_t v_last = 0;

    for (size_t u = 0; u < 4; u++) {
        for (size_t v = u + 1; v < 4; v++) {
            if (graph_has_edge(g, u, v)) {
                edges_count++;
                u_last = u;
                v_last = v;
            }
        }
    }

    assert(edges_count == 1);

    assert(graph_degree(g, u_last) == 1);
    assert(graph_degree(g, v_last) == 1);

    for (size_t v = 0; v < 4; v++) {
        if (v != u_last && v != v_last) {
            assert(graph_degree(g, v) == 0);
        }
    }

    graph_free(g);
}

// Сложный случай:
// путь 0-1-2-3-4 и треугольник 4-5-6-4
// после серии упрощений остаётся одно ребро 0-6
static void test_complex_mixed_graph() {
    graph_t *g = graph_create(7);
    assert(g != NULL);

    assert(graph_add_edge(g, 0, 1) == 0);
    assert(graph_add_edge(g, 1, 2) == 0);
    assert(graph_add_edge(g, 2, 3) == 0);
    assert(graph_add_edge(g, 3, 4) == 0);

    assert(graph_add_edge(g, 4, 5) == 0);
    assert(graph_add_edge(g, 5, 6) == 0);
    assert(graph_add_edge(g, 6, 4) == 0);

    graph_simplify(g);

    assert(graph_has_edge(g, 0, 6) == 1);
    assert(graph_degree(g, 0) == 1);
    assert(graph_degree(g, 6) == 1);

    for (size_t v = 1; v <= 5; v++) {
        assert(graph_degree(g, v) == 0);
    }

    for (size_t u = 0; u < 7; u++) {
        for (size_t v = u + 1; v < 7; v++) {
            if (u == 0 && v == 6) {
                continue;
            }
            assert(graph_has_edge(g, u, v) == 0);
        }
    }

    graph_free(g);
}

// Граф с двумя компонентами связности:
//  компонента 1: 0-1-2-3
//  компонента 2: 4-5-6
// После упрощения:
//  компонента 1: 0-3
//  компонента 2: 4-6
static void test_two_components() {
    graph_t *g = graph_create(7);
    assert(g != NULL);

    // Первая компонента: путь 0-1-2-3
    assert(graph_add_edge(g, 0, 1) == 0);
    assert(graph_add_edge(g, 1, 2) == 0);
    assert(graph_add_edge(g, 2, 3) == 0);

    // Вторая компонента: путь 4-5-6
    assert(graph_add_edge(g, 4, 5) == 0);
    assert(graph_add_edge(g, 5, 6) == 0);

    graph_simplify(g);

    // Проверяем первую компоненту: осталось ребро 0-3
    assert(graph_has_edge(g, 0, 3) == 1);
    assert(graph_degree(g, 0) == 1);
    assert(graph_degree(g, 3) == 1);
    assert(graph_degree(g, 1) == 0);
    assert(graph_degree(g, 2) == 0);

    // Проверяем вторую компоненту: осталось ребро 4-6
    assert(graph_has_edge(g, 4, 6) == 1);
    assert(graph_degree(g, 4) == 1);
    assert(graph_degree(g, 6) == 1);
    assert(graph_degree(g, 5) == 0);

    // Между компонентами рёбер быть не должно
    assert(graph_has_edge(g, 0, 4) == 0);
    assert(graph_has_edge(g, 0, 6) == 0);
    assert(graph_has_edge(g, 3, 4) == 0);
    assert(graph_has_edge(g, 3, 6) == 0);

    graph_free(g);
}

// Тест для графа с 11 вершинами
static void test_big_graph() {
    graph_t *g = graph_create(11);
    assert(g != NULL);

    // Добавление рёбер
    assert(graph_add_edge(g, 0, 1) == 0); 
    assert(graph_add_edge(g, 1, 2) == 0); 
    assert(graph_add_edge(g, 1, 3) == 0); 
    assert(graph_add_edge(g, 3, 4) == 0);  
    assert(graph_add_edge(g, 4, 5) == 0); 
    assert(graph_add_edge(g, 3, 6) == 0); 
    assert(graph_add_edge(g, 6, 7) == 0); 
    assert(graph_add_edge(g, 3, 8) == 0); 
    assert(graph_add_edge(g, 8, 10) == 0); 
    assert(graph_add_edge(g, 8, 9) == 0); 
    assert(graph_add_edge(g, 9, 10) == 0);  

    // Упрощаем
    graph_simplify(g);

    // Ожидаем, что остались только рёбра:
    // (0,1), (1,2), (1,3), (3,5), (3,7), (3,10)
    assert(graph_has_edge(g, 0, 1) == 1);
    assert(graph_has_edge(g, 1, 2) == 1);
    assert(graph_has_edge(g, 1, 3) == 1);
    assert(graph_has_edge(g, 3, 5) == 1);
    assert(graph_has_edge(g, 3, 7) == 1);
    assert(graph_has_edge(g, 3, 10) == 1);

    // Проверяем, что других рёбер нет
    for (size_t u = 0; u < 11; ++u) {
        for (size_t v = u + 1; v < 11; ++v) {
            int should_exist =
                (u == 0 && v == 1) ||
                (u == 1 && v == 2) ||
                (u == 1 && v == 3) ||
                (u == 3 && v == 5) ||
                (u == 3 && v == 7) ||
                (u == 3 && v == 10);

            if (!should_exist) {
                assert(graph_has_edge(g, u, v) == 0);
            }
        }
    }

    // Проверяем степени вершин
    assert(graph_degree(g, 0) == 1);
    assert(graph_degree(g, 1) == 3);
    assert(graph_degree(g, 2) == 1);
    assert(graph_degree(g, 3) == 4);
    assert(graph_degree(g, 5) == 1);
    assert(graph_degree(g, 7) == 1);
    assert(graph_degree(g, 10) == 1);

    size_t degrees[] = {4, 6, 8, 9};
    for (size_t i = 0; i < sizeof(degrees) / sizeof(degrees[0]); i++) {
        assert(graph_degree(g, degrees[i]) == 0);
    }

    graph_free(g);
}

int main() {
    test_edge_case_null_and_zero();
    test_edge_case_invalid_and_self_loop();
    test_simple_single_edge();
    test_simple_path_len3();
    test_medium_path_len5();
    test_medium_cycle4();
    test_complex_mixed_graph();
    test_two_components();
    test_big_graph();

    printf("Все тесты пройдены\n");
    return 0;
}