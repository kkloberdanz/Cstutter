/*
 * Author: Kyle Kloberdanz
 * Project Start Date: 27 Nov 2018
 * License: GNU GPLv3 (see LICENSE.txt)
 *     This file is part of stutter.
 *
 *     stutter is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     stutter is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with stutter.  If not, see <https://www.gnu.org/licenses/>.
 * File: assembler.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"
#include "bst.h"
#include "instructions.h"

typedef enum {
    false = 0,
    true = 1
} bool;

struct instruction {
    inst_t inst;
    const char *immediate;
    const char *str;
};

static bool requires_immediate(inst_t inst) {
    switch (inst) {
        case PUSH:
        case STORE:
        case LOAD:
        case J:
        case JZ:
        case JNZ:
        case JLEZ:
        case CALL:
            return true;
        default:
            return false;
    }
}

static bool is_jump(inst_t inst) {
    switch (inst) {
        case J:
        case JZ:
        case JNZ:
        case JLEZ:
        case CALL:
            return true;
        default:
            return false;
    }
}

static char *make_str(const char *str) {
    char *dst = malloc(strlen(str) + 1);
    strcpy(dst, str);
    return dst;
}

static struct instruction *lookup_instruction(const char *str) {
    inst_t inst;
    struct instruction *instruction;
    int i;
    for (i = 0; inst_names[i] != NULL; i++) {
        if (strcmp(inst_names[i], str) == 0) {
            inst = i;
            goto inst_found;
        }
    }
    return NULL;
inst_found:
    instruction = malloc(sizeof(struct instruction));
    if (instruction == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(EXIT_FAILURE);
    }

    instruction->inst = inst;
    instruction->str = make_str(str);
    instruction->immediate = NULL;
    return instruction;
}

static struct instruction *make_inst(const char *inst_str) {
    struct instruction *instruction = lookup_instruction(inst_str);
    return instruction;
}

static void strip(char *str) {
    while (*str++) {
        if (*str == '\n') {
            *str = 0;
            return;
        }
    }
}

static void populate_labels(linkedlist *instructions, struct BST *jump_labels) {
    linkedlist *cursor = instructions->next;
    while (cursor) {
        struct instruction *inst = cursor->value;
        if (is_jump(inst->inst)) {
            char *label = (char *)inst->immediate;
            struct BST *label_node = bst_find(jump_labels, label);
            int label_location = label_node->value;
            char str[11];
            sprintf(str, "%d", label_location);
            inst->immediate = make_str(str);
        }
        cursor = cursor->next;
    }
}

static linkedlist *assemble(const char *input_filename) {
    FILE *input_file = fopen(input_filename, "r");

    char input_buffer[255] = {0};
    linkedlist *instructions = ll_new(make_inst("NOP"));
    struct linkedlist *cursor = instructions;
    struct BST *jump_labels = NULL;
    int i = 0;
    while (fgets(input_buffer, 255, input_file)) {
        struct instruction *inst;
        int len;
        strip(input_buffer);
        len = strlen(input_buffer) - 1;
        if (input_buffer[len] == ':') {
            input_buffer[len] = '\0';
            jump_labels = bst_insert(jump_labels, make_str(input_buffer), i);
        } else {
            inst = make_inst(input_buffer);
            if (inst == NULL) {
                fprintf(stderr, "not a valid instruction: %s\n", input_buffer);
                exit(EXIT_FAILURE);
            }
            if (requires_immediate(inst->inst)) {
                if (!fgets(input_buffer, 255, input_file)) {
                    fprintf(stderr, "%s\n", "failed to read input");
                } else {
                    strip(input_buffer);
                    inst->immediate = make_str(input_buffer);
                }
                i++;
            }
            if (is_jump(inst->inst)) {
                char *label = (char *)inst->immediate;
                jump_labels = bst_insert(jump_labels, label, -1);
            }
            cursor = ll_append(cursor, inst);
        }
        i++;
    }
    populate_labels(instructions, jump_labels);
    fclose(input_file);
    return instructions;
}

static void destroy_instruction(struct instruction *inst) {
    free((char*)inst->immediate);
    free((char*)inst->str);
    free(inst);
}

static void destroy_instructions(linkedlist *ll) {
    linkedlist *prev = ll;
    while (ll) {
        prev = ll;
        destroy_instruction(ll->value);
        ll = ll->next;
        free(prev);
    }
}

static void emit_assembly(char *input_filename, char *output_filename) {
    FILE *output_file = fopen(output_filename, "w");
    linkedlist *instructions = assemble(input_filename);
    linkedlist *head = instructions->next;
    struct instruction *instruction;
    inst_t inst;
    while (head) {
        instruction = (struct instruction *)head->value;
        inst = instruction->inst;
        fprintf(output_file, "%d\n", inst);
        if (instruction->immediate) {
            fprintf(output_file, "%s\n", instruction->immediate);
        }
        head = head->next;
    }
    fclose(output_file);
    destroy_instructions(instructions);
}

int main(int argc, char **argv) {
    char *input_filename;
    char *output_filename;
    if (argc != 3) {
        fprintf(stderr, "%s",
                "invalid arguments, expecting assembler INPUT OUTPUT\n");
        exit(EXIT_FAILURE);
    }

    input_filename = argv[1];
    output_filename = argv[2];
    emit_assembly(input_filename, output_filename);

    return 0;
}
