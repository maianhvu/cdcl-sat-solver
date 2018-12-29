//
//  UIPConflictAnalyzer.c
//  satcdcl
//
//  Created by Mai Anh Vu on 28/12/18.
//  Copyright © 2018 Anh Vu Mai. All rights reserved.
//

#include "UIPConflictAnalyzer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

int *UIPConflictAnalyzer_analyze_conflict(const Formula f, Model m, int decision_level, int conflict_clause_idx, int *beta, size_t *learned_clause_size) {
    int *clause_literals = f->literals->buffer;
    size_t clause_count = (size_t) f->clause_bounds->buffer[conflict_clause_idx];
    if (conflict_clause_idx > 0) {
        clause_literals += f->clause_bounds->buffer[conflict_clause_idx - 1];
        clause_count -= f->clause_bounds->buffer[conflict_clause_idx - 1];
    }
    
#if LOG
    printf("Conflicted clause is ");
    Clause_print_literals(clause_literals, clause_count);
    printf("\n");
#endif
    
    int is_clause_original = 1;
    
    for (int did_modify_clause = 1; did_modify_clause;) {
        did_modify_clause = 0;
        for (int literal_idx = 0; literal_idx < clause_count; ++literal_idx) {
            int literal = clause_literals[literal_idx];
            Assignment assignment = m->assignments[abs(literal) - 1];
            if (assignment.value == kModelValueUnassigned || assignment.decision_level != decision_level || assignment.antecedent_idx == -1) { continue; }
            Clause antecedent = Formula_get_clause(f, assignment.antecedent_idx);
            size_t resolved_clause_size = 0;
            int *resolved_clause = Clause_resolve(clause_literals, clause_count, antecedent, &resolved_clause_size);
#if LOG
            printf("Resolve ");
            Clause_print_literals(clause_literals, clause_count);
            printf(" with ");
            Clause_print(antecedent);
            printf(" gives ");
            Clause_print_literals(resolved_clause, resolved_clause_size);
            printf("\n");
#endif
            if (is_clause_original) {
                is_clause_original = 0;
            } else {
                free(clause_literals);
            }
            clause_literals = resolved_clause;
            clause_count = resolved_clause_size;
            did_modify_clause = 1;
            break;
        }
    }
    
    if (is_clause_original) { // No resolution performed
        *beta = -1;
        return NULL;
    }
    
#if LOG
    printf("Learned clause ");
    Clause_print_literals(clause_literals, clause_count);
    printf("\n");
#endif
    
    // Figure out beta
    int min_decision_level = INT_MAX;
    for (int literal_idx = 0; literal_idx < clause_count; ++literal_idx) {
        Assignment assignment = m->assignments[abs(clause_literals[literal_idx]) - 1];
        if (assignment.value == kModelValueUnassigned || assignment.decision_level < 0) { continue; }
        if (assignment.decision_level >= min_decision_level) { continue; }
        min_decision_level = assignment.decision_level;
    }
    if (min_decision_level == INT_MAX) { *beta = -1; return NULL; }
    *beta = min_decision_level;
    
    *learned_clause_size = clause_count;
    return clause_literals;
}
