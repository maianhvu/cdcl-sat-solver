//
//  CdclSolver.c
//  satcdcl
//
//  Created by Mai Anh Vu on 28/12/18.
//  Copyright © 2018 Anh Vu Mai. All rights reserved.
//

#include "CdclSolver.h"
#include <stdlib.h>
#include "MostCommonVariablePicker.h"
#include "UIPConflictAnalyzer.h"
#include <stdio.h>

// Constants
void (*VariablePicker_pick_variable)(const Formula, const Model, int *, ModelValue *) = &MostCommonVariablePicker_pick_variable;
int *(*ConflictAnalyzer_analyze_conflict)(const Formula, Model, int decision_level, int conflict_clause_idx, int *beta, size_t *learned_clause_size) = &UIPConflictAnalyzer_analyze_conflict;

// Returns -1 when no conflict. Returns conflicted clause index otherwise.
int CdclSolver_unit_propagate(const Formula f, Model m, int decision_level);

Model CdclSolver_solve(Formula f) {
    Model m = Model_create(f->max_variables);
    if (CdclSolver_unit_propagate(f, m, 0) != -1) { return NULL; /* unsat */ }
    
    int decision_level = 0;
    int variable = 0;
    ModelValue value = kModelValueUnassigned;
    int conflict_clause_idx = -1;
    
    while (m->num_assigned < m->num_variables) {
#if LOG
        printf("Now has %d variables assigned out of %d variables\n", m->num_assigned, m->num_variables);
#endif
        (*VariablePicker_pick_variable)(f, m, &variable, &value);
        
        decision_level += 1;
        Model_assign(m, variable, value, decision_level, -1);
        conflict_clause_idx = CdclSolver_unit_propagate(f, m, decision_level);
        
        if (conflict_clause_idx != -1) {
            int beta = -1;
            size_t learned_clause_size = 0;
            int *learned_clause_literals = (*ConflictAnalyzer_analyze_conflict)(f, m, decision_level, conflict_clause_idx, &beta, &learned_clause_size);
            
            if (beta < 0 || learned_clause_literals == NULL || learned_clause_size == 0) {
                return NULL; // unsat
            }
            
            // Backtrack
            decision_level = beta;
#if LOG
            printf("Backtrack to %d\n", decision_level);
#endif
            
            // Add learned clause
            ConstIntArray learned_clause_array = { .buffer = learned_clause_literals, .count = learned_clause_size };
            Clause learned_clause = Clause_form(learned_clause_array);
            Formula_add_clause(f, learned_clause);
            free(learned_clause_literals);
            
            Model_backtrack(m, decision_level);
        }
    }
    
    return m;
}

int CdclSolver_unit_propagate(const Formula f, Model m, int decision_level) {
    int unit_literal_idx = -1;
    for (int didPropagate = 1; didPropagate;) {
        didPropagate = 0;
        for (int clause_idx = 0; clause_idx < Formula_count_clauses(f); ++clause_idx) {
            Clause clause = Formula_get_clause(f, clause_idx);
            ModelValue eval = Model_evaluate(m, clause, &unit_literal_idx);
            
            // Conflict at current clause
            if (eval == kModelValueFalse) {
#if LOG
                printf("Conflicted at clause %d\n", clause_idx);
#endif
                return clause_idx;
            }
            
            if (eval == kModelValueUnassigned && unit_literal_idx != -1) {
                int literal = clause.literals.buffer[unit_literal_idx];
                ModelValue value = kModelValueTrue;
                if (literal < 0) { value = -value; }
                Model_assign(m, abs(literal), value, decision_level, clause_idx);
#if LOG
                printf("Propagated %d at %d with antecedent %d\n", literal, decision_level, clause_idx);
#endif
                didPropagate = 1;
                break;
            }
        }
    }
    return -1;
}


