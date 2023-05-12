#include "parser.h"

bool program()
{
    return type_specifier() && match("main") && match(LPAREN) && params() && match(RPAREN) && match(LBRACE) && declaration_list() && statement_list() && match(RBRACE);
}

bool declaration_list()
{
    return declaration() &&
           declaration_list_tail();
}

bool declaration_list_tail()
{
    if (isNext("int") || isNext("float"))
        return declaration() && declaration_list_tail();
    else
        return true;
}

bool declaration()
{
    return var_declaration();
}

bool var_declaration()
{
    return type_specifier() && match(ID) && var_declaration_tail();
}

bool var_declaration_tail()
{
    return (match(LBRACKET) && match(NUM) && match(RBRACKET) && match(SEMICOLON)) || match(SEMICOLON);
}

bool type_specifier()
{
    return match("int") ||
           match("float");
}

bool params()
{
    if (isNext("void"))
    {
        return match("void");
    }
    else
        return param_list();
}

bool param_list()
{
    return param() && param_list_tail();
}

bool param_list_tail()
{
    if (isNext(COMMA))
        return match(COMMA) && param() && param_list_tail();
    else
        return true;
}

bool param()
{
    return type_specifier() && match(ID) && param_tail();
}

bool param_tail()
{
    if (isNext(LBRACKET))
    {
        return match(LBRACKET) && match(RBRACKET);
    }
    else
        return true;
}

bool compound_stmt()
{
    return match(LBRACE) && statement_list() && match(RBRACE);
}

bool statement_list()
{
    return statement_list_tail();
}

bool statement_list_tail()
{
    if (isNext(ID) || isNext("if") || isNext("while") || isNext(LBRACE))
        return statement() && statement_list_tail();
    else
        return true;
}

bool statement()
{
    return selection_statement() ||
           iteration_statement() ||
           assignment_statement() ||
           compound_stmt();
}

bool selection_statement()
{
    return match("if") &&
           match(LPAREN) &&
           expression() &&
           match(RPAREN) &&
           statement() &&
           selection_statement_tail();
}

bool selection_statement_tail()
{
    if (isNext("else"))
        return match("else") && statement();
    else
        return true;
}

bool iteration_statement()
{
    return match("while") &&
           match(LPAREN) &&
           expression() &&
           match(RPAREN) &&
           statement();
}

bool assignment_statement()
{
    return var() &&
           match(ASSIGN) &&
           expression() && match(SEMICOLON);
}

bool var()
{
    return match(ID) &&
           var_tail();
}

bool var_tail()
{
    if (isNext(LBRACKET))
        return match(LBRACKET) && expression() && match(RBRACKET);
    else
        return true;
}

bool expression()
{
    return additive_expression() && expression_tail();
}

bool expression_tail()
{
    if (isNext(LT) || isNext(GT) || isNext(EQ) || isNext(NEQ))
        return relop() && additive_expression() && expression_tail();
    else
        return true;
}

bool relop()
{
    if (isNext(LT))
        return (match(LT) && relop_tail());
    else if (isNext(GT))
        return (match(GT) && relop_tail());
    else if (isNext(EQ))
        return match(EQ);
    else
        return match(NEQ);
}

bool relop_tail()
{
    if (isNext(ASSIGN))
        return match(ASSIGN);
    return true;
}

bool additive_expression()
{
    return term() && additive_expression_tail();
}

bool additive_expression_tail()
{
    if (isNext(PLUS) || isNext(MINUS))
        return addop() && term() && additive_expression_tail();

    return true;
}

bool addop()
{
    if (isNext(PLUS))
        return (match(PLUS));
    else
        return (match(MINUS));
}

bool term()
{
    return factor() && term_tail();
}

bool term_tail()
{
    if (isNext(MULT) || isNext(DIV))
        return mulop() && factor() && term_tail();
    else
        return true;
}

bool mulop()
{
    if (isNext(MULT))
        return (match(MULT));
    else
        return (match(DIV));
}

bool factor()
{
    if (isNext(NUM))
        return match(NUM);
    else if (isNext(ID))
        return match(ID);
    else
        return match(LPAREN) && expression() && match(RPAREN);
}
