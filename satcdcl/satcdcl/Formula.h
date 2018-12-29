//
//  Formula.h
//  satcdcl
//
//  Created by Mai Anh Vu on 28/12/18.
//  Copyright © 2018 Anh Vu Mai. All rights reserved.
//

#ifndef Formula_h
#define Formula_h

#include "IntArray.h"
#include "Clause.h"

typedef struct Formula {
    IntArray literals;
    IntArray clause_bounds;

    int max_variables;
    int max_clauses;
} *Formula;

Formula Formula_create(int num_variables, int num_clauses);

void Formula_add_clause(Formula formula, Clause clause);
size_t Formula_count_clauses(Formula formula);
Clause Formula_get_clause(Formula formula, int clause_idx);

void Formula_free(Formula formula);

#endif /* Formula_h */
