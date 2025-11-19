#include <stdio.h>
#include <stdlib.h>
#include "../include/mylib.h"

int main() {
    size_t n, m, i, u, v;
    graph_t *graph;
    int *colors;
    int res;

    printf("Введите количество вершин n и количество ребер m:\n");

    if (scanf("%zu %zu", &n, &m) != 2) {
        fprintf(stderr, "ошибка ввода\n");
        return 1;
    }

    graph = graph_create(n);
    if (graph == NULL) {
        fprintf(stderr, "ошибка создания графа\n");
        return 1;
    }

    printf("Введите пары вершин для каждого ребра:\n");

    for (i = 0; i < m; i++) {
        if (scanf("%zu %zu", &u, &v) != 2) {
            fprintf(stderr, "ошибка ввода\n");
            graph_free(graph);
            return 1;
        }

        if (graph_add_edge(graph, u, v) != 0) {
            fprintf(stderr, "ошибка добавления ребра\n");
            graph_free(graph);
            return 1;
        }
    }

    colors = malloc(n * sizeof(int));
    if (colors == NULL) {
        fprintf(stderr, "ошибка выделения памяти\n");
        graph_free(graph);
        return 1;
    }

    res = graph_color_5(graph, colors);
    if (res != 0) {
        fprintf(stderr, "ошибка раскраски\n");
        free(colors);
        graph_free(graph);
        return 1;
    }

    printf("Раскраска графа:\n");

    for (i = 0; i < n; i++) {
        printf("%zu %d\n", i, colors[i]);
    }

    free(colors);
    graph_free(graph);

    return 0;
}