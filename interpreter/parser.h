#pragma once

#include <iostream>
#include <string>
#include <fstream>

#define PLUS "PLUS"
#define MINUS "MINUS"
#define ASSIGN "ASSIGN"
#define MULT "MULT"
#define DIV "DIV "
#define LT "LT"
#define LEQ "LEQ"
#define GT "GT"
#define GEQ "GEQ"
#define EQ "EQ"
#define NEQ "NEQ"
#define LBRACKET "LBRACKET"
#define RBRACKET "RBRACKET"
#define LPAREN "LPAREN"
#define RPAREN "RPAREN"
#define LBRACE "LBRACE"
#define RBRACE "RBRACE"
#define SEMICOLON "SEMICOLON"
#define COMMA "COMMA"
#define ID "ID"
#define NUM "NUM"

// Functions of the grammar
bool program();
bool declaration_list();
bool declaration_list_tail();
bool declaration();
bool var_declaration();
bool var_declaration_tail();
bool type_specifier();
bool params();
bool param_list();
bool param_list_tail();
bool param();
bool param_tail();
bool compound_stmt();
bool statement_list();
bool statement_list_tail();
bool statement();
bool selection_statement();
bool selection_statement_tail();
bool iteration_statement();
bool assignment_statement();
bool var();
bool var_tail();
bool expression();
bool expression_tail();
bool relop();
bool relop_tail();
bool additive_expression();
bool additive_expression_tail();
bool addop();
bool term();
bool term_tail();
bool mulop();
bool factor();

struct Token
{
    std::string txt;
    uint32_t lineNumber;
    Token *next;
};

inline Token *lookAheadToken;
inline std::string expectedToken;

inline static void nextToken()
{
    lookAheadToken = lookAheadToken->next;
}

inline static bool isNext(std::string currentToken)
{
    return (lookAheadToken->txt == currentToken);
}

inline static bool match(std::string _expectedToken)
{
    if (lookAheadToken->txt == _expectedToken)
    {
        nextToken();
        return true;
    }
    else
    {
        expectedToken = _expectedToken;
        return false;
    }
}