%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<stdarg.h>

#define ANSI_COLOR_RED     "\033[31;1m"
#define ANSI_COLOR_RESET   "\033[0m"
#define ANSI_BOLD          "\033[1m"

int stop_at_first_error = 0;
int print_compact = 0;
int output_to_file = 0;
int found_err = 0;


FILE* openedFile;

// sync stdout and stderr
static void sync_stdout_stderr() {
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
}



/* Keep track of line numbers and character positions */

int line_number = 1;
int char_position = 1;

#define UPDATE_CHAR_POSITION char_position += yyleng;


/* Error handling */
void print_red(const char *msg) {
    if(!output_to_file)
    fprintf(stderr, ANSI_COLOR_RED "%s" ANSI_COLOR_RESET, msg);
    else
    fprintf(stderr, "%s" ,msg);

}

void print_parsing_error(const char *msg) {
    fprintf(openedFile, "\n" __FILE__ ":%d:%d:",  line_number, char_position);
    print_red(" error: ");
    fprintf(stderr, "%s %s\n",msg,yytext);
    found_err = 1;
    if (stop_at_first_error) {
        exit(1);
    }
}

void throw_error(const char* msg){

    fprintf(openedFile, "error: %s",msg);
    exit(1);
}

/* Print with new line */
void  print_token(const char *format , ...)
{
    // print with new line
    va_list args;
    va_start(args, format);
    vfprintf(openedFile, format, args);
    va_end(args);

    // fprintf(openedFile, " %d", line_number);

    if(!print_compact)
    fprintf(openedFile, "\n");

}
void print_bold( const char* msg ){
    if(!output_to_file&&!print_compact)
    fprintf(openedFile, ANSI_BOLD "%s" ANSI_COLOR_RESET, msg);
    else
    fprintf(openedFile, "%s",msg);

}

void yyerror(char *);

%}

%option noyywrap


/* Keywords */
keywords (else|if|int|return|void|while|main|float)
/*special symbols*/
PLUS (\+)
MINUS "-"
ASSIGN "="
MULT "*"
DIV "/"
LT "<"
LEQ "<="
GT ">"
GEQ ">="
EQ "=="
NEQ "!="
LBRACKET "["
RBRACKET "]"
LPAREN "("
RPAREN ")"
LBRACE "{"
RBRACE "}"
SEMICOLON ";"
COMMA ","


at_symbol "@"
hash_tag "#"
dollar_sign "$"
under_score "_"





/* Regular expression definitions */

letter [a-zA-Z]
digit [0-9]

ID {letter}({letter}|{digit})*(({at_symbol}|{hash_tag}|{dollar_sign}|{under_score})?{digit}+)?

/* a wrong identifier in cases like these: a# followed by any characters which is not letter nor digit */
invalid_ID  {letter}({letter}|{digit})*(({at_symbol}|{hash_tag}|{dollar_sign}|{under_score}))((^{letter})|(^{digit}))

num {digit}+(\.{digit}+)?((E|e)[\+|-]?{digit}+)?
invalid_num {digit}+(\.{digit}+)?((E|e)[\+|-]?[^digit])?

ws [ \t]
nested_comment "/*"
unclosed_comment "/*"(.|\n)*
comment "/*"([^\*][^\\]|\n)*?"*/"

%x COMMENT



%%


{keywords}  { print_token(yytext); UPDATE_CHAR_POSITION; }


{PLUS}          { print_token("PLUS"); UPDATE_CHAR_POSITION;}
{MINUS}         { print_token("MINUS"); UPDATE_CHAR_POSITION;}
{ASSIGN}        { print_token("ASSIGN"); UPDATE_CHAR_POSITION;}
{MULT}          { print_token("MULT"); UPDATE_CHAR_POSITION;}
{DIV}           { print_token("DIV"); UPDATE_CHAR_POSITION;}
{LT}            { print_token("LT"); UPDATE_CHAR_POSITION;}
{LEQ}           { print_token("LEQ"); UPDATE_CHAR_POSITION;}
{GT}            { print_token("GT"); UPDATE_CHAR_POSITION;}
{GEQ}           { print_token("GEQ"); UPDATE_CHAR_POSITION;}
{EQ}            { print_token("EQ"); UPDATE_CHAR_POSITION;}
{NEQ}           { print_token("NEQ"); UPDATE_CHAR_POSITION;}
{LBRACKET}      { print_token("LBRACKET"); UPDATE_CHAR_POSITION;}
{RBRACKET}      { print_token("RBRACKET"); UPDATE_CHAR_POSITION;}
{LPAREN}        { print_token("LPAREN"); UPDATE_CHAR_POSITION;}
{RPAREN}        { print_token("RPAREN"); UPDATE_CHAR_POSITION;}
{LBRACE}        { print_token("LBRACE"); UPDATE_CHAR_POSITION;}
{RBRACE}        { print_token("RBRACE"); UPDATE_CHAR_POSITION;}
{SEMICOLON}     { print_token("SEMICOLON"); UPDATE_CHAR_POSITION;}
{COMMA}         { print_token("COMMA"); UPDATE_CHAR_POSITION;}


\n      { line_number++; char_position = 1;} /* Keep track of line numbers */

"/*"    { BEGIN(COMMENT); UPDATE_CHAR_POSITION; }
 <COMMENT>{
    "*/"    { BEGIN(INITIAL); UPDATE_CHAR_POSITION; }
    "\n"    { line_number++; char_position = 1; }
    "/*"   { UPDATE_CHAR_POSITION; print_parsing_error("Nested comment"); }
    .       { UPDATE_CHAR_POSITION; }

    <<EOF>> { UPDATE_CHAR_POSITION; print_parsing_error("Unclosed comment"); return 1;
    }
}

{ws}    {UPDATE_CHAR_POSITION;} /* Keep track of character positions */


{ID}             { print_token("ID"); UPDATE_CHAR_POSITION;}
{num}            { print_token("NUM"); UPDATE_CHAR_POSITION;}


{invalid_num} {UPDATE_CHAR_POSITION; print_parsing_error("Invalid number"); }

.       {
    UPDATE_CHAR_POSITION;
    print_parsing_error("Invalid character"); } /* Error handling */


%%

int lexAnalysis(const char* outFile, const char* inputFile)
{
    openedFile = fopen(outFile, "w");
    yyin = fopen(inputFile, "r");
    yylex();
    fclose(openedFile);
    fclose(yyin);

    return found_err;
}