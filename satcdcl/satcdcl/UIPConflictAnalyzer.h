//
//  UIPConflictAnalyzer.h
//  satcdcl
//
//  Created by Mai Anh Vu on 28/12/18.
//  Copyright © 2018 Anh Vu Mai. All rights reserved.
//

#ifndef UIPConflictAnalyzer_h
#define UIPConflictAnalyzer_h

#include "Formula.h"
#include "Model.h"

int *UIPConflictAnalyzer_analyze_conflict(const Formula, Model, int decision_level, int conflict_clause_idx, int *beta, size_t *learned_clause_size);

#endif /* UIPConflictAnalyzer_h */
