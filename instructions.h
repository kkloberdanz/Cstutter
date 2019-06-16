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
 * File: instructions.h
 */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdio.h>

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
    "HALT",
    NULL
};

#endif /* INSTRUCTIONS_H */
