#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE                 2000
#define PROG_SIZE                  2000
#define CALL_STACK_SIZE             500
#define STORAGE_SIZE                500

/* Code section */
int *program;

/* execution stack */
int stack[STACK_SIZE];

/* accessed with instructions STORE and LOAD*/
int storage[STORAGE_SIZE];

/* Save return address here */
int call_stack[CALL_STACK_SIZE];

/* Registers */
int pc = 0; /* Program Counter */

int sp = 0; /* Stack Pointer */

int cp = 0; /* Call Pointer 
               Return address returns address after jump instructions */

int sr = PROG_SIZE; /* Stack Register / Save Register
                       Used for saving data to the stack */

void print_call_stack() {
    int i;
    printf("CP = %d\n", cp);
    for (i = 0; i <= cp; ++i) {
        printf("CALL STACK[%d] = %d\n", i, call_stack[i]);
    }
}

void set_call_stack() {
    call_stack[cp] = pc + 2; 
    cp++;
}

char inst_names[50][50] = {
    "NOOP",
    "PUSH",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "MOD",
    "PRINTI",
    "READI",
    "PRINTC",
    "READC",
    "POP",
    "LOAD",
    "STORE",
    "J",
    "JZ",
    "JLEZ",
    "JNZ",
    "JWL",
    "JZWL",
    "JLEZWL",
    "JNZWL",
    "RET",
    "POPC",
    "HALT"
};

enum {
    NOOP    = 0,
    PUSH    = 1,
    ADD     = 2,
    SUB     = 3,
    MUL     = 4,
    DIV     = 5,
    MOD     = 6,
    PRINTI  = 7,
    READI   = 8,
    PRINTC  = 9,
    READC   = 10,
    POP     = 11,
    LOAD    = 12,
    STORE   = 13,
    J       = 14,
    JZ      = 15,
    JLEZ    = 16,
    JNZ     = 17,
    JWL     = 18,
    JZWL    = 19,
    JLEZWL  = 20,
    JNZWL   = 21,
    RET     = 22,
    POPC    = 23,
    HALT = 999999
};

void print_stack() {
    printf("*** PRINTING STACK ***\n");
    int i;
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

int get_num_lines(char* filename) {
    FILE *fp;
    fp = fopen(filename, "r");
    int count = 0;
    char c;
    while ( (c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            count++;
        }
    }
    fclose(fp);
    return count;
}

void load_code_from_file(int *code, char *filename) {
    FILE *fp;
    fp = fopen(filename, "r");

    char buff[255];
    char c;
    int  count = 0, i = 0;
    printf("Reading from: %s\n", filename);
    while ( (c = fgetc(fp)) != EOF) { 
        if (c == '\n') {
            buff[count] = '\0';
            count = 0;
            code[i] = atoi(buff);
            i++;
        }
        buff[count] = c;
        count++;
    }
    fclose(fp);
}

int execute(int inst) {

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
    /*printf("%d\n", inst);*/
    if (inst != HALT) {
        printf("\nINST: %s, PC: %d, SP: %d, TOP: %d\n", inst_names[inst], pc, sp, stack[sp]);
    }
#endif

    switch (inst) {

        case NOOP:
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
            set_call_stack();
            pc = program[pc+1]; 
#ifdef DEBUG
            printf("J target: %d\n", pc);
#endif
            return 1;
            break;

        case JZ:
            set_call_stack();
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
            set_call_stack();
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
            set_call_stack();
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


        case JZWL:
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

        case JLEZWL:
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
        case JNZWL:
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

        case JWL:
            pc = program[pc+1];
            return 1;
            break;

        case PRINTI:
            printf("%d", stack[sp]);
            break;

        case READI:
            sp++;
            scanf("%d", &stack[sp]);
            break;

        case PRINTC: 
            printf("%c", stack[sp]);
            break;

        case READC:
            sp++;
            stack[sp] = getchar();
            // String is done being read once RETURN is pressed
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

void loop() {
    for (pc = 0; execute(program[pc]); ) {
#ifdef DEBUG
        print_stack();
#endif
    }
}

void print_array(int* arr, int size) {
    int i;
    printf("[");
    for (i = 0; i < size - 1; ++i) {
        printf("%d, ", arr[i]);
    }
    printf("%d]\n", arr[i]);
}

int main(int argc, char** argv) { 

    if (argc < 2) {
        fprintf(stderr, "error: specify the file name\n");
        exit(EXIT_FAILURE);
    } 

    printf("*** LOADING ***\n");
    int num_lines = get_num_lines(argv[1]);
    program = malloc(num_lines * sizeof(int));

    load_code_from_file(program, argv[1]);
#ifdef DEBUG
    print_array(program, num_lines);
#endif

    printf("*** DONE LOADING ***\n");
    printf("### RUNNING ###\n");

    loop();
    print_stack();
    free(program);
    return 0;
}
