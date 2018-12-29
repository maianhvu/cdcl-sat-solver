//
//  main.c
//  satcdcl
//
//  Created by Mai Anh Vu on 28/12/18.
//  Copyright © 2018 Anh Vu Mai. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Formula.h"
#include "CdclSolver.h"

const char DIRECTIVE_COMMENT = 'c';
const char DIRECTIVE_INIT = 'p';

Formula parse_formula(FILE *input);

int main(int argc, const char * argv[]) {
    FILE *file;
    if (argc >= 2) {
        file = fopen(argv[1], "r");
    } else {
        file = stdin;
    }
    if (file == NULL) { printf("Invalid input.\n"); return 1; }
    
    Formula f = parse_formula(file);

    if (f == NULL) {
        printf("Invalid input.\n");
        fclose(file);
        return 1;
    }
    
    Model m = CdclSolver_solve(f);
    
    if (m != NULL) {
        printf("SAT\n");
        for (int i = 0; i < m->num_variables; ++i) {
            switch (m->assignments[i].value) {
                case kModelValueTrue: printf("1"); break;
                case kModelValueFalse: printf("0"); break;
                default: printf("u"); break;
            }
            if (i + 1 == m->num_variables) { printf("\n"); }
            else { printf(" "); }
        }
        Model_free(m);
    } else {
        printf("UNSAT\n");
    }
    
    Formula_free(f);
    fclose(file);
    
    return 0;
}

Formula parse_formula(FILE *input) {
    char *line;
    size_t line_buffer;
    ssize_t read_size;
    
    char directive;
    char format[4];
    int num_variables = 0;
    int num_clauses = 0;
    Formula f = NULL;
    IntArray clause_buffer = IntArray_create(3);
    const char delim[2] = " ";
    char *token;
    int literal;
    
    while ((read_size = getline(&line, &line_buffer, input)) != -1) {
        // Stop when all clauses are read
        if (num_clauses > 0 && f != NULL && Formula_count_clauses(f) >= num_clauses) { break; }
        
        // Read directive
        if (sscanf(line, "%c", &directive) != 1) { continue; }
        
        switch (directive) {
            case DIRECTIVE_COMMENT:
                continue;
            case DIRECTIVE_INIT:
                if (sscanf(line, "%c %s %d %d", &directive, format, &num_variables, &num_clauses) != 4) { continue; }
                if (num_variables <= 0 || num_clauses <= 0) { continue; }
                f = Formula_create(num_variables, num_clauses);
                continue;
            default:
                if (num_variables <= 0 || num_clauses <= 0) { continue; }
                token = strtok(line, delim);
                
                while (token != NULL) {
                    literal = (int) strtol(token, (char **) NULL, 10);
                    if (literal == 0) {
                        if (clause_buffer->count > 0) {
                            ConstIntArray literals = ConstIntArray_from_array(clause_buffer);
                            Clause clause = Clause_form(literals);
                            Formula_add_clause(f, clause);
                            IntArray_clear_retain_capacity(clause_buffer);
                        }
                        break;
                    }
                    IntArray_append(clause_buffer, literal);
                    token = strtok(NULL, delim);
                }
                if (clause_buffer->count > 0) {
                    ConstIntArray literals = ConstIntArray_from_array(clause_buffer);
                    Clause clause = Clause_form(literals);
                    Formula_add_clause(f, clause);
                    
                    IntArray_clear_retain_capacity(clause_buffer);
                }
                break;
        }
    }
    
    IntArray_free(clause_buffer);
    
    return f;
}
