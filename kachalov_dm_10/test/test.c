#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "mylib.h"

// Тест графа с одной вершиной
static void test_single_vertex(void) {
    graph_t *g;
    int colors[1];
    int res;

    g = graph_create(1);
    assert(g != NULL);

    res = graph_color_5(g, colors);
    assert(res == 0);
    assert(graph_is_coloring_correct(g, colors, 1));

    graph_free(g);
}

// Тест ребра между двумя вершинами
static void test_two_vertices_edge(void) {
    graph_t *g;
    int colors[2];
    int res;

    g = graph_create(2);
    assert(g != NULL);

    assert(graph_add_edge(g, 0, 1) == 0);

    res = graph_color_5(g, colors);
    assert(res == 0);
    assert(graph_is_coloring_correct(g, colors, 2));
    assert(colors[0] != colors[1]);

    graph_free(g);
}

// Тест треугольника 3 цикла
static void test_triangle(void) {
    graph_t *g;
    int colors[3];
    int res;

    g = graph_create(3);
    assert(g != NULL);

    assert(graph_add_edge(g, 0, 1) == 0);
    assert(graph_add_edge(g, 1, 2) == 0);
    assert(graph_add_edge(g, 2, 0) == 0);

    res = graph_color_5(g, colors);
    assert(res == 0);
    assert(graph_is_coloring_correct(g, colors, 3));

    graph_free(g);
}

// Тест квадрата с диагональю
static void test_square_with_diagonal(void) {
    graph_t *g;
    int colors[4];
    int res;

    g = graph_create(4);
    assert(g != NULL);

    assert(graph_add_edge(g, 0, 1) == 0);
    assert(graph_add_edge(g, 1, 2) == 0);
    assert(graph_add_edge(g, 2, 3) == 0);
    assert(graph_add_edge(g, 3, 0) == 0);
    assert(graph_add_edge(g, 0, 2) == 0);

    res = graph_color_5(g, colors);
    assert(res == 0);
    assert(graph_is_coloring_correct(g, colors, 4));

    graph_free(g);
}

// Тест полного графа на 5 вершинах K5
static void test_complete_graph_5(void) {
    graph_t *g;
    int colors[5];
    int res;
    size_t i;
    size_t j;
    int used[5];

    g = graph_create(5);
    assert(g != NULL);

    for (i = 0; i < 5; i++) {
        for (j = i + 1; j < 5; j++) {
            assert(graph_add_edge(g, i, j) == 0);
        }
    }

    res = graph_color_5(g, colors);
    assert(res == 0);
    assert(graph_is_coloring_correct(g, colors, 5));

    for (i = 0; i < 5; i++) {
        used[i] = 0;
    }

    for (i = 0; i < 5; i++) {
        assert(colors[i] >= 0 && colors[i] < 5);
        used[colors[i]] = 1;
    }

    {
        int sum;

        sum = 0;
        for (i = 0; i < 5; i++) {
            sum += used[i];
        }
        assert(sum == 5);
    }

    graph_free(g);
}

// Тест большого графа со смешанной структурой
static void test_big_graph(void) {
    graph_t *g;
    int *colors;
    size_t n;
    int res;

    n = 10;
    g = graph_create(n);
    assert(g != NULL);

    // цепочка
    assert(graph_add_edge(g, 0, 1) == 0);
    assert(graph_add_edge(g, 1, 2) == 0);
    assert(graph_add_edge(g, 2, 3) == 0);
    assert(graph_add_edge(g, 3, 4) == 0);

    // дополнительные ребра
    assert(graph_add_edge(g, 1, 4) == 0);
    assert(graph_add_edge(g, 2, 5) == 0);
    assert(graph_add_edge(g, 5, 6) == 0);
    assert(graph_add_edge(g, 6, 7) == 0);
    assert(graph_add_edge(g, 7, 8) == 0);
    assert(graph_add_edge(g, 8, 9) == 0);
    assert(graph_add_edge(g, 4, 9) == 0);

    colors = malloc(n * sizeof(int));
    assert(colors != NULL);

    res = graph_color_5(g, colors);
    assert(res == 0);
    assert(graph_is_coloring_correct(g, colors, n));

    free(colors);
    graph_free(g);
}

// Тест некорректных аргументов для раскраски
static void test_invalid_arguments(void) {
    graph_t *g;
    int colors[2];
    int res;

    g = graph_create(2);
    assert(g != NULL);

    res = graph_color_5(NULL, colors);
    assert(res != 0);

    res = graph_color_5(g, NULL);
    assert(res != 0);

    assert(graph_is_coloring_correct(NULL, colors, 2) == false);
    assert(graph_is_coloring_correct(g, NULL, 2) == false);
    assert(graph_is_coloring_correct(g, colors, 1) == false);

    graph_free(g);
}

