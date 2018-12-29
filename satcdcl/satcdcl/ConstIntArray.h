//
//  ConstIntArray.h
//  satcdcl
//
//  Created by Mai Anh Vu on 28/12/18.
//  Copyright © 2018 Anh Vu Mai. All rights reserved.
//

#ifndef ConstIntArray_h
#define ConstIntArray_h

#include <stddef.h>
#include "IntArray.h"

typedef struct ConstIntArray {
    const int *buffer;
    const size_t count;
} ConstIntArray;

ConstIntArray ConstIntArray_from_array(IntArray array);
ConstIntArray ConstIntArray_form(const int *buffer, const size_t count);

void IntArray_append_const_array(IntArray array, ConstIntArray const_array);

#endif /* ConstIntArray_h */
