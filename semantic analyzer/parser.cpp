#include <iostream>
#include <string>
#include <fstream>


#define PLUS        "PLUS"
#define ASSIGN      "ASSIGN"
#define MULT        "MULT"
#define DIV         "DIV "
#define LT          "LT"
#define LEQ         "LEQ"
#define GT          "GT"
#define GEQ         "GEQ"
#define EQ          "EQ"
#define LBRACKET    "LBRACKET"
#define RBRACKET    "RBRACKET"
#define LPAREN      "LPAREN"
#define RPAREN      "RPAREN"
#define LBRACE      "LBRACE"
#define RBRACE      "RBRACE"
#define SEMICOLON   "SEMICOLON"
#define COMMA       "COMMA"
#define ID          "ID"
#define NUM         "NUM"

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


extern int lexAnalysis(const char* outFile, const char* inputFile);

struct Token {
    std::string txt;
    uint32_t lineNumber;
    Token* next;
};

Token* lookAheadToken;
std::string foundToken;


void nextToken()
{
    lookAheadToken = lookAheadToken->next;
}

bool isNext(std::string currentToken)
{
    return (lookAheadToken->txt == currentToken);
}

bool match(std::string currentToken)
{
    if (lookAheadToken->txt == currentToken)
    {
        nextToken();
        return true;
    }
    else
    {
        foundToken = currentToken;
        return false;
    }
}

int main(int argc, const char* argv[])
{
    const char* intermediateFile = "testFile.txt";    
    const char* inputFile = "input.c";
    if (lexAnalysis(intermediateFile, inputFile)) 
    {
        printf("Error in syntax analysis\n");
        remove(intermediateFile);
        return  1;
    }

    std::ifstream inputStream(intermediateFile);

    Token* list = nullptr;
    Token* lastToken = nullptr;
    while(!inputStream.eof())
    {
        std::string token;
        uint32_t lineNumber;

        inputStream >> token >> lineNumber;
        if (token.empty() == false)
        {
            if (list == nullptr)
            {
                list = new Token{token, lineNumber, nullptr};
                lastToken = list;
            }
            else 
            {
                lastToken->next = new Token{token, lineNumber, nullptr};
                lastToken = lastToken->next;
            }
        }
   }
    inputStream.close();

    lookAheadToken = list;
    if (program() == false)
    {
        std::cerr << "Syntax error line # " << lookAheadToken->lineNumber << ", expected token (" << 
        lookAheadToken->txt << "), but got " << "(" << foundToken << ")";

    }

    remove(intermediateFile);
    return 0;
}

bool program()
{
    return  type_specifier() 
            && match("main")
            && match(LPAREN)
            && declaration_list()
            && match(RPAREN)
            && match(LBRACE)
            && declaration_list()
            && statement_list()
            && match(RBRACE);
}

bool declaration_list()
{
    return  declaration() &&
            declaration_list_tail();
}

bool declaration_list_tail()
{
    if (isNext("int") || isNext("void"))
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
    return type_specifier() && match("ID") && var_declaration_tail();
}

bool var_declaration_tail()
{
    return match(SEMICOLON) || 
            (match(LBRACKET) 
            && match(NUM) 
            && match(RBRACKET) 
            && match(SEMICOLON));
}

bool type_specifier()
{
    return match("int") || match("float");
}

bool params()
{
    return param_list() || match("void");
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
    else return true;
}

bool compound_stmt()
{

}

bool statement_list()
{

}

bool statement_list_tail()
{

}

bool statement()
{

}

bool selection_statement()
{

}

bool selection_statement_tail()
{

}

bool iteration_statement()
{

}

bool assignment_statement()
{

}

bool var()
{

}

bool var_tail()
{

}

bool expression()
{

}

bool expression_tail()
{

}

bool relop()
{

}

bool relop_tail()
{

}

bool additive_expression()
{

}

bool additive_expression_tail()
{

}

bool addop()
{

}

bool term()
{

}

bool term_tail()
{

}

bool mulop()
{

}

bool factor()
{

}

