PUSH 0
PUSH 100
PUSH 108
PUSH 114
PUSH 111
PUSH 119
PUSH 32
PUSH 111
PUSH 108
PUSH 108
PUSH 101
PUSH 104

CALL _PrintASCII
CALL _PrintNewline
J _exit

_PrintNewline:
    PUSH 10         ; Print Newline
    PRINTC
    POP
    RET

_PrintASCII:
    PRINTC          ; Print char then pop
    POP
    JNZ _PrintASCII ; Print until 0 on top of stack (0 is end of string)
    RET

_exit:
    HALT
