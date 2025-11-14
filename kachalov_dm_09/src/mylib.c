#include "../include/mylib.h"
#include <stdio.h>
#include <stdlib.h>

// Внутренняя функция: освобождение полей структуры graph
// Используется как из graph_free, так и при неудачной инициализации
static void graph_free_internal(graph_t *g) {
    if (!g) {
        return;
    }

    if (g->adj) {
        for (size_t i = 0; i < g->n; i++) {
            free(g->adj[i]);
        }
        free(g->adj);
    }

    free(g->deg);
    free(g->cap);
}

// Создание графа
graph_t *graph_create(size_t vertex_count) {
    graph_t *g = (graph_t *)malloc(sizeof(graph_t));
    if (!g) {
        return NULL;
    }

    g->n = vertex_count;
    g->deg = NULL;
    g->cap = NULL;
    g->adj = NULL;

    if (vertex_count == 0) {
        return g;
    }

    g->deg = (size_t *)calloc(vertex_count, sizeof(size_t));
    g->cap = (size_t *)calloc(vertex_count, sizeof(size_t));
    g->adj = (int **)calloc(vertex_count, sizeof(int *));
    if (!g->deg || !g->cap || !g->adj) {
        graph_free_internal(g);
        free(g);
        return NULL;
    }

    return g;
}

// Освобождение графа
void graph_free(graph_t *g) {
    if (!g) {
        return;
    }

    graph_free_internal(g);
    free(g);
}

// Внутренняя функция расширения списка смежности вершины v
// до не менее чем new_cap элементов
static int ensure_capacity_for_vertex(graph_t *g, size_t v, size_t new_cap) {
    if (g->cap[v] >= new_cap) {
        return 0;
    }

    size_t cap = g->cap[v] == 0 ? 4 : g->cap[v];
    while (cap < new_cap) {
        cap *= 2;
    }

    int *tmp = (int *)realloc(g->adj[v], cap * sizeof(int));
    if (!tmp) {
        return -1;
    }

    g->adj[v] = tmp;
    g->cap[v] = cap;

    return 0;
}

// Проверка наличия вершины u в списке смежности вершины v
static int has_neighbor(const graph_t *g, size_t v, size_t u) {
    if (!g || v >= g->n) {
        return 0;
    }

    for (size_t i = 0; i < g->deg[v]; i++) {
        if ((size_t)g->adj[v][i] == u) {
            return 1;
        }
    }

    return 0;
}

// Внутренняя функция удаления u из списка смежности v
// Если сосед не найден, ничего не делает
static void remove_neighbor(graph_t *g, size_t v, size_t u) {
    if (!g || v >= g->n) {
        return;
    }

    size_t d = g->deg[v];
    for (size_t i = 0; i < d; i++) {
        if ((size_t)g->adj[v][i] == u) {
            // Переносим последний элемент на место удаляемого
            g->adj[v][i] = g->adj[v][d - 1];
            g->deg[v]--;
            return;
        }
    }
}

// Внутренняя функция удаления неориентированного ребра (u, v)
static void remove_edge_undirected(graph_t *g, size_t u, size_t v) {
    if (!g) {
        return;
    }
    if (u >= g->n || v >= g->n) {
        return;
    }
    if (u == v) {
        return;
    }

    remove_neighbor(g, u, v);
    remove_neighbor(g, v, u);
}

// Добавление неориентированного ребра
int graph_add_edge(graph_t *g, size_t u, size_t v) {
    if (!g) {
        return -1;
    }

    if (u >= g->n || v >= g->n) {
        return -1;
    }

    if (u == v) {
        // Не допускаем петель
        return 0;
    }

    if (has_neighbor(g, u, v)) {
        // Не допускаем кратных рёбер
        return 0;
    }

    if (ensure_capacity_for_vertex(g, u, g->deg[u] + 1) != 0) {
        return -2;
    }
    if (ensure_capacity_for_vertex(g, v, g->deg[v] + 1) != 0) {
        return -2;
    }

    g->adj[u][g->deg[u]++] = (int)v;
    g->adj[v][g->deg[v]++] = (int)u;

    return 0;
}

// Проверка существования ребра
int graph_has_edge(const graph_t *g, size_t u, size_t v) {
    if (!g) {
        return 0;
    }
    if (u >= g->n || v >= g->n) {
        return 0;
    }
    if (u == v) {
        return 0;
    }

    return has_neighbor(g, u, v);
}

// Количество вершин
size_t graph_vertex_count(const graph_t *g) {
    if (!g) {
        return 0;
    }

    return g->n;
}

// Степень вершины
size_t graph_degree(const graph_t *g, size_t v) {
    if (!g) {
        return 0;
    }
    if (v >= g->n) {
        return 0;
    }

    return g->deg[v];
}

// Упрощение графа: последовательное «сглаживание» вершин степени 2
void graph_simplify(graph_t *g) {
    if (!g) {
        return;
    }

    int changed = 1;
    while (changed) {
        changed = 0;

        for (size_t v = 0; v < g->n; v++) {
            if (g->deg[v] != 2) {
                continue;
            }

            int u = g->adj[v][0];
            int w = g->adj[v][1];

            if ((size_t)u >= g->n || (size_t)w >= g->n) {
                continue;
            }

            // Удаляем рёбра (u, v) и (v, w)
            remove_edge_undirected(g, (size_t)u, v);
            remove_edge_undirected(g, v, (size_t)w);

            // Вершина v теперь изолирована (степень 0)
            // Добавляем ребро (u, w), если это разные вершины и ребра ещё нет
            if ((size_t)u != (size_t)w && !graph_has_edge(g, (size_t)u, (size_t)w)) {
                // Ошибку выделения памяти здесь игнорируем:
                // в худшем случае ребро просто не добавится
                (void)graph_add_edge(g, (size_t)u, (size_t)w);
            }

            changed = 1;
        }
    }
}

// Печать графа (списки смежности)
void print_graph(const graph_t *g) {
    if (!g) {
        return;
    }

    size_t n = graph_vertex_count(g);
    for (size_t v = 0; v < n; v++) {
        printf("%zu:", v);
        for (size_t i = 0; i < g->deg[v]; i++) {
            printf(" %d", g->adj[v][i]);
        }
        printf("\n");
    }
}