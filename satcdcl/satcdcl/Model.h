//
//  Model.h
//  satcdcl
//
//  Created by Mai Anh Vu on 28/12/18.
//  Copyright © 2018 Anh Vu Mai. All rights reserved.
//

#ifndef Model_h
#define Model_h

#include "Clause.h"

typedef int ModelValue;

static const ModelValue kModelValueFalse = -1;
static const ModelValue kModelValueTrue = 1;
static const ModelValue kModelValueUnassigned = 0;

typedef struct Assignment {
    ModelValue value;
    int decision_level;
    int antecedent_idx;
} Assignment;
Assignment ModelAssignment_create_unassigned(void);

typedef struct Model {
    Assignment *assignments;
    int num_variables;
    int num_assigned;
} *Model;

Model Model_create(int num_variables);
Assignment Model_assign(Model model, int variable, ModelValue value, int decision_level, int antecedent_idx);
ModelValue Model_evaluate(Model model, Clause clause, int *unit_literal_idx);
void Model_backtrack(Model model, int decision_level);
void Model_free(Model model);

#endif /* Model_h */
