//
//  Formula.c
//  satcdcl
//
//  Created by Mai Anh Vu on 28/12/18.
//  Copyright © 2018 Anh Vu Mai. All rights reserved.
//

#include "Formula.h"
#include <stdlib.h>

Formula Formula_create(int num_variables, int num_clauses) {
    Formula f = (Formula) malloc(sizeof(struct Formula));
    f->literals = IntArray_create(4);
    f->clause_bounds = IntArray_create(num_clauses);
    f->max_variables = num_variables;
    f->max_clauses = num_clauses;
    return f;
}

void Formula_add_clause(Formula formula, Clause clause) {
    IntArray_append_const_array(formula->literals, clause.literals);
    IntArray_append(formula->clause_bounds, (int) formula->literals->count);
}

size_t Formula_count_clauses(Formula formula) {
    return formula->clause_bounds->count;
}

Clause Formula_get_clause(Formula formula, int clause_idx) {
    int buffer_start = clause_idx == 0 ? 0 : formula->clause_bounds->buffer[clause_idx - 1];
    int buffer_end = formula->clause_bounds->buffer[clause_idx];
    ConstIntArray literals = ConstIntArray_form(formula->literals->buffer + buffer_start,
                                                (size_t) buffer_end - buffer_start);
    return Clause_form(literals);
}

void Formula_free(Formula formula) {
    IntArray_free(formula->literals);
    IntArray_free(formula->clause_bounds);
    free(formula);
}
