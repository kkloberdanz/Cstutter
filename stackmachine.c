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
 * File: stackmachine.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "instructions.h"

#define STACK_SIZE                 2000
#define PROG_SIZE                  2000
#define CALL_STACK_SIZE             500
#define STORAGE_SIZE                500

/* Code section */
static int *program;

/* execution stack */
static int stack[STACK_SIZE];

/* accessed with instructions STORE and LOAD*/
static int storage[STORAGE_SIZE];

/* Save return address here */
static int call_stack[CALL_STACK_SIZE];

/* Registers */
static int pc = 0; /* Program Counter */

static int sp = 0; /* Stack Pointer */

static int cp = 0; /* Call Pointer
               Return address returns address after jump instructions */

/* Stack Register Save Register used for saving data to the stack */
/*static int sr = PROG_SIZE;*/

#ifdef DEBUG
static void print_call_stack() {
    int i;
    printf("CP = %d\n", cp);
    for (i = 0; i <= cp; ++i) {
        printf("CALL STACK[%d] = %d\n", i, call_stack[i]);
    }
}
#endif

static void set_call_stack() {
    call_stack[cp] = pc + 2;
    cp++;
}

static void print_stack() {
    int i;
    printf("*** PRINTING STACK ***\n");
    printf("SP: %d\n", sp);
    printf("PC: %d\n", pc);
    for (i = 0; i <= sp; ++i) {
        if (i == sp) {
            printf("%2d: %d*\n", i, stack[i]);
        } else {
            printf("%2d: %d\n", i, stack[i]);
        }
    }
    printf("*** DONE PRINTING ***\n");
}

static int get_num_lines(char* filename) {
    FILE *fp;
    int count = 0;
    int c;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "not a file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            count++;
        }
    }
    fclose(fp);
    return count;
}

static void load_code_from_file(int *code, char *filename) {
    FILE *fp;
    char buff[255];
    int c;
    int count = 0;
    int i = 0;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "not a file: %s\n", filename);
    }

    printf("Reading from: %s\n", filename);
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            buff[count] = '\0';
            count = 0;
            code[i] = atoi(buff);
            i++;
        }
        buff[count] = (char)c;
        count++;
    }
    fclose(fp);
}

