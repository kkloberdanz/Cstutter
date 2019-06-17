#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "ir.h"
#include "linkedlist.h"
#include "instructions.h"


static char *make_str(const char *str) {
    char *dst = malloc(strlen(str) + 1);
    strcpy(dst, str);
    return dst;
}


void ir_print_program(FILE *output, const linkedlist *program) {
    while (program) {
        Ir *ir = (Ir *)program->value;
        if (ir->value.op == PUSH) {
            fprintf(output, "%s ", ir->repr);
        } else {
            fprintf(output, "%s\n", ir->repr);
        }
        program = program->next;
    }
}


linkedlist *ir_halt_program(linkedlist* program) {
    Ir *ir = (Ir *)malloc(sizeof(Ir));
    ir->kind = IR_OP;
    ir->repr = "HALT";
    ir->value.op = HALT;
    ll_append(program, ir);
    return program;
}


Ir *ir_new_label(const char *label) {
    Ir *ir = malloc(sizeof(struct Ir));
    if (ir == NULL) {
        fprintf(stderr, "out of memory\n");
    }
    ir->kind = IR_LABEL;
    ir->repr = make_str(label);
    return ir;
}
