#ifndef IR_H
#define IR_H


#include <stdio.h>
#include "linkedlist.h"
#include "instructions.h"


typedef enum ir_kind {
    IR_OP,
    IR_JMP,
    IR_END,
    IR_NUMBER,
    IR_LABEL,
    IR_SAVE,
    IR_LOAD,
    IR_PUSH
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
struct Ir *ir_new_jump_inst(inst_t instruction, const char *label);
Ir *ir_new_label(const char *label);
void ir_free_list(linkedlist *ll);
struct Ir *ir_new_save();
struct Ir *ir_new_load();
struct Ir *ir_new_push_immediate(int immediate);

#endif /* IR_H */
