#include <stdio.h>
#include <stdlib.h>
#include "../include/mylib.h"

int main(void) {
    size_t n, m;

    printf("Введите количество вершин и рёбер графа: \n");
    if (scanf("%zu %zu", &n, &m) != 2) {
        return 1;
    }
    printf("Вершины графа пронумерованы от 0 до %zu \n", n-1);

    Graph *g = create_graph(n);
    if (!g) {
        return 1;
    }

    printf("Введите %zu пар(у|ы) смежных вершин в формате (u, v): \n", m);
    for (size_t i = 0; i < m; i++) {
        size_t u, v;
        if (scanf("%zu %zu", &u, &v) != 2) {
            free_graph(g);
            return 1;
        }
        if (add_edge(g, u, v) == 1) {
            printf("Ошибка создания ребра. Попробуйте ещё раз. \n");
            i--; // отматываем счётчик назад
        };
    }

    int d = compute_graph_diameter(g);
    if (d == -1) {
        return 0;
    }
    printf("Диаметр графа: %u\n", d);

    free_graph(g);
    return 0;
}