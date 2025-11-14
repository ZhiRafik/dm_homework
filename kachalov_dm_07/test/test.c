#include <assert.h>
#include <stdio.h>
#include "../include/mylib.h"

static void test_create_graph(void) {
    Graph *g = create_graph(3);
    assert(g != NULL);
    assert(g->n == 3);
    // матрица должна быть из нулей
    for (size_t i = 0; i < g->n; i++) {
        for (size_t j = 0; j < g->n; j++) {
            assert(g->matrix[i][j] == 0);
        }
    }
    free_graph(g);
}

static void test_add_edge_ok(void) {
    Graph *g = create_graph(4);
    assert(g != NULL);

    int r = add_edge(g, 1, 3);
    assert(r == 0);
    // неориентированный граф -> обе стороны = 1
    assert(g->matrix[1][3] == 1);
    assert(g->matrix[3][1] == 1);

    // остальные должны быть 0
    for (size_t i = 0; i < g->n; i++) {
        for (size_t j = 0; j < g->n; j++) {
            if (!((i == 1 && j == 3) || (i == 3 && j == 1))) {
                assert(g->matrix[i][j] == 0);
            }   
        }
    }

    free_graph(g);
}

static void test_add_edge_out_of_range(void) {
    Graph *g = create_graph(2);
    assert(g != NULL);

    int r = add_edge(g, 5, 6);
    assert(r == 1); // должно вернуть ошибку
    // ничего не должно измениться
    assert(g->matrix[0][0] == 0);
    assert(g->matrix[0][1] == 0);
    assert(g->matrix[1][0] == 0);
    assert(g->matrix[1][1] == 0);

    free_graph(g);
}

// пример 1: диаметр = 3, цепочка
//   0 — 1 — 2 — 3
//   |
//   4
static void test_diameter_example_1_a_path(void) {
    Graph *g = create_graph(5);
    assert(g != NULL);

    assert(add_edge(g, 0, 1) == 0);
    assert(add_edge(g, 1, 2) == 0);
    assert(add_edge(g, 2, 3) == 0);
    assert(add_edge(g, 0, 4) == 0);

    int d = compute_graph_diameter(g);
    assert(d == 4);

    free_graph(g);
}

// пример 2: тоже диаметр = 3, не цепочка
//     4
//     |
// 0 - 1 - 2 - 3
static void test_diameter_example_2_not_a_path(void) {
    Graph *g = create_graph(5);
    assert(g != NULL);

    assert(add_edge(g, 0, 1) == 0);
    assert(add_edge(g, 1, 4) == 0);
    assert(add_edge(g, 1, 2) == 0);
    assert(add_edge(g, 2, 3) == 0);

    int d = compute_graph_diameter(g);
    assert(d == 3);

    free_graph(g);
}

// пример 3: диаметр = 5
//
//      9 - 6 - 7 - 8
//          |
// 10 - 4   5
//      | / 
//  0 - 1 - 2 - 3 - 11
static void test_diameter_example_3_complex(void) {
    Graph *g = create_graph(12);
    assert(g != NULL);

    assert(add_edge(g, 0, 1) == 0);
    assert(add_edge(g, 1, 2) == 0);
    assert(add_edge(g, 2, 3) == 0);
    assert(add_edge(g, 1, 4) == 0);
    assert(add_edge(g, 4, 10) == 0);
    assert(add_edge(g, 1, 5) == 0);
    assert(add_edge(g, 5, 6) == 0);
    assert(add_edge(g, 6, 9) == 0);
    assert(add_edge(g, 6, 7) == 0);
    assert(add_edge(g, 7, 8) == 0);
    assert(add_edge(g, 3, 11) == 0);

    int d = compute_graph_diameter(g);
    assert(d == 7); // (11 - 3 - 2 - 1 - 5 - 6 - 7 - 8)

    free_graph(g);
}

