//
//  ConstIntArray.c
//  satcdcl
//
//  Created by Mai Anh Vu on 28/12/18.
//  Copyright © 2018 Anh Vu Mai. All rights reserved.
//

#include "ConstIntArray.h"

ConstIntArray ConstIntArray_from_array(IntArray array) {
    ConstIntArray const_array = {
        .buffer = array->buffer,
        .count = array->count
    };
    return const_array;
}

ConstIntArray ConstIntArray_form(const int *buffer, const size_t count) {
    ConstIntArray const_array = {
        .buffer = buffer,
        .count = count
    };
    return const_array;
}

void IntArray_append_const_array(IntArray array, ConstIntArray const_array) {
    IntArray_append_values(array, const_array.buffer, const_array.count);
}
