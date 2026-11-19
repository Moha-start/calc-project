#ifndef CALC_TYPES_H
#define CALC_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef unsigned int uint;

typedef enum Type {
    NUMBER = 0,
    OPERATION,
    VARIABLE,
    DELIMITER,
    SINGLEOPERATION,
} Type; 

typedef struct {
    char **word;
} Calc;

#endif /* CALC_TYPES_H */