// Тест отказа при петле
static void test_self_loop_rejected(void) {
    graph_t *g;

    g = graph_create(3);
    assert(g != NULL);

    assert(graph_add_edge(g, 0, 0) != 0);

    graph_free(g);
}

// Тест отказа при кратном ребре
static void test_duplicate_edge_rejected(void) {
    graph_t *g;

    g = graph_create(3);
    assert(g != NULL);

    assert(graph_add_edge(g, 0, 1) == 0);
    assert(graph_add_edge(g, 0, 1) != 0);

    graph_free(g);
}

// Тест функции проверки раскраски на неверной раскраске
static void test_incorrect_coloring_detection(void) {
    graph_t *g;
    int colors[3];

    g = graph_create(3);
    assert(g != NULL);

    assert(graph_add_edge(g, 0, 1) == 0);
    assert(graph_add_edge(g, 1, 2) == 0);

    colors[0] = 1;
    colors[1] = 1;
    colors[2] = 2;

    assert(graph_is_coloring_correct(g, colors, 3) == false);

    graph_free(g);
}

static void test_incorrect_coloring_square(void) {
    graph_t *g;
    int colors[4];

    g = graph_create(4);
    assert(g != NULL);

    // рёбра квадрата
    assert(graph_add_edge(g, 0, 1) == 0);
    assert(graph_add_edge(g, 1, 2) == 0);
    assert(graph_add_edge(g, 2, 3) == 0);
    assert(graph_add_edge(g, 3, 0) == 0);
    assert(graph_add_edge(g, 3, 1) == 0);

    // неправильная раскраска
    colors[0] = 2;
    colors[1] = 3;
    colors[2] = 4;
    colors[3] = 3;   // ошибка: вершина 3 смежна с вершиной 1

    assert(graph_is_coloring_correct(g, colors, 4) == false);

    graph_free(g);
}

static void test_quadrilateral_with_diagonal(void) {
    graph_t *g;
    int colors[4];
    int res;
    size_t i;

    g = graph_create(4);
    assert(g != NULL);

    assert(graph_add_edge(g, 1, 2) == 0);
    assert(graph_add_edge(g, 2, 3) == 0);
    assert(graph_add_edge(g, 3, 0) == 0);
    assert(graph_add_edge(g, 0, 1) == 0);
    assert(graph_add_edge(g, 0, 2) == 0);

    res = graph_color_5(g, colors);
    assert(res == 0);
    assert(graph_is_coloring_correct(g, colors, 4));

    for (i = 0; i < 4; i++) {
        assert(colors[i] >= 0 && colors[i] < 5);
    }

    graph_free(g);
}

// Тест графа с картинки из интернета (могу картинку скинуть, рисовать с ума сойду)
static void test_picture_graph(void) {
    graph_t *g;
    int colors[8];
    int res;
    size_t i;

    g = graph_create(8);
    assert(g != NULL);

    // x1
    assert(graph_add_edge(g, 0, 5) == 0); // x1 - x6
    assert(graph_add_edge(g, 0, 6) == 0); // x1 - x7
    assert(graph_add_edge(g, 0, 1) == 0); // x1 - x2

    // x2
    assert(graph_add_edge(g, 1, 3) == 0); // x2 - x4
    assert(graph_add_edge(g, 1, 2) == 0); // x2 - x3
    assert(graph_add_edge(g, 1, 7) == 0); // x2 - x8

    // x3
    assert(graph_add_edge(g, 2, 3) == 0); // x3 - x4
    assert(graph_add_edge(g, 2, 7) == 0); // x3 - x8

    // x4
    assert(graph_add_edge(g, 3, 4) == 0); // x4 - x5
    assert(graph_add_edge(g, 3, 6) == 0); // x4 - x7

    // x5
    assert(graph_add_edge(g, 4, 5) == 0); // x5 - x6
    assert(graph_add_edge(g, 4, 6) == 0); // x5 - x7

    // x6
    // ребро x6 - x1 уже добавлено
    // ребро x6 - x5 уже добавлено
    assert(graph_add_edge(g, 5, 6) == 0); // x6 - x7

    // x7
    assert(graph_add_edge(g, 6, 7) == 0); // x7 - x8

    // x8
    // рёбра x8 - x2 и x8 - x3 уже добавлены
    res = graph_color_5(g, colors);
    assert(res == 0);
    assert(graph_is_coloring_correct(g, colors, 8));

    for (i = 0; i < 8; i++) {
        assert(colors[i] >= 0 && colors[i] < 5);
    }

    graph_free(g);
}

int main(void) {
    test_single_vertex();
    test_two_vertices_edge();
    test_triangle();
    test_square_with_diagonal();
    test_complete_graph_5();
    test_big_graph();
    test_invalid_arguments();
    test_self_loop_rejected();
    test_duplicate_edge_rejected();
    test_quadrilateral_with_diagonal();
    test_incorrect_coloring_detection();
    test_picture_graph();
    test_incorrect_coloring_square();

    printf("all tests passed\n");

    return 0;
}