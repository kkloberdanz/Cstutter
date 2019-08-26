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
 * File: grammar.y
 */


%{
#define YYPARSER


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>


#include "minic.h"


#define YYSTYPE ASTNode *


static int yylex();
void yyerror(const char *s);
static ASTNode *tree = NULL;
static FILE *source_file = NULL;

%}

%token ENDFILE
%token ERROR
%token IF
%token THEN
%token ELSE
%token PRINT
%token INT
%token ID
%token NUMBER
%token ASSIGN
%token EQ
%token NE
%token LT
%token GE
%token LE
%token GT
%token PLUS
%token MINUS
%token TIMES
%token OVER
%token EXPONENT
%token LPAREN
%token RPAREN
%token LBRACE
%token RBRACE
%token SEMICOLON
%token COMMA


%left MINUS PLUS
%left TIMES OVER
%right EXPONENT        /* exponentiation */

%%
prog        : stmts                 { tree = $1 ; }
            ;

stmts       : stmt                  { $$ = $1 ; }
            | stmts stmt            {
                                        YYSTYPE ast = $1;
                                        if (ast) {
                                            while (ast->sibling) {
                                                ast = ast->sibling;
                                            }
                                            ast->sibling = $2;
                                            $$ = $1;
                                        } else {
                                            $$ = $2;
                                        }
                                    }

            ;

stmt        : expr SEMICOLON        { $$ = $1 ; }
            | if_stmt               { $$ = $1 ; }
            | assign_expr SEMICOLON { $$ = $1 ; }
            | declare SEMICOLON     { $$ = $1 ; }
            | decl_assign SEMICOLON { $$ = $1 ; }
            | decl_func             { $$ = $1 ; }
            ;

decl_func   : INT id LPAREN RPAREN
              LBRACE
              stmts
              RBRACE                { $$ = make_function_node($2, $6); }
            ;

declare     : INT id                { $$ = make_declare_node($2) ; }
            ;

assign_expr : id ASSIGN expr        { $$ = make_assign_node($1, $3); }
            ;

decl_assign : INT id ASSIGN expr    {
                                        YYSTYPE decl = make_declare_node($2);
                                        decl->right = make_assign_node($2, $4);
                                        $$ = decl;
                                    }
            ;

if_stmt     : IF LPAREN expr RPAREN LBRACE
                  stmt
              RBRACE ELSE LBRACE
                  stmt
              RBRACE                { $$ = make_conditional_node($3, $6, $10) ; }
            | IF LPAREN expr RPAREN LBRACE
                  stmt
              RBRACE                { $$ = make_conditional_node($3, $6, NULL) ; }
            ;

expr        : expr PLUS expr        { $$ = make_operator_node(OP_PLUS, $1, $3) ; }
            | expr MINUS expr       { $$ = make_operator_node(OP_MINUS, $1, $3) ; }
            | expr TIMES expr       { $$ = make_operator_node(OP_TIMES, $1, $3) ; }
            | expr OVER expr        { $$ = make_operator_node(OP_DIVIDE, $1, $3) ; }
            | bool_expr             { $$ = $1 ; }
            | call_func             { $$ = $1 ; }
            | LPAREN expr RPAREN    { $$ = $2 ; }
            | NUMBER                { $$ = make_leaf_node(make_number_obj(token_string)) ; }
            | id                    { $$ = make_load_node($1) ; }
            ;

bool_expr   : expr EQ expr          { $$ = make_operator_node(OP_EQ, $1, $3) ; }
            | expr LT expr          { $$ = make_operator_node(OP_LT, $1, $3) ; }
            | expr LE expr          { $$ = make_operator_node(OP_LE, $1, $3) ; }
            | expr GT expr          { $$ = make_operator_node(OP_GT, $1, $3) ; }
            | expr GE expr          { $$ = make_operator_node(OP_GE, $1, $3) ; }
            | expr NE expr          { $$ = make_operator_node(OP_NE, $1, $3) ; }
            ;

args        : expr                  { $$ = $1 ; }
            | expr COMMA args       {
                                        YYSTYPE ast = $3;
                                        if (ast) {
                                            while (ast->sibling) {
                                                ast = ast->sibling;
                                            }
                                            ast->sibling = $1;
                                            $$ = $3;
                                        } else {
                                            $$ = $1;
                                        }
                                    }
            ;

call_func   : id LPAREN args RPAREN { $$ = make_func_call_node($1, $3) ; }
            ;

id          : ID                    { $$ = make_leaf_node(make_id_obj(token_string)) ; }
            ;

%%


ASTNode *parse(FILE *src_file) {
    source_file = src_file;
    yyparse();
    return tree;
}


static int yylex(void) {
    int token = get_token(source_file);
    return token;
}
