#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"

typedef enum {
    false = 0,
    true = 1
} bool;

typedef enum {
    NOP,
    PUSH,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    PRINTI,
    READI,
    PRINTC,
    READC,
    POP,
    LOAD,
    STORE,
    J,
    JZ,
    JLEZ,
    JNZ,
    CALL,
    RET,
    POPC,
    HALT = 999999
} inst_t;

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
    if (strcmp(str, "NOP") == 0) {
        inst = NOP;
    } else if (strcmp(str, "PUSH") == 0) {
        inst = PUSH;
    } else if (strcmp(str, "ADD") == 0) {
        inst = ADD;
    } else if (strcmp(str, "SUB") == 0) {
        inst = SUB;
    } else if (strcmp(str, "MUL") == 0) {
        inst = MUL;
    } else if (strcmp(str, "DIV") == 0) {
        inst = DIV;
    } else if (strcmp(str, "MOD") == 0) {
        inst = MOD;
    } else if (strcmp(str, "PRINTI") == 0) {
        inst = PRINTI;
    } else if (strcmp(str, "READI") == 0) {
        inst = READI;
    } else if (strcmp(str, "PRINTC") == 0) {
        inst = PRINTC;
    } else if (strcmp(str, "READC") == 0) {
        inst = READC;
    } else if (strcmp(str, "POP") == 0) {
        inst = POP;
    } else if (strcmp(str, "LOAD") == 0) {
        inst = LOAD;
    } else if (strcmp(str, "STORE") == 0) {
        inst = STORE;
    } else if (strcmp(str, "J") == 0) {
        inst = J;
    } else if (strcmp(str, "JZ") == 0) {
        inst = JZ;
    } else if (strcmp(str, "JLEZ") == 0) {
        inst = JLEZ;
    } else if (strcmp(str, "JNZ") == 0) {
        inst = JNZ;
    } else if (strcmp(str, "CALL") == 0) {
        inst = CALL;
    } else if (strcmp(str, "RET") == 0) {
        inst = RET;
    } else if (strcmp(str, "POPC") == 0) {
        inst = POPC;
    } else if (strcmp(str, "HALT") == 0) {
        inst = HALT;
    } else {
        return NULL;
    }

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

static linkedlist *assemble(const char *input_filename) {
    FILE *input_file = fopen(input_filename, "r");

    char input_buffer[255] = {0};
    linkedlist *instructions = ll_new(make_inst("NOP"));
    struct linkedlist *next = instructions;
    struct instruction *inst;
    while (fgets(input_buffer, 255, input_file)) {
        strip(input_buffer);
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
        }
        if (is_jump(inst->inst)) {
            /* TODO: store lable in labels map to be resolved at the
               end of parsing */
        }
        next = ll_append(next, inst);
    }
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
