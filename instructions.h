#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

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
    HALT
} inst_t;

char *inst_names[] = {
    "NOP",
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
    "CALL",
    "RET",
    "POPC",
    "HALT"
};

#endif /* INSTRUCTIONS_H */
