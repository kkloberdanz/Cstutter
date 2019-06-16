#ifndef IR_H
#define IR_H


#include <stdio.h>
#include "linkedlist.h"
#include "instructions.h"


typedef enum ir_kind {
    IR_OP,
    IR_NUMBER
} ir_kind;


typedef struct Ir {
    ir_kind kind;
    char *repr;
    union {
        inst_t op;
        char *number;
    } value;
} Ir;


void ir_print_program(FILE *output, const linkedlist *program);
linkedlist *ir_halt_program(linkedlist* program);


#endif /* IR_H */
