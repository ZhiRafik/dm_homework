#include "../include/mylib.h"
#include <stdlib.h>
#include <stdio.h>

/*
В идеале я бы хотел для начала отсортировать отношения по возрастанию первого элемента слева
и правого элемента справа, чтобы после этого обойти два списка с помощью двух указателей,
а не вложенного цикла в цикле - так сложность будет О(logn * n), а не O(n^2),
но на реализацию сортировки потребуется очень много кода на С - a я пока плохо умею:(
*/
struct Relation *join_binary(struct Relation *r, struct Relation *s) {
    // максимальный размер результата
    unsigned maxSize = r->size * s->size;
    struct Relation *out = malloc(sizeof(struct Relation));
    out->size = 0;
    out->pairs = malloc(maxSize * sizeof(struct Pair));

    for (unsigned i = 0; i < r->size; i++) {
        for (unsigned j = 0; j < s->size; j++) {
            if (r->pairs[i].second == s->pairs[j].first) {
                out->pairs[out->size].first = r->pairs[i].first;
                out->pairs[out->size].second = s->pairs[j].second;
                out->size++;
            }
        }
    }

    return out;
}

void initRelation(struct Relation *r) {
    unsigned n;
    scanf("%u", &n);
    r->size = n;
    r->pairs = malloc(sizeof(struct Pair) * n);
    printf("Введите %u пар (first, second):\n", n);
    for (unsigned i = 0; i < n; i++) {
        scanf("%d %d", &r->pairs[i].first, &r->pairs[i].second);
    }
}

void printRelation(struct Relation *r) {
    if (r->size == 0) {
        printf("{}\n");
        return;
    }
    printf("{");
    for (unsigned i = 0; i < r->size - 1; i++) {
        printf("(%d %d), ", r->pairs[i].first, r->pairs[i].second);
    }
    printf("(%d %d)}\n", r->pairs[r->size-1].first, r->pairs[r->size-1].second);
}