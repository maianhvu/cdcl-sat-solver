//
//  MostCommonVariablePicker.h
//  satcdcl
//
//  Created by Mai Anh Vu on 28/12/18.
//  Copyright © 2018 Anh Vu Mai. All rights reserved.
//

#ifndef MostCommonVariablePicker_h
#define MostCommonVariablePicker_h

#include "Formula.h"
#include "Model.h"

void MostCommonVariablePicker_pick_variable(const Formula f, const Model m, int *var, ModelValue *val);

#endif /* MostCommonVariablePicker_h */
