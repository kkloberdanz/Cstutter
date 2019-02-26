%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>


#include "stutter.h"


#define YYSTYPE ASTNode *


int yylex();
void yyerror(const char *s);
static ASTNode *tree = NULL;

%}

%token NUMBER
%token ID

%left '-' '+'
%left '*' '/'
%right '^'        /* exponentiation */

%%
prog        : expr                       { tree = $1 ; }
            ;

expr        : expr '+' expr              { $$ = make_operator_node(ADD, $1, $3) ; }
            | expr '-' expr              { $$ = make_operator_node(SUB, $1, $3) ; }
            | expr '*' expr              { $$ = make_operator_node(MUL, $1, $3) ; }
            | expr '/' expr              { $$ = make_operator_node(DIV, $1, $3) ; }
            | '(' expr ')'               { $$ = $2 ; }
            | NUMBER                     { $$ = make_leaf_node(make_number_obj((number)$1)) ; }
            | ID                         { $$ = make_leaf_node(make_id_obj((char *)$1)) ; }
            ;

%%


ASTNode *parse(void) {
    yyparse();
    return tree;
}


int yylex() {
    int c;
    while ((c = getchar()) == ' ');
    if (isdigit(c)) {
        ungetc(c, stdin);
        if (!scanf("%ld", (number *)&yylval)) {
            fprintf(stderr, "%s\n", "failed to read from stdin");
        }
        return NUMBER;
    } else if (c == '\n') {
        return 0;
    } else if ((c == '+') ||
               (c == '-') ||
               (c == '*') ||
               (c == '/')) {
        return c;
    } else {
        ungetc(c, stdin);
        if (!scanf("%s", (char *)&yylval)) {
            fprintf(stderr, "%s\n", "failed to read from stdin");
        }
        return ID;
    }
}
