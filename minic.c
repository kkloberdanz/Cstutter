/*
 * Author: Kyle Kloberdanz
 * Project Start Date: 27 Nov 2018
 * License: GNU GPLv3 (see LICENSE.txt)
 *     This file is part of minic.
 *
 *     minic is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     minic is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with minic.  If not, see <https://www.gnu.org/licenses/>.
 * File: minic.c
 */


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#include "minic.h"
#include "linkedlist.h"
#include "ir.h"
#include "instructions.h"


char token_string[MAX_TOKEN_SIZE+1];


/* constructors */
MinicObject *make_number_obj(char *n) {
    MinicObject *obj;
    int len_n;
    if ((obj = (MinicObject *)malloc(sizeof(MinicObject))) == NULL) {
        fprintf(stderr, "failed to allocate memory");
        return NULL;
    }
    obj->type = NUMBER_TYPE;

    len_n = strlen(n);
    obj->value.number_value = (char *)calloc(len_n + 1, sizeof(char));
    strcpy(obj->value.number_value, n);
    return obj;
}


MinicObject *make_string_obj(char *str) {
    MinicObject *obj;
    if ((obj = (MinicObject *)malloc(sizeof(MinicObject))) == NULL) {
        fprintf(stderr, "failed to allocate memory");
        return NULL;
    }
    obj->type = STRING_TYPE;
    obj->value.string_value = str;
    return obj;
}


MinicObject *make_id_obj(char *symb) {
    MinicObject *obj;
    if ((obj = (MinicObject *)malloc(sizeof(MinicObject))) == NULL) {
        fprintf(stderr, "failed to allocate memory");
        return NULL;
    }
    obj->type = VOID_TYPE;
    obj->value.symbol = symb;
    return obj;
}


char *make_string(char *str) {
    char *s = (char *)malloc(strlen(str) + 1);
    if (s == NULL) {
        fprintf(stderr, "%s\n", "out of memory");
        return NULL;
    }
    strcpy(s, str);
    return s;
}


ASTNode *make_ast_node(const ASTkind kind,
                       MinicObject *obj,
                       const Operator op,
                       ASTNode *left_node,
                       ASTNode *condition,
                       ASTNode *right_node) {

    ASTNode *node;
    if ((node = (ASTNode *)malloc(sizeof(ASTNode))) == NULL) {
        fprintf(stderr, "failed to allocate memory");
        return NULL;
    }

    node->kind = kind;

    switch (kind) {
        case LEAF:
            node->obj = obj;
            node->op = op;
            node->left = NULL;
            node->condition = NULL;
            node->right = NULL;
            break;

        case CONDITIONAL:
            node->obj = NULL;
            node->op = OP_NIL;
            node->left = left_node; /* the true path */
            node->condition = condition; /* the expr to evaluate */
            node->right = right_node; /* the false path */
            break;

        case OPERATOR:
            node->obj = NULL;
            node->op = op;
            node->left = left_node;
            node->condition = NULL;
            node->right = right_node;
            break;

        default:
            fprintf(stderr, "error: invalid ASTkind %d\n", kind);
            destroy_ast_node(node);
            node = NULL;
    }

    return node;
}


ASTNode *make_leaf_node(MinicObject *obj) {
    ASTNode *node = make_ast_node(LEAF, obj, OP_NIL, NULL, NULL, NULL);
    return node;
}


ASTNode *make_operator_node(Operator op, ASTNode *left, ASTNode *right) {
    ASTNode *node = make_ast_node(OPERATOR, NULL, op, left, NULL, right);
    return node;
}


ASTNode *make_conditional_node(ASTNode *condition, ASTNode *left, ASTNode *right) {
    ASTNode *node = make_ast_node(CONDITIONAL, NULL, OP_NIL, left, condition, right);
    return node;
}


/* destructors */
void destroy_obj(MinicObject *obj) {
    free(obj->value.number_value);
    free(obj);
}


void destroy_ast_node(ASTNode *node) {
    if (node) {
        if (node->obj) {
            destroy_obj(node->obj);
            node->obj = NULL;
        }

        /* recursive call */
        if (node->condition) {
            destroy_ast_node(node->condition);
            node->condition = NULL;
        }

        /* recursive call */
        if (node->left) {
            destroy_ast_node(node->left);
            node->left = NULL;
        }

        /* recursive call */
        if (node->right) {
            destroy_ast_node(node->right);
            node->right = NULL;
        }

        free(node);
        node = NULL;
    }
}


static Ir *get_op_ir(const Operator op) {
    Ir *ir = (Ir *)malloc(sizeof(Ir));
    if (ir == NULL) {
        fprintf(stderr, "%s\n", "failed to allocate Ir object");
        exit(EXIT_FAILURE);
    }
    ir->kind = IR_OP;
    switch (op) {
        case OP_NIL:
            ir->repr = "NOP";
            ir->value.op = NOP;
            break;

        case OP_PLUS:
            ir->repr = "ADD";
            ir->value.op = ADD;
            break;

        case OP_MINUS:
            ir->repr = "SUB";
            ir->value.op = SUB;
            break;

        case OP_TIMES:
            ir->repr = "MUL";
            ir->value.op = MUL;
            break;

        case OP_DIVIDE:
            ir->repr = "DIV";
            ir->value.op = DIV;
            break;

        case OP_GE:
            ir->repr = "GE";
            ir->value.op = GE;
            break;

        case OP_GT:
            ir->repr = "GT";
            ir->value.op = GT;
            break;

        case OP_EQ:
            ir->repr = "EQ";
            ir->value.op = EQ;
            break;

        case OP_NE:
            ir->repr = "NE";
            ir->value.op = NE;
            break;

        case OP_LT:
            ir->repr = "LT";
            ir->value.op = LT;
            break;

        case OP_LE:
            ir->repr = "LE";
            ir->value.op = LE;
            break;

        case OP_NOT:
            ir->repr = "NOT";
            ir->value.op = NOT;
            break;
    }
    return ir;
}


