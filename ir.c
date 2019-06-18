#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "ir.h"
#include "linkedlist.h"
#include "instructions.h"
#include "util.h"


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
    Ir *ir = (Ir *)minic_malloc(sizeof(Ir));
    ir->kind = IR_OP;
    ir->repr = "HALT";
    ir->value.op = HALT;
    ll_append(program, ir);
    return program;
}


Ir *ir_new_label(const char *label) {
    Ir *ir = minic_malloc(sizeof(struct Ir));
    ir->kind = IR_LABEL;
    ir->repr = make_str(label);
    return ir;
}


struct Ir *ir_new_jump_inst(inst_t instruction, const char *label) {
    char *tmp_str;
    const char *inst_name;
    struct Ir *ir = minic_malloc(sizeof(struct Ir));
    inst_name = inst_names[instruction];

    /* allocate enough memory for \0 and a space between the two strings */
    tmp_str = minic_malloc(strlen(inst_name) + strlen(label) + 2);

    sprintf(tmp_str, "%s %s", inst_name, label);
    ir->kind = IR_OP;
    ir->repr = tmp_str;
    return ir;
}
