%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define YYSTYPE char *
int yylex(void);
void yyerror(const char *s);
%}

// Tokens Definition
%token PLUS
%token MINUS
%token MULT
%token ASSIGN
%token DIV
%token LT
%token LEQ
%token GT
%token GEQ
%token EQ
%token NEQ
%token LBRACKET
%token RBRACKET
%token LPAREN
%token RPAREN
%token LBRACE
%token RBRACE
%token SEMICOLON
%token COMMA
%token ID
%token NUM
%token IF_KEYWORD
%token ELSE_KEYWORD
%token INT_KEYWORD
%token RETURN_KEYWORD
%token VOID_KEYWORD
%token WHILE_KEYWORD
%token MAIN_KEYWORD
%token FLOAT_KEYWORD

%error-verbose

%%
program: typeSpecifier MAIN_KEYWORD LPAREN optionalDeclarationList RPAREN LBRACE optionalDeclarationList statementList RBRACE
;
optionalDeclarationList: declarationList
| /*epislon*/
;
declarationList: declaration declarationListTail
;
declarationListTail: declaration declarationListTail | /*epslion*/
;
declaration: varDeclaration
;
varDeclaration: typeSpecifier ID varDeclarationTail
;
varDeclarationTail: SEMICOLON | LBRACKET NUM RBRACKET SEMICOLON
;
typeSpecifier: INT_KEYWORD | FLOAT_KEYWORD | VOID_KEYWORD
;
params: paramList | VOID_KEYWORD
;
paramList: param paramListTail
;
paramListTail: COMMA param paramList
| /*epsilon*/
;
param: typeSpecifier ID paramTail
;
paramTail: LBRACKET RBRACKET
| /*epsilon*/
;
compoundStmt: LBRACE statementList RBRACE
;
statementList: statementListTail
;
statementListTail: statement statementListTail
| /*epsilon*/
;
statement: assignmentStmt | compoundStmt | selectionStmt | iterationStmt
;
selectionStmt: IF_KEYWORD LPAREN expression RPAREN statement selectionStmtTail
;
selectionStmtTail: ELSE_KEYWORD statement
| /*epsilon*/
;
iterationStmt: WHILE_KEYWORD LPAREN expression RPAREN statement
;
assignmentStmt: var ASSIGN expression SEMICOLON
;
var: ID varTail
;
varTail: LBRACKET expression RBRACKET
| /*epsilon*/
;
expression: additiveExpr expressionTail
;
expressionTail: relop additiveExpr expressionTail
| /*epsilon*/
;
relop: LEQ | GEQ | EQ | NEQ | LT | GT
;
additiveExpr: term addtiveExprTail
;
addtiveExprTail: addop term addtiveExprTail
| /*epsilon*/
;
addop: PLUS | MINUS
;
term: factor termTail
;
termTail: mulop factor termTail
| /*epsilon*/
;
factor: LPAREN expression RPAREN | var | NUM
;
mulop: MULT | DIV
;

%%
void yyerror(const char *s) {
    fprintf(stdout, "%s\n", s);
}


int main()
{
    if (yyparse() == 0)
        fprintf(stderr, "Parsing completed with no errors!\n");
}