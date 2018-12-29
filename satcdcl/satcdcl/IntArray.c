//
//  IntArray.c
//  satcdcl
//
//  Created by Mai Anh Vu on 28/12/18.
//  Copyright © 2018 Anh Vu Mai. All rights reserved.
//

#include "IntArray.h"
#include <stdlib.h>
#include <string.h>

// MARK: - Constants
const size_t IntArray_DEFAULT_GROW_FACTOR = 2;

// MARK: - Private Interface
void IntArray_buffer_grow(IntArray array, size_t factor);

// MARK: - Public Implementation
IntArray IntArray_create(size_t initial_size) {
    IntArray array = (IntArray) malloc(sizeof(struct IntArray));
    array->buffer = (int *) malloc(sizeof(int) * initial_size);
    array->count = 0;
    array->buffer_size = initial_size;
    return array;
}

void IntArray_append(IntArray array, int value) {
    if (array->count >= array->buffer_size) {
        IntArray_buffer_grow(array, IntArray_DEFAULT_GROW_FACTOR);
    }
    array->buffer[array->count++] = value;
}

void IntArray_append_values(IntArray array, const int *buffer, const size_t count) {
    while (array->count + count > array->buffer_size) {
        IntArray_buffer_grow(array, IntArray_DEFAULT_GROW_FACTOR);
    }
    memcpy(array->buffer + array->count, buffer, sizeof(int) * count);
    array->count += count;
}

void IntArray_append_array(IntArray array, IntArray other) {
    IntArray_append_values(array, other->buffer, other->count);
}

void IntArray_clear_retain_capacity(IntArray array) {
    array->count = 0;
}

int IntArray_pop_last(IntArray array, int *out_value) {
    if (array->count <= 0) { return 0; }
    int value = array->buffer[array->count--];
    if (out_value != NULL) { *out_value = value; }
    return 1;
}

int IntArray_get_last(IntArray array, int *out_value) {
    if (array->count <= 0) { return 0; }
    *out_value = array->buffer[array->count - 1];
    return 1;
}

void IntArray_free(IntArray array) {
    free(array->buffer);
    free(array);
}

// MARK: - Private Implementation
void IntArray_buffer_grow(IntArray array, size_t factor) {
    size_t new_buffer_size = array->buffer_size * factor;
    int *new_buffer = (int *) malloc(sizeof(int) * new_buffer_size);
    memcpy(new_buffer, array->buffer, sizeof(int) * array->count);
    free(array->buffer);
    array->buffer = new_buffer;
    array->buffer_size = new_buffer_size;
}
