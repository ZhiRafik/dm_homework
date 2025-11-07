#include "../include/mylib.h"

// внутренний флаг для инициализации ГПСЧ один раз
static int rng_initialized = 0;

// инициализация генератора случайных чисел
static void init_rng(void) {
    if (!rng_initialized) {
        srand((unsigned int)time(NULL)); // инициализация по текущему времени
        rng_initialized = 1; // больше не вызывать повторно
    }
} // по текущему времени реализуем рандом, т.к. простой rand() может повторяться при перезапуске программы

// восстановление дерева по коду Прюфера
Tree *decode_prufer(int *code, int m) {
    if (m < 0) {
        return NULL;
    }

    int n = m + 2;
    if (n <= 0) {
        return NULL;
    }

    // degree[i] — текущая степень вершины i
    int *degree = (int *)calloc(n + 1, sizeof(int));
    if (!degree) {
        return NULL;
    }

    // изначально у всех вершин степень 1
    for (int i = 1; i <= n; i++) {
        degree[i] = 1;
    }

    // увеличиваем степени вершин, встречающихся в коде Прюфера
    for (int i = 0; i < m; i++) {
        int v = code[i];
        if (v < 1 || v > n) {
            free(degree);
            return NULL;
        }
        degree[v]++;
    }

    // создаём структуру дерева
    Tree *t = (Tree *)malloc(sizeof(Tree));
    if (!t) {
        free(degree);
        return NULL;
    }
    t->n = n;
    t->adj = (int **)malloc((n + 1) * sizeof(int *));
    if (!t->adj) {
        free(degree);
        free(t);
        return NULL;
    }

    // для каждой вершины выделяем массив размера degree[i] + 1
    // adj[i][0] будет хранить текущее количество соседей
    for (int i = 1; i <= n; i++) {
        t->adj[i] = (int *)malloc((degree[i] + 1) * sizeof(int));
        if (!t->adj[i]) {
            for (int j = 1; j < i; j++) {
                free(t->adj[j]);
            }
            free(t->adj);
            free(degree);
            free(t);
            return NULL;
        }
        t->adj[i][0] = 0;
    }

    // восстановление рёбер
    for (int i = 0; i < m; i++) {
        int leaf = -1;

        // ищем вершину с degree == 1 и минимальным номером
        for (int v = 1; v <= n; v++) {
            if (degree[v] == 1) {
                leaf = v;
                break;
            }
        }

        int v = code[i];

        // добавляем ребро leaf - v
        int deg_leaf = ++t->adj[leaf][0];
        int deg_v = ++t->adj[v][0];
        t->adj[leaf][deg_leaf] = v;
        t->adj[v][deg_v] = leaf;

        // обновляем степени
        degree[leaf]--;
        degree[v]--;
    }

    // остаются две вершины степени 1 — соединяем их
    int u = -1;
    int v = -1;
    for (int i = 1; i <= n; i++) {
        if (degree[i] == 1) {
            if (u == -1) {
                u = i;
            } else {
                v = i;
                break;
            }
        }
    }

    if (u != -1 && v != -1) {
        int deg_u = ++t->adj[u][0];
        int deg_v = ++t->adj[v][0];
        t->adj[u][deg_u] = v;
        t->adj[v][deg_v] = u;
    }

    free(degree);
    return t;
}

// генерация случайного дерева порядка n
Tree *generate_random_tree(int n) {
    if (n <= 0) {
        return NULL;
    }

    // n == 1: дерево из одной вершины без рёбер
    if (n == 1) {
        Tree *t = (Tree *)malloc(sizeof(Tree));
        if (!t) {
            return NULL;
        }
        t->n = 1;
        t->adj = (int **)malloc(2 * sizeof(int *));
        if (!t->adj) {
            free(t);
            return NULL;
        }
        t->adj[1] = (int *)malloc(sizeof(int));
        if (!t->adj[1]) {
            free(t->adj);
            free(t);
            return NULL;
        }
        t->adj[1][0] = 0;
        return t;
    }

    int m = n - 2;
    int *code = (int *)malloc(m * sizeof(int));
    if (!code) {
        return NULL;
    }

    init_rng();

    // равномерно выбираем каждое значение кода из {1, ..., n}
    for (int i = 0; i < m; i++) {
        code[i] = (rand() % n) + 1;
    }

    Tree *t = decode_prufer(code, m);
    free(code);
    return t;
}

// вывод дерева в формате списков смежности
void print_tree(Tree *t) {
    if (!t || !t->adj) {
        return;
    }

    for (int i = 1; i <= t->n; i++) {
        printf("%d:", i);
        if (t->adj[i]) {
            int deg = t->adj[i][0];
            for (int j = 1; j <= deg; j++) {
                printf(" %d", t->adj[i][j]);
            }
        }
        printf("\n");
    }
}

// освобождение памяти, занятой деревом
void free_tree(Tree *t) {
    if (!t) {
        return;
    }
    if (t->adj) {
        for (int i = 1; i <= t->n; i++) {
            free(t->adj[i]);
        }
        free(t->adj);
    }
    free(t);
}