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
    printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET, msg);
    else
    printf("%s" ,msg);

}

void print_parsing_error(const char *msg) {
    printf("\n" __FILE__ ":%d:%d:",  line_number, char_position);
    print_red(" error: ");
    printf("%s %s\n",msg,yytext);

    if (stop_at_first_error) {
        exit(1);
    }
}

void throw_error(const char* msg){

    printf("error: %s",msg);
    exit(1);
}

/* Print with new line */
void  print_token(const char *format , ...)
{
    // print with new line
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    if(!print_compact)
    printf("\n");

}
void print_bold( const char* msg ){
    if(!output_to_file&&!print_compact)
    printf(ANSI_BOLD "%s" ANSI_COLOR_RESET, msg);
    else
    printf("%s",msg);

}

%}

%option noyywrap


/* Keywords */
keywords (else|if|int|return|void|while|main)
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


{keywords}  { print_bold("KEYWORD: "); print_token("{%s} ", yytext); UPDATE_CHAR_POSITION; }


{PLUS}     { print_token("{PLUS} "); UPDATE_CHAR_POSITION;}
{ASSIGN}     { print_token("{ASSIGN} "); UPDATE_CHAR_POSITION;}
{MULT}     { print_token("{MULT} "); UPDATE_CHAR_POSITION;}
{DIV}     { print_token("{DIV} "); UPDATE_CHAR_POSITION;}
{LT}     { print_token("{LT} "); UPDATE_CHAR_POSITION;}
{LEQ}    { print_token("{LEQ} "); UPDATE_CHAR_POSITION;}
{GT}    { print_token("{GT} "); UPDATE_CHAR_POSITION;}
{GEQ}    { print_token("{GEQ} "); UPDATE_CHAR_POSITION;}
{EQ}    { print_token("{EQ} "); UPDATE_CHAR_POSITION;}
{LBRACKET}     { print_token("{LBRACKET} "); UPDATE_CHAR_POSITION;}
{RBRACKET}     { print_token("{RBRACKET} "); UPDATE_CHAR_POSITION;}
{LPAREN}     { print_token("{LPAREN} "); UPDATE_CHAR_POSITION;}
{RPAREN}     { print_token("{RPAREN} "); UPDATE_CHAR_POSITION;}
{LBRACE}     { print_token("{LBRACE} "); UPDATE_CHAR_POSITION;}
{RBRACE}     { print_token("{RBRACE} "); UPDATE_CHAR_POSITION;}
{SEMICOLON}     { print_token("{SEMICOLON} "); UPDATE_CHAR_POSITION;}
{COMMA}     { print_token("{COMMA} "); UPDATE_CHAR_POSITION;}


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


{ID}             { print_bold("ID: ");print_token("%s ",yytext); UPDATE_CHAR_POSITION;}
{num}              { print_bold("NUM: ");print_token("%s ",yytext); UPDATE_CHAR_POSITION;}


{invalid_num} {UPDATE_CHAR_POSITION; print_parsing_error("Invalid number"); }

.       {
    UPDATE_CHAR_POSITION;
    print_parsing_error("Invalid character"); } /* Error handling */


%%



int main(int argc, char *argv[])
{

    sync_stdout_stderr();

    int flags = 0;
    int i = 1;
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--stop-on-error") == 0 || strcmp(argv[i], "-s") == 0)
        {
            stop_at_first_error = 1;
            flags++;
        }

        if (strcmp(argv[i], "--compact") == 0 || strcmp(argv[i], "-c") == 0)
        {
            print_compact = 1;
            flags++;
        }
        if (strcmp(argv[i], "--output") == 0 || strcmp(argv[i], "-o") == 0)
        {
            if (i + 1 < argc)
            {
                output_to_file = 1;
                i++;
                freopen(argv[i], "w", stdout);

                argc -= 2;
                int j = i;
                for (j = i; j < argc; j++)
                {
                    argv[j] = argv[j + 2];
                }
            }
            else throw_error("Error: no file name provided for --output|-o\n");
        }
    }

    argc -= flags;
    argv += flags;

    if (argc == 1)
    {
        printf("usage: %s [--stop-on-error|-s] [--compact|-c] file1 file2 [--output|-o file]\n", argv[0]);
        return 1;
    }

    if (strcmp){
        int i = 1;
        for (i = 1; i < argc; i++)
        {
            FILE *file = fopen(argv[i], "r");
            if (!file)
            {
                fprintf(stderr, "Error: could not open file %s\n", argv[i]);
                return 1;
            }
            yyin = file;
            yylex();
            fclose(file);
        }
    }
    printf("\n");
    return 0;
}