//
//  Clause.h
//  satcdcl
//
//  Created by Mai Anh Vu on 28/12/18.
//  Copyright © 2018 Anh Vu Mai. All rights reserved.
//

#ifndef Clause_h
#define Clause_h

#include "ConstIntArray.h"

typedef struct Clause {
    ConstIntArray literals;
} Clause;

Clause Clause_form(ConstIntArray literals);
int *Clause_resolve(const int *clause1_literals, const size_t clause1_count, Clause clause2, size_t *out_clause_size);

void Clause_print(Clause clause);
void Clause_print_literals(const int *literals, size_t count);

#endif /* Clause_h */
