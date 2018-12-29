//
//  MostCommonVariablePicker.c
//  satcdcl
//
//  Created by Mai Anh Vu on 28/12/18.
//  Copyright © 2018 Anh Vu Mai. All rights reserved.
//

#include "MostCommonVariablePicker.h"
#include <stdlib.h>

// MARK: - Private Types
typedef struct AppearanceMap {
    int *buffer;
    int *indexes_map;
    size_t count;
} *AppearanceMap;

// MARK: - Private Interface
AppearanceMap AppearanceMap_create(int num_unassigned);
void AppearanceMap_register(AppearanceMap map, int variable);
void AppearanceMap_record_literal(AppearanceMap map, int literal);
void AppearanceMap_get_best_assignment(AppearanceMap map, int *variable, ModelValue *value);
void AppearanceMap_free(AppearanceMap map);
int AppearanceMap_get_index(AppearanceMap map, int variable);

// MARK: - Public Implementation
void MostCommonVariablePicker_pick_variable(const Formula f, const Model m, int *var, ModelValue *val) {
    AppearanceMap map = AppearanceMap_create(m->num_variables - m->num_assigned);
    for (int i = 0; i < m->num_variables; ++i) {
        if (m->assignments[i].value != kModelValueUnassigned) { continue; }
        AppearanceMap_register(map, i + 1);
    }
    
    // Registration done!
    // TODO: Improve performance by caching unassignments
    for (int clause_idx = 0; clause_idx < Formula_count_clauses(f); ++clause_idx) {
        Clause clause = Formula_get_clause(f, clause_idx);
        if (Model_evaluate(m, clause, NULL) != kModelValueUnassigned) { continue; }
        for (int literal_idx = 0; literal_idx < clause.literals.count; ++literal_idx) {
            AppearanceMap_record_literal(map, clause.literals.buffer[literal_idx]);
        }
    }
    
    AppearanceMap_get_best_assignment(map, var, val);
    AppearanceMap_free(map);
}

// MARK: - Private Implementation
AppearanceMap AppearanceMap_create(int num_unassigned) {
    AppearanceMap map = (AppearanceMap) malloc(sizeof(struct AppearanceMap));
    map->buffer = (int *) malloc(sizeof(int) * num_unassigned * 2);
    for (int i = 0; i < num_unassigned * 2; ++i) { map->buffer[i] = 0; }
    map->indexes_map = (int *) malloc(sizeof(int) * num_unassigned);
    map->count = 0;
    return map;
}

void AppearanceMap_register(AppearanceMap map, int variable) {
    map->indexes_map[map->count++] = variable;
}

int AppearanceMap_get_index(AppearanceMap map, int variable) {
    // Binary search for variable
    int low = 0;
    int high = (int) map->count - 1;
    int mid = -1;
    while (low <= high) {
        mid = low + (high - low) / 2;
        if (map->indexes_map[mid] == variable) { return mid; }
        if (map->indexes_map[mid] > variable) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return (mid == -1 || map->indexes_map[mid] != variable) ? -1 : mid;
}

void AppearanceMap_record_literal(AppearanceMap map, int literal) {
    // Binary search for variable
    int variable = abs(literal);
    int variable_idx = AppearanceMap_get_index(map, variable);
    if (variable_idx == -1) { return; }
    
    int record_idx = variable_idx * 2;
    if (literal < 0) { record_idx += 1; }
    map->buffer[record_idx] += 1;
}

void AppearanceMap_get_best_assignment(AppearanceMap map, int *variable, ModelValue *value) {
    int max_variable_idx = 0;
    int max_count = map->buffer[0] + map->buffer[1];
    
    for (int i = 1; i < map->count; ++i) {
        int count = map->buffer[i * 2] + map->buffer[i * 2 + 1];
        if (count <= max_count) { continue; }
        max_variable_idx = i;
        max_count = count;
    }
    
    *variable = map->indexes_map[max_variable_idx];
    *value = map->buffer[max_variable_idx * 2] < map->buffer[max_variable_idx * 2 + 1] ? kModelValueFalse : kModelValueTrue;
}

void AppearanceMap_free(AppearanceMap map) {
    free(map->buffer);
    free(map->indexes_map);
    free(map);
}
