#include "../include/mylib.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    size_t n;
    size_t m;

    printf("Введите число вершин: ");
    if (scanf("%zu", &n) != 1) {
        printf("Ошибка ввода\n");
        return 1;
    }

    graph_t *g = graph_create(n);
    if (!g) {
        printf("Ошибка выделения памяти\n");
        return 1;
    }

    printf("Введите число рёбер: ");
    if (scanf("%zu", &m) != 1) {
        printf("Ошибка ввода\n");
        graph_free(g);
        return 1;
    }

    for (size_t i = 0; i < m; i++) {
        size_t u, v;
        printf("Ребро %zu (u v): ", i + 1);
        if (scanf("%zu %zu", &u, &v) != 2) {
            printf("Ошибка ввода\n");
            graph_free(g);
            return 1;
        }
        int r = graph_add_edge(g, u, v);
        if (r == -1) {
            printf("Некорректный индекс вершины\n");
        } else if (r == -2) {
            printf("Ошибка выделения памяти при добавлении ребра\n");
        }
    }

    printf("\nГраф до упрощения:\n");
    print_graph(g);

    graph_simplify(g);

    printf("\nГраф после упрощения:\n");
    print_graph(g);

    graph_free(g);
    return 0;
}