static int execute(int inst) {

    if (pc > PROG_SIZE) {
        fprintf(stderr, "ERROR: PC out of bounds\n");
        print_stack();
        exit(EXIT_FAILURE);
    }

    if (sp > STACK_SIZE) {
        fprintf(stderr, "ERROR: SP out of bounds\n");
        print_stack();
        exit(EXIT_FAILURE);
    }

    if (sp < 0) {
        fprintf(stderr, "ERROR: SP less than zero\n");
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG
    printf("\nINST: %s:%d, PC: %d, SP: %d, TOP: %d\n",
           inst_names[inst], inst, pc, sp, stack[sp]);
#endif

    switch (inst) {

        case NOP:
            break;

        case PUSH:
            sp++;
            stack[sp] = program[++pc];
            break;

        case LOAD:
            stack[++sp] = storage[program[++pc]];
            break;

            /*
        case LOADRA:
             ra = stack[sr];
             sr++;
             break;
             */

        case STORE:
            storage[program[++pc]] = stack[sp--];
            break;

            /*
        case STORERA:
            stack[sr] = ra;
            sr--;
            break;
            */

        case J:
            pc = program[pc+1];
            return 1;
            break;

        case CALL:
            set_call_stack();
            pc = program[pc+1];
#ifdef DEBUG
            printf("J target: %d\n", pc);
#endif
            return 1;
            break;

        case JZ:
            if (stack[sp] == 0) {
                pc = program[pc+1];
#ifdef DEBUG
                printf("JZ target: %d\n", pc);
#endif
                return 1;
            } else {
                pc++;
            }
#ifdef DEBUG
            printf("JZ target: %d\n", pc);
#endif
            break;

        case JLEZ:
            if (stack[sp] <= 0) {
                pc = program[pc+1];
#ifdef DEBUG
                printf("JLEZ target: %d\n", pc);
                print_call_stack();
#endif
                return 1;
            } else {
                pc++;
            }
#ifdef DEBUG
            printf("JLEZ target: %d\n", pc);
#endif
            break;


        /* Jump if Not Zero */
        case JNZ:
            if (stack[sp] != 0) {
                pc = program[pc+1];
#ifdef DEBUG
                printf("JNZ target: %d\n", pc);
#endif
                return 1;
            } else {
                pc++;
            }
#ifdef DEBUG
            printf("JZ target: %d\n", pc);
#endif
            break;


        /* Return from subroutine,
         * Sets PC to the top address from call_stack[]
         */
        case RET:
            cp--;
            pc = call_stack[cp];
#ifdef DEBUG
            print_call_stack();
            printf("PC = %d, RETURNING TO: %d\n", pc, program[call_stack[cp]]);
#endif
            return 1;
            break;

        case POPC:
            cp--;
            break;

        case ADD:
            {
            int a = stack[sp--];
            int b = stack[sp];
            stack[sp] = a + b;
            }
            break;

        case SUB:
            {
            int a = stack[sp--];
            int b = stack[sp];
            stack[sp] = a - b;
            }
            break;

        case MUL:
            {
            int a = stack[sp--];
            int b = stack[sp];
            stack[sp] = a * b;
            }
            break;

        case DIV:
            {
            int a = stack[sp--];
            int b = stack[sp];
            stack[sp] = a / b;
            }
            break;

        case MOD:
            {
            int a = stack[sp--];
            int b = stack[sp];
            stack[sp] = a % b;
            }
            break;

        case EQ:
            {
            int a = stack[sp--];
            int b = stack[sp];
            stack[sp] = a == b;
            }
            break;

        case NE:
            {
            int a = stack[sp--];
            int b = stack[sp];
            stack[sp] = a != b;
            }
            break;

        case LT:
            {
            int a = stack[sp--];
            int b = stack[sp];
            stack[sp] = a < b;
            }
            break;

        case LE:
            {
            int a = stack[sp--];
            int b = stack[sp];
            stack[sp] = a <= b;
            }
            break;

        case GT:
            {
            int a = stack[sp--];
            int b = stack[sp];
            stack[sp] = a > b;
            }
            break;

        case GE:
            {
            int a = stack[sp--];
            int b = stack[sp];
            stack[sp] = a >= b;
            }
            break;

        case PRINTI:
            printf("%d", stack[sp]);
            break;

        case PRINTC:
            printf("%c", stack[sp]);
            break;

        case READC:
            sp++;
            stack[sp] = getchar();
            /* String is done being read once RETURN is pressed */
            if (stack[sp] == '\n') {
                stack[sp] = '\0';
            }
            break;

        case POP:
            sp--;
            break;

        case HALT:
            printf("### HALTING ###\n");
            return 0;
            break;

        default:
            fprintf(stderr, "ERROR: unknown instruction: %d\n", inst);
            print_stack();
            exit(EXIT_FAILURE);
    }
    ++pc;
    return 1;
}

static void loop() {
    for (pc = 0; execute(program[pc]); ) {
#ifdef DEBUG
        print_stack();
#endif
    }
}

#ifdef DEBUG
static void print_array(int* arr, int size) {
    int i;
    printf("[");
    for (i = 0; i < size - 1; ++i) {
        printf("%d, ", arr[i]);
    }
    printf("%d]\n", arr[i]);
}
#endif

int main(int argc, char** argv) {

    int num_lines;
    if (argc < 2) {
        fprintf(stderr, "error: specify the file name\n");
        exit(EXIT_FAILURE);
    }

    printf("*** LOADING ***\n");
    num_lines = get_num_lines(argv[1]);
    program = malloc(num_lines * sizeof(int));

    load_code_from_file(program, argv[1]);
#ifdef DEBUG
    fprintf(stderr, "DEBUG MODE\n");
    print_array(program, num_lines);
#endif

    printf("*** DONE LOADING ***\n");
    printf("### RUNNING ###\n");

    loop();
    print_stack();
    free(program);
    return 0;
}