char *get_op_val(char *str, const MinicObject *obj) {
    switch (obj->type) {
        case NUMBER_TYPE:
            sprintf(str, "%s", obj->value.number_value);
            break;

        default:
            fprintf(stderr, "unhandled case: %d\n", obj->type);
            return NULL;
    }
    return str;
}


static Ir *get_ir_node(const ASTNode *ast) {
    Ir *ir_node = NULL;
    switch (ast->kind) {
        case LEAF:
        {
            char *value = ast->obj->value.number_value;
            ir_node = (Ir *)malloc(sizeof(Ir));
            ir_node->kind = IR_NUMBER;
            ir_node->value.number = value;
            ir_node->repr = value;
            break;
        }

        default:
            fprintf(stderr, "incorrect ast kind: %d\n", ast->kind);
            exit(EXIT_FAILURE);
    }
    if (ir_node == NULL) {
        fprintf(stderr, "%s\n", "failed to initialize ir_node");
        exit(EXIT_FAILURE);
    }
    return ir_node;
}


/* code generation */
static linkedlist *rec_codegen_stack_machine(const ASTNode *ast, int current_label) {
    linkedlist *program = NULL;
    linkedlist *cursor = NULL;
    printf("current_label = %d\n", current_label);
    switch (ast->kind) {
        case CONDITIONAL:
        {
            /*
             * evaluate condition
             * if condition == 1
             *     then do left sub-tree
             * else
             *     then do right sub-tree
             *
             * if (1 > 0) {
             *     putchar('y');
             * } else {
             *     putchar('n');
             * }
             *
             * PUSH 1         ; (1 > 0)
             * PUSH 0
             * GT             ; 1 if true, 0 if false
             *
             * JZ _else       ; jump if 0 (i.e. if false, goto else block)
             * _if:           ; if block
             *     PUSH 'y'
             *     PRINTC
             *     J _end_if  ; break out of if (skip over the else block)
             *
             * _else:
             *     PUSH 'n'
             *     PRINTC
             * _end_if:       ; continue with program
             * ...
             */
            char else_label[255];
            char target_else_label[255];
            char if_label[255];
            char end_if_label[255];
            char target_end_if[255];
            sprintf(else_label, "_else_%d", current_label);
            sprintf(target_else_label, "_else_%d:", current_label);
            sprintf(if_label, "_if_%d:", current_label);
            sprintf(end_if_label, "_end_if_%d", current_label);
            sprintf(target_end_if, "_end_if_%d:", current_label);

            /* eval condition */
            program = rec_codegen_stack_machine(ast->condition, current_label + 1);
            cursor = program;

            /* append jump to else if 0 */
            cursor = ll_append(cursor, ir_new_jump_inst(JZ, else_label));

            /* append if label (not needed but helps for clarity in ASM) */
            cursor = ll_append(cursor, ir_new_label(if_label));

            /* concat eval left */
            ll_concat(cursor, rec_codegen_stack_machine(ast->left, current_label + 1));

            /* append jump to end if */
            cursor = ll_append(cursor, ir_new_jump_inst(J, end_if_label));

            /* append else label */
            cursor = ll_append(cursor, ir_new_label(target_else_label));

            /* concat eval right */
            ll_concat(cursor, rec_codegen_stack_machine(ast->right, current_label + 1));

            /* append end if label */
            cursor = ll_append(cursor, ir_new_label(target_end_if));

            current_label++;
            break;
        }

        case OPERATOR:
            program = rec_codegen_stack_machine(ast->right, current_label);
            ll_concat(program, rec_codegen_stack_machine(ast->left, current_label));
            ll_append(program, get_op_ir(ast->op));
            break;

        case LEAF:
        {
            Ir *ir = (Ir *)malloc(sizeof(Ir));
            ir->repr = "PUSH";
            ir->kind = IR_OP;
            ir->value.op = PUSH;
            program = ll_new(ir);
            ll_append(program, get_ir_node(ast));
            break;
        }

        default:
            fprintf(stderr, "unknown ASTNode kind in emit: %d\n", ast->kind);
            exit(EXIT_FAILURE);
    }
    return program;
}


static linkedlist *codegen_stack_machine(const ASTNode *ast) {
    return rec_codegen_stack_machine(ast, 0);
}


int emit(FILE *output, const ASTNode *ast) {
    linkedlist *program = codegen_stack_machine(ast);
    program = ir_halt_program(program);
    ir_print_program(output, program);
    ll_free(program);
    return 0;
}
