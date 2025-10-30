#include "../include/mylib.h"
#include <stdio.h>
#include <stdlib.h>

Graph *create_graph(size_t n) {
    Graph *g = malloc(sizeof(Graph));
    if (!g) {
        return NULL;
    }

    g->n = n;
    g->matrix = malloc(n * sizeof(uint8_t *));
    if (!g->matrix) {
        free(g);
        return NULL;
    }

    for (size_t i = 0; i < n; i++) {
        g->matrix[i] = calloc(n, sizeof(uint8_t));
        if (!g->matrix[i]) {
            free_graph(g);
            return NULL;
        }
    }

    return g;
}

int add_edge(Graph *g, size_t u, size_t v) {
    if (!g) {
        printf("Ошибка: передан неинициализированный граф. \n");
        return 1;
    }
    if (u >= g->n || v >= g->n) {
        printf("Ребро (%zu, %zu) игнорируется (индексы вне диапазона 0..%zu) \n", u, v, g->n-1);
        return 1;
    }
    if (u == v) {
        printf("Ребро (%zu, %zu) игнорируется (петля) \n", u, v);
        return 1;
    }
    g->matrix[u][v] = 1;
    g->matrix[v][u] = 1;

    return 0;
}

void free_graph(Graph *g) {
    if (!g) {
        return;
    }
    for (size_t i = 0; i < g->n; i++) {
        free(g->matrix[i]);
    }
    free(g->matrix);
    free(g);
}

// BFS, вернуть макс расстояние от start
uint32_t bfs_farthest(const Graph *g, size_t start) {
    if (!g || g->n == 0 || start >= g->n) {
        return 0;
    }

    size_t n = g->n;
    int32_t *dist = malloc(n * sizeof(int32_t));
    uint8_t *used = malloc(n * sizeof(uint8_t));
    size_t *queue = malloc(n * sizeof(size_t));
    if (!dist || !used || !queue) {
        free(dist);
        free(used);
        free(queue);
        return 0;
    }

    for (size_t i = 0; i < n; i++) {
        dist[i] = -1;
        used[i] = 0;
    }

    size_t head = 0;
    size_t tail = 0;
    queue[tail++] = start;
    dist[start] = 0;
    used[start] = 1;

    uint32_t max_dist = 0;

    while (head < tail) {
        size_t v = queue[head++];
        for (size_t u = 0; u < n; u++) {
            if (g->matrix[v][u] && !used[u]) {
                used[u] = 1;
                dist[u] = dist[v] + 1;
                queue[tail++] = u;
                if ((uint32_t)dist[u] > max_dist)
                    max_dist = dist[u];
            }
        }
    }

    free(dist);
    free(used);
    free(queue);
    return max_dist;
}

// диаметр графа
uint32_t compute_graph_diameter(const Graph *g) {
    if (!g || g->n == 0 || g->n == 1) {
        return 0;
    }

    uint32_t diameter = 0;
    for (size_t i = 0; i < g->n; i++) {
        uint32_t d = bfs_farthest(g, i);
        if (d > diameter)
            diameter = d;
    }

    return diameter;
}