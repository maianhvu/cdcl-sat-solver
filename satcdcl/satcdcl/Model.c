//
//  Model.c
//  satcdcl
//
//  Created by Mai Anh Vu on 28/12/18.
//  Copyright © 2018 Anh Vu Mai. All rights reserved.
//

#include "Model.h"
#include <stdlib.h>

Assignment ModelAssignment_create_unassigned(void) {
    Assignment unassigned = {
        .value = kModelValueUnassigned,
        .decision_level = -1,
        .antecedent_idx = -1
    };
    return unassigned;
}

Model Model_create(int num_variables) {
    Model model = (Model) malloc(sizeof(struct Model));
    model->assignments = (Assignment *) malloc(sizeof(Assignment) * num_variables);
    for (int i = 0; i < num_variables; ++i) {
        model->assignments[i] = ModelAssignment_create_unassigned();
    }
    model->num_variables = num_variables;
    model->num_assigned = 0;
    return model;
}

Assignment Model_assign(Model model, int variable, ModelValue value, int decision_level, int antecedent_idx) {
    int index = variable - 1;
    ModelValue oldValue = model->assignments[index].value;
    if (oldValue != value) {
        model->assignments[index].value = value;
        model->assignments[index].decision_level = decision_level;
        model->assignments[index].antecedent_idx = antecedent_idx;
        
        if (oldValue == kModelValueUnassigned) {
            ++model->num_assigned;
        } else if (value == kModelValueUnassigned) {
            --model->num_assigned;
        }
    }
    return model->assignments[index];
}

ModelValue Model_evaluate(Model model, Clause clause, int *unit_literal_idx) {
    if (unit_literal_idx != NULL) { *unit_literal_idx = -1; }
    int unassigned_count = 0;
    
    for (int literal_idx = 0; literal_idx < clause.literals.count; ++literal_idx) {
        int literal = clause.literals.buffer[literal_idx];
        int variable = abs(literal);
        ModelValue value = model->assignments[variable - 1].value;
        if (literal < 0) { value = -value; }
        
        if (value == kModelValueTrue) { return kModelValueTrue; }
        
        if (value == kModelValueUnassigned) {
            ++unassigned_count;
            if (unit_literal_idx != NULL) {
                if (unassigned_count == 1) { *unit_literal_idx = literal_idx; }
                else if (unassigned_count == 2) { *unit_literal_idx = -1; }
            }
        }
    }
    
    return unassigned_count ? kModelValueUnassigned : kModelValueFalse;
}

void Model_backtrack(Model model, int decision_level) {
    model->num_assigned = 0;
    for (int i = 0; i < model->num_variables; ++i) {
        if (model->assignments[i].value == kModelValueUnassigned) { continue; }
        if (model->assignments[i].decision_level <= decision_level) {
            ++model->num_assigned;
            continue;
        }
        model->assignments[i] = ModelAssignment_create_unassigned();
    }
}

void Model_free(Model model) {
    free(model->assignments);
    free(model);
}
