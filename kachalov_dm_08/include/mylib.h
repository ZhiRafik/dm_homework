#ifndef MYLIB_H
#define MYLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// n — число вершин
// adj — массив списков смежности (индексация с 1)
typedef struct {
    int **adj;
    int n;
} Tree;

// восстанавливает дерево по коду Прюфера длины m
// code — массив длины m (значения от 1 до n)
// m — длина кода (n - 2)
// возвращает указатель на структуру Tree (списки смежности по ТЗ)
Tree *decode_prufer(int *code, int m);

// генерирует случайное дерево порядка n
Tree *generate_random_tree(int n);

// выводит дерево в виде списков смежности
void print_tree(Tree *t);

// освобождает память
void free_tree(Tree *t);

#endif