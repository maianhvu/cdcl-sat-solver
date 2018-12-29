//
//  IntArray.h
//  satcdcl
//
//  Created by Mai Anh Vu on 28/12/18.
//  Copyright © 2018 Anh Vu Mai. All rights reserved.
//

#ifndef IntArray_h
#define IntArray_h

#include <stddef.h>

typedef struct IntArray {
    int *buffer;
    size_t count;
    size_t buffer_size;
} *IntArray;

IntArray IntArray_create(size_t initial_size);

void IntArray_append(IntArray array, int value);
void IntArray_append_values(IntArray array, const int *buffer, const size_t count);
void IntArray_append_array(IntArray array, IntArray other);
void IntArray_clear_retain_capacity(IntArray array);
int IntArray_pop_last(IntArray array, int *out_value);
int IntArray_get_last(IntArray array, int *out_value);

void IntArray_free(IntArray array);

#endif /* IntArray_h */
