%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int yylex(void);
void yyerror(char *s);
%}

%token NUM

%%
E : NUM {printf("E\n");}
;
%%
int yaccParse(const char* outFile, const char* inputFile)
{
    yyparse();
}
void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}
