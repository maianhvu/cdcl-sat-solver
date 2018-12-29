//
//  Clause.c
//  satcdcl
//
//  Created by Mai Anh Vu on 28/12/18.
//  Copyright © 2018 Anh Vu Mai. All rights reserved.
//

#include "Clause.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Clause Clause_form(ConstIntArray literals) {
    Clause clause = {
        .literals = literals
    };
    return clause;
}

int Clause_compare_ints(const void *a, const void *b) {
    int i1 = *(const int *)a;
    int i2 = *(const int *)b;
    int ai1 = abs(i1);
    int ai2 = abs(i2);
    if (ai1 != ai2) { return ai1 < ai2 ? -1 : 1; }
    if (i1 > i2) { return -1; }
    if (i1 < i2) { return 1; }
    return 0;
}

int *Clause_resolve(const int *clause1_literals, const size_t clause1_count, Clause clause2, size_t *out_clause_size) {
    size_t total_count = clause1_count + clause2.literals.count;
    int all_literals[total_count];
    memcpy(all_literals, clause1_literals, sizeof(int) * clause1_count);
    memcpy(all_literals + (int) clause1_count, clause2.literals.buffer, sizeof(int) * clause2.literals.count);
    qsort(all_literals, total_count, sizeof(int), Clause_compare_ints);
    IntArray out_literals = IntArray_create(clause1_count);
    IntArray ignored_variables = IntArray_create(2);
    int retval = 0;
    int last_ignored = 0;
    int last_literal = 0;
    
    for (int i = 0; i < total_count; ++i) {
        // Check for ignored literals
        int literal = all_literals[i];
        retval = IntArray_get_last(ignored_variables, &last_ignored);
        if (retval && last_ignored == abs(literal)) { continue; }
        
        // Not ignored, and out literals are empty, so add new literal
        if (out_literals->count == 0) {
            IntArray_append(out_literals, literal);
            continue;
        }
        
        IntArray_get_last(out_literals, &last_literal);
        if (last_literal + literal == 0) {
            IntArray_pop_last(out_literals, NULL);
            IntArray_append(ignored_variables, abs(literal));
        } else if (last_literal != literal) {
            // Different literals now
            IntArray_append(out_literals, literal);
        }
    }
    
    *out_clause_size = out_literals->count;
    return out_literals->buffer;
}

void Clause_print_literals(const int *literals, size_t count) {
    for (int i = 0; i < count; ++i) {
        printf("%d", literals[i]);
        if (i + 1 != count) { printf(" "); }
    }
}

void Clause_print(Clause clause) {
    Clause_print_literals(clause.literals.buffer, clause.literals.count);
}