// пример 4: диаметр = 7, с циклом
//
//      9 - 6 - 7 - 8
//      |   |
// 10 - 4 - 5
//      | / 
//  0 - 1 - 2 - 3 - 11
static void test_diameter_example_4_complex_with_cycle(void) {
    Graph *g = create_graph(12);
    assert(g != NULL);

    assert(add_edge(g, 0, 1) == 0);
    assert(add_edge(g, 1, 2) == 0);
    assert(add_edge(g, 2, 3) == 0);
    assert(add_edge(g, 1, 4) == 0);
    assert(add_edge(g, 4, 10) == 0);
    assert(add_edge(g, 1, 5) == 0);
    assert(add_edge(g, 5, 6) == 0);
    assert(add_edge(g, 6, 9) == 0);
    assert(add_edge(g, 6, 7) == 0);
    assert(add_edge(g, 7, 8) == 0);
    assert(add_edge(g, 4, 9) == 0);
    assert(add_edge(g, 3, 11) == 0);
    assert(add_edge(g, 4, 5) == 0);

    int d = compute_graph_diameter(g);
    assert(d == 7); 

    free_graph(g);
}

// несвязный граф
// 0 - 1        2 - 3
static void test_unconnected_graph_with_inf_1(void) {
    Graph *g = create_graph(4);
    assert(g != NULL);

    assert(add_edge(g, 0, 1) == 0);
    assert(add_edge(g, 2, 3) == 0);

    int d = compute_graph_diameter(g);
    assert(d == -1); // возвращается минус 1, печатается сообщение, что равен бесконечности

    free_graph(g);
}

// несвязный граф
// 0 - 1 - 2 - 3       4 - 5
static void test_unconnected_graph_with_inf_2(void) {
    Graph *g = create_graph(6);
    assert(g != NULL);

    assert(add_edge(g, 0, 1) == 0);
    assert(add_edge(g, 2, 3) == 0);
    assert(add_edge(g, 1, 2) == 0);
    assert(add_edge(g, 4, 5) == 0);

    int d = compute_graph_diameter(g);
    assert(d == -1); // возвращается минус 1, печатается сообщение, что равен бесконечности

    free_graph(g);
}

//
//      9 - 6 - 7 - 8           14
//          |                    |
// 10 - 4   5              12 - 13
//      | / 
//  0 - 1 - 2 - 3 - 11
static void test_unconnected_graph_with_inf_3(void) {
    Graph *g = create_graph(15);
    assert(g != NULL);

    assert(add_edge(g, 0, 1) == 0);
    assert(add_edge(g, 1, 2) == 0);
    assert(add_edge(g, 2, 3) == 0);
    assert(add_edge(g, 1, 4) == 0);
    assert(add_edge(g, 4, 10) == 0);
    assert(add_edge(g, 1, 5) == 0);
    assert(add_edge(g, 5, 6) == 0);
    assert(add_edge(g, 6, 9) == 0);
    assert(add_edge(g, 6, 7) == 0);
    assert(add_edge(g, 7, 8) == 0);
    assert(add_edge(g, 3, 11) == 0);
    assert(add_edge(g, 12, 13) == 0);
    assert(add_edge(g, 13, 14) == 0);

    int d = compute_graph_diameter(g);
    assert(d == -1); 

    free_graph(g);
}

// тест на одиночную вершину
static void test_diameter_single_vertex(void) {
    Graph *g = create_graph(1);
    assert(g != NULL);

    int d = compute_graph_diameter(g);
    assert(d == 0);

    free_graph(g);
}

int main(void) {
    test_create_graph();
    test_add_edge_ok();
    test_add_edge_out_of_range();
    test_diameter_example_1_a_path();
    test_diameter_example_2_not_a_path();
    test_diameter_example_3_complex();
    test_diameter_example_4_complex_with_cycle();
    test_diameter_single_vertex();
    test_unconnected_graph_with_inf_1();
    test_unconnected_graph_with_inf_2();
    test_unconnected_graph_with_inf_3();

    printf("Все тесты прошли успешно!");
    return 0;
}