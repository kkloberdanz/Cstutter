/*
 * Author: Kyle Kloberdanz
 * Project Start Date: 27 Nov 2018
 * License: GNU GPLv3 (see LICENSE.txt)
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * File: minic.h
 */


#ifndef STUTTER_H
#define STUTTER_H


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


/* fix warning from lex.yy.c */
int fileno(FILE *stream);


/* globals */
enum { MAX_TOKEN_SIZE=100 };
extern char token_string[MAX_TOKEN_SIZE+1];

/* embedded strings */
static volatile char author[] = "Author: Kyle Kloberdanz";
static volatile char license[] = "License: GNU GPLv3";


/* enums */
typedef enum {
    VOID_TYPE,  /* voids must be resolved during type deduction */
    NUMBER_TYPE,
    REAL_TYPE,
    BOOL_TYPE,
    STRING_TYPE
} MinicType;


typedef enum {
    CONDITIONAL,
    OPERATOR,
    LEAF,
    ASSIGN_EXPR,
    DECLARE_STMT
} ASTkind;


typedef enum {
    OP_NIL,
    OP_PLUS,
    OP_MINUS,
    OP_TIMES,
    OP_DIVIDE,
    OP_EQ,
    OP_LT,
    OP_LE,
    OP_GT,
    OP_GE,
    OP_NE,
    OP_NOT
} Operator;


/* structs */
typedef struct MinicObject {
    MinicType type;
    union {
        char *number_value;
        char *real_value;
        bool bool_value;
        char *string_value;
        char *symbol;
    } value;
} MinicObject;


typedef struct ASTNode {
    ASTkind kind;
    MinicObject *obj;
    Operator op;
    struct ASTNode *left;
    struct ASTNode *condition;
    struct ASTNode *right;
    struct ASTNode *sibling;
} ASTNode;


/* constructors */
MinicObject *make_number_obj(char *number);
MinicObject *make_string_obj(char *str);
MinicObject *make_id_obj(char *str);

char *make_string(char *str);

ASTNode *make_ast_node(const ASTkind, /* base constructor */
                       MinicObject *,
                       const Operator,
                       ASTNode *,
                       ASTNode *,
                       ASTNode *);

ASTNode *make_leaf_node(MinicObject *); /* just holds minic object */

ASTNode *make_operator_node(Operator,  /* holds operator and child items */
                            ASTNode *, /* to operate on */
                            ASTNode *);

ASTNode *make_conditional_node(ASTNode *left,
                               ASTNode *condition,
                               ASTNode *right);

ASTNode *make_assign_node(ASTNode *leaf_obj, ASTNode *right);
ASTNode *make_declare_node(ASTNode *leaf_obj);

/* destructors */
void destroy_obj(MinicObject *);
void destroy_ast_node(ASTNode *);


/* lexer */
int get_token(FILE *source_file);


/* parser */
ASTNode *parse(FILE *source_file);


/* code generation */
char *get_op_str(const Operator op);
char *get_op_val(char *str, const MinicObject *obj);
int emit(FILE *, const ASTNode *);


#endif /* STUTTER_H */
