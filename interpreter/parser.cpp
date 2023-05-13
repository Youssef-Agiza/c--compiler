#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <assert.h>
#include <vector>

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
int var_declaration_tail();
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

extern int lexAnalysis(const char *outFile, const char *inputFile);

struct Token
{
	std::string txt;
	std::string value;
	uint32_t lineNumber;
	Token *next;
};


uint32_t lineNumber;
std::string error_line, match_error;
Token *lookAheadToken;
std::string expectedToken;

std::vector<uint32_t> expression_is_true;
bool shouldExecute = true;
bool outputOfExpression = false;
std::string relationOp;

struct Symbol{
	union{
		int as_int = 0;
		float as_float;
		Symbol* as_pointer;
	};
	uint32_t size = 0;
	std::string name = "";
	uint32_t declaration_line = 0;
	enum TYPE{
		INT,
		FLOAT,
		POINTER,
		VOID,
		COUNT
	} type;
};
std::string symbolTypes[] = {
	"int",
	"float",
	"pointer",
	"void",
	"INVALID"
};

Symbol* left_v = nullptr, *right_v = nullptr;


std::ostream& operator<<(std::ostream& stream, const Symbol& sym)
{
	stream << "Symbol: (" << sym.name << ", " << symbolTypes[sym.type];
	if (sym.type == Symbol::POINTER)
	{
		stream << ", " << sym.size;
	}
	stream << ", " << sym.declaration_line;
	if (sym.type == Symbol::INT)
		stream << ", " << sym.as_int;
	else if (sym.type == Symbol::FLOAT)
		stream << ", " << sym.as_float;
	else if (sym.type == Symbol::POINTER)
		stream << ", " << std::hex << sym.as_pointer;

	stream << ")";
	return stream;
}


std::unordered_map<std::string, Symbol> symbolTable;

Symbol::TYPE lastType;
Symbol* lastSymbol;
uint32_t tmp_count = 0;
std::string operation;


bool removeTmps()
{
	bool removedSomething = true;
	while(removedSomething)
	for(auto& sym : symbolTable)
		if (sym.first.find("INTERNAL_INT_TMP_") != std::string::npos || (sym.first.find("INTERNAL_FLOAT_TMP_") != std::string::npos))
		{
			symbolTable.erase(sym.first);
			removedSomething = true;
			break;
		}
		else
			removedSomething = false;
			
	return true;
}

bool lookup(const std::string& symbolName)
{
	return symbolTable.find(symbolName) != symbolTable.end();
}

Symbol::TYPE getType(const std::string& symbolName)
{
	if (lookup(symbolName))
		return symbolTable[symbolName].type;
	else return Symbol::COUNT;
}

void nextToken()
{
	lookAheadToken = lookAheadToken->next;
}

bool isNext(std::string currentToken)
{
	return (lookAheadToken->txt == currentToken);
}

bool is_integer_number(const std::string& str)
{
	for(auto& c : str)
		if (isdigit(c) == false)
			return false;
	return true;
}

bool is_float_number(const std::string& str)
{
	try {
		std::stof(str);
		return true;
	} catch (...) {
		return false;
	}
}

bool match(std::string _expectedToken)
{
	lineNumber = lookAheadToken->lineNumber;
	// std::cout << "MATCHING " << lookAheadToken->txt << ", " << _expectedToken << '\n';
	if (lookAheadToken->txt == _expectedToken)
	{
		nextToken();
		return true;
	}
	else
	{
		match_error = std::string("Syntax error line # ") +
			std::to_string(lineNumber) + " expected token: " + _expectedToken
			+ " but got " + lookAheadToken->txt ;
		return false;
	}
}

Symbol* match_declared()
{
	if (lookAheadToken->txt == "ID")
	{
		const std::string& symbolName = lookAheadToken->value;
		// match(ID);
		nextToken();
		if (symbolTable.find(symbolName) == symbolTable.end())
		{
			match_error = "Undeclared variable: " + symbolName;
			return nullptr;
		}
		else
		{
			return &symbolTable[symbolName];
		}
	}
		match_error = "Failed to match declared ID";
	return nullptr;
}

Symbol* match_undeclared()
{
	if (lookAheadToken->txt == "ID")
	{
		std::string symbolName = lookAheadToken->value;

		if (symbolTable.find(symbolName) == symbolTable.end())
		{
			Symbol symbol;
			symbol.name = symbolName;
			symbol.declaration_line = lookAheadToken->lineNumber;
			symbol.type = lastType;
			symbolTable.insert({symbolName, symbol});

			// match(ID);
			nextToken();
			return &symbolTable[symbolName];
		}
		else
		{
			error_line = "Redeclaring an already declared variable: " + symbolTable[symbolName].name + " in line: #" + std::to_string(lookAheadToken->lineNumber)
			+ " Declared in line: " + std::to_string(symbolTable[symbolName].declaration_line);
			return nullptr;
		}
	}
	error_line = "Failed to match undeclared ID";
	return nullptr;
}

Symbol* match_number()
{
	if (lookAheadToken->txt == NUM)
	{
		if (is_integer_number(lookAheadToken->value))
		{
			std::string name = std::string("INTERNAL_INT_TMP_") + std::to_string(tmp_count);
			tmp_count++;
			Symbol tmp_symbol{};
			tmp_symbol.type = Symbol::INT;
			tmp_symbol.declaration_line = lookAheadToken->lineNumber;
			tmp_symbol.as_int = std::stoi(lookAheadToken->value);
			tmp_symbol.name = name;

			// match(NUM);
			nextToken();
			symbolTable.insert({name, tmp_symbol});
			return &symbolTable[name];
		}
		else if (is_float_number(lookAheadToken->value))
		{
			std::string name = std::string("INTERNAL_FLOAT_TMP_") + std::to_string(tmp_count);
			tmp_count++;
			Symbol tmp_symbol{};
			tmp_symbol.type = Symbol::FLOAT;
			tmp_symbol.declaration_line = lookAheadToken->lineNumber;
			tmp_symbol.as_float = std::stof(lookAheadToken->value);
			tmp_symbol.name = name;

			// match(NUM);
			nextToken();
			symbolTable.insert({name, tmp_symbol});
			return &symbolTable[name];
		}
		else
		{
			error_line = "Invalid number!";
			return nullptr;
		}
	}
	error_line = "Failed to match a number";
	return nullptr;
}

void printSymbolTable()
{
	std::cout << "************************************\n";
	std::cout << "Symbol Table: \n";
	for (auto& symbols : symbolTable)
	{
		std::cout << symbols.second << '\n';
	}
	std::cout << "************************************\n";
}


int main(int argc, const char *argv[])
{
	const char *intermediateFile = "testFile.txt";
	const char *inputFile = argc >= 2 ? argv[1] : "input.c";

	{
		FILE* file = fopen(inputFile, "r");
		if (file == nullptr)
		{
			printf("Input File not found!\n");
			return 1;
		}
		fclose(file);
	}

	if (lexAnalysis(intermediateFile, inputFile))
	{
		printf("Error in syntax analysis\n");
		remove(intermediateFile);
		return 1;
	}

	std::ifstream inputStream(intermediateFile);

	Token *list = nullptr;
	Token *lastToken = nullptr;
	while (!inputStream.eof())
	{
		std::string token, value;
		uint32_t lineNum;

		inputStream >> token;
		if (token.empty() == false)
		{
			if (token == "ID" || token == "NUM")
			{
				inputStream >> value;
			}

			inputStream >> lineNum;
			if (list == nullptr)
			{
				list = new Token{token, value, lineNum, nullptr};
				lastToken = list;
			}
			else
			{
				lastToken->next = new Token{token, value, lineNum, nullptr};
				lastToken = lastToken->next;
			}
		}
	}
	inputStream.close();

	lookAheadToken = list;
	if (program() == false)
	{
		if (error_line.empty())
			std::cerr << match_error << '\n';
		else
			std::cerr << error_line << '\n';
	}
	else
		std::cout << "Parsing has completed successfully!\n";

	removeTmps();
	printSymbolTable();


	// remove(intermediateFile);

	return 0;
}

bool program()
{
	return type_specifier() && match("main")
	&& match(LPAREN) && params()
	&& match(RPAREN) && match(LBRACE) && declaration_list() && statement_list() && match(RBRACE);
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
	if (type_specifier())
	{
 		Symbol* symbol = match_undeclared();
		if (symbol == nullptr)
		{
			return false;
		}
		int size = var_declaration_tail();
		if (size == 0)
			return false;

		symbol->size = size;
		if (size != 1)
		{
			symbol->size = size;
			symbol->type = Symbol::POINTER;
			symbol->as_pointer = new Symbol[size];
			for(int i = 0; i < size; i++)
			{
				symbol->as_pointer[i].type = lastType;
				symbol->as_pointer[i].size = 1;
				symbol->as_pointer[i].name = symbol->name + "_" + std::to_string(i);
				symbol->as_pointer[i].declaration_line = symbol->declaration_line;
			}
		}
		return true;
	}
	else
	{
		error_line = "";
		return false;
	}
}

int var_declaration_tail()
{
	if (match(SEMICOLON))
		return 1;
	else if (match(LBRACKET))
	{
		Symbol* number = match_number();

		if (number != nullptr && number->type != Symbol::INT)
		{
			error_line = "Invalid Array size in line #" + std::to_string(lineNumber);
			return 0;
		}
		if (match(RBRACKET) && match(SEMICOLON))
			return number->as_int;
	}
	return 0;
}

bool type_specifier()
{
	if (match("int"))
	{
		lastType = Symbol::TYPE::INT;
		return true;
	}
	else if (match("float"))
	{
		lastType = Symbol::TYPE::FLOAT;
		return true;
	}
	error_line = "Unmatched type specifier found: " + lookAheadToken->txt + " line number #" + std::to_string(lineNumber);
	return false;
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
	return assignment_statement() || compound_stmt() || selection_statement() ||
			iteration_statement();
}

bool selection_statement()
{
	Token* currentToken = lookAheadToken;
	bool parsable =  match("if") &&
			match(LPAREN) &&
			expression() &&
			match(RPAREN) &&
			statement() &&
			selection_statement_tail();

	if (!shouldExecute)
		return parsable;
	
	lookAheadToken = currentToken;

	match("if");
	match(LPAREN);
	
	expression();
	if (left_v && right_v)
	{
		if (relationOp == LT)
			shouldExecute = left_v < right_v;
		else if (relationOp == GT)
			shouldExecute = left_v > right_v;
		else if (relationOp == EQ)
			shouldExecute = left_v == right_v;
		else if (relationOp == NEQ)
			shouldExecute = left_v != right_v;
		else
			shouldExecute = false;
	}
	else if (left_v && !right_v)
		shouldExecute = left_v->as_int != 0;
	else if (!left_v && right_v)
		shouldExecute = right_v->as_int != 0;
	else
		shouldExecute = false;


	match(RPAREN);
	statement();
	shouldExecute = ~shouldExecute; 
	selection_statement_tail();

	shouldExecute = true;
	// TODO: FIX THIS
	return true;
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
	Token* currentToken = lookAheadToken;
	bool parsable = match("while") &&
           match(LPAREN) &&
           expression() &&
           match(RPAREN) &&
           statement();

	if (!shouldExecute)
		return parsable;
	
	do
	{
		lookAheadToken = currentToken;
		match("while");
		match(LPAREN);
		expression();

		// outputOfExpression = lastSymbol->as_int != 0;
		if (left_v && right_v)
		{
			if (relationOp == LT)
				shouldExecute = left_v < right_v;
			else if (relationOp == GT)
				shouldExecute = left_v > right_v;
			else if (relationOp == EQ)
				shouldExecute = left_v == right_v;
			else if (relationOp == NEQ)
				shouldExecute = left_v != right_v;
			else
				shouldExecute = false;
		}
		else if (left_v && !right_v)
			shouldExecute = left_v->as_int != 0;
		else if (!left_v && right_v)
			shouldExecute = right_v->as_int != 0;
		
		match(RPAREN);
		statement();
		// removeTmps();
	} while(shouldExecute);

	shouldExecute = true;
	return true;
}

bool assignment_statement()
{
	Token* currentToken = lookAheadToken;
	bool parsable = var() &&
           match(ASSIGN) &&
     	      expression() && match(SEMICOLON);
	if (!shouldExecute)
	    return parsable;

	lookAheadToken = currentToken;
	if (var())
	{
		if (match(ASSIGN))
		{
			return expression() && match(SEMICOLON);
		}
	}
	return false;
}

bool var()
{
	if (!shouldExecute)
		return match(ID) &&
           var_tail();

	Symbol* symbol = match_declared();
	if (symbol == nullptr)
		return false;
	lastSymbol = symbol;

	return var_tail();
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
	left_v = nullptr;
	right_v = nullptr;
	return additive_expression() && expression_tail();
}

bool expression_tail()
{
	Symbol* sym = lastSymbol;
	if (isNext(LT) || isNext(GT) || isNext(EQ) || isNext(NEQ))
	{
		lastSymbol = nullptr;
		bool out = relop() && additive_expression() && expression_tail();
		lastSymbol = sym;
		return out;
	}
	else
		return true;
}

bool relop()
{
	if (isNext(LT))
	{
		bool out = (match(LT) && relop_tail());
		if (!out)
			return false;
		relationOp = LT;
		return true;
	}	
else if (isNext(GT))
	{
		bool out = (match(GT) && relop_tail());
		if (!out)
			return false;
		relationOp = GT;
		return true;
	}
	else if (isNext(EQ))
	{
		bool out = match(EQ);
		if (!out)
			return false;
		relationOp = EQ;
		return true;
	}
	else
	{
		bool out = match(NEQ);
		if (!out)
			return false;
		relationOp = NEQ;
		return true;
	}
}

bool relop_tail()
{
	if (isNext(ASSIGN))
		return match(ASSIGN);
	return true;
}

bool additive_expression()
{
	bool parsable = term() && additive_expression_tail();
	return parsable;
}

bool additive_expression_tail()
{
	if (isNext(PLUS) || isNext(MINUS))
		return addop() && term() && additive_expression_tail();

	return true;
}

bool addop()
{
	if (!shouldExecute)
	    if (isNext(PLUS))
        	return (match(PLUS));
    	else
        	return (match(MINUS));
	if (isNext(PLUS))
	{
		if (match(PLUS))
		{
			operation = "+";
			return true;
		}
		return false;
	}
	else
	{
		if (match(MINUS))
		{
			operation = "-";
			return true;
		}
		return false;
	}
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
	if (!shouldExecute)
	{
		if (isNext(MULT))
			return (match(MULT));
		else
			return (match(DIV));
	}

	if (isNext(MULT))
	{
		if (match(MULT))
		{
			operation = "*";
			return true;
		}
		return false;
	}
	else
	{
		if (match(DIV))
		{
			operation = "/";
			return true;
		}
		return false;
	}

}

bool factor()
{
	if (!shouldExecute)
	{
		// Just parse
		if (isNext(NUM))
			return match(NUM);
		else if (isNext(ID))
			return match(ID);
		else
			return match(LPAREN) && expression() && match(RPAREN);
	}
	else {
		// Now we need to start doing something
		if (isNext(NUM))
		{
			auto numberSym = match_number();
			if (numberSym == nullptr)
				return false;

			if (left_v == nullptr)
				left_v = numberSym;
			else if (right_v == nullptr)
				right_v = numberSym;
			
			if (lastSymbol == nullptr)
			{
				lastSymbol = numberSym;
				return true;
			}

			// std::cout << "TYPES: " << symbolTypes[lastSymbol->type] << ", " << symbolTypes[numberSym->type] << '\n';
			if (lastSymbol->type != numberSym->type)
			{
				std::cout << "Invalid type!. Expeected: " + (lastSymbol->type == Symbol::INT) ? "int" : "float";
				error_line = "Invalid type!. Expeected: " + (lastSymbol->type == Symbol::INT) ? "int" : "float";
				return false;
			}
			switch(numberSym->type){
				case Symbol::FLOAT:
					if (operation.empty()) lastSymbol->as_float = numberSym->as_float;
					else if (operation == "+") lastSymbol->as_float += numberSym->as_float;
					else if (operation == "-") lastSymbol->as_float -= numberSym->as_float;
					else if (operation == "*") lastSymbol->as_float *= numberSym->as_float;
					else if (operation == "/") {
						if (numberSym->as_float == 0)
						{
							std::cerr << "Division by zero exception! #" << lineNumber << " terminating.\n";
							exit(10);
						}
						lastSymbol->as_float /= numberSym->as_float;
					}
				break;
				case Symbol::INT:
					if (operation.empty()) lastSymbol->as_int = numberSym->as_int;
					else if (operation == "+") lastSymbol->as_int += numberSym->as_int;
					else if (operation == "-") lastSymbol->as_int -= numberSym->as_int;
					else if (operation == "*") lastSymbol->as_int *= numberSym->as_int;
					else if (operation == "/") {
						if (numberSym->as_int == 0)
						{
							std::cerr << "Division by zero exception! #" << lineNumber << " terminating.\n";
							exit(10);
						}
						lastSymbol->as_int /= numberSym->as_int;
					}
				break;
				default:
					// error?
					break;
			}
			operation = "";
			return true;
		}
		else if (isNext(ID))
		{
			auto idSymbol = match_declared();
			if (idSymbol == nullptr)
				return false;

			if (right_v == nullptr)
				right_v = idSymbol;
			else if (left_v == nullptr)
				left_v = idSymbol;
			if (lastSymbol == nullptr)
			{
				lastSymbol = idSymbol;
				return true;
			}
			
			if (lastSymbol->type != idSymbol->type)
			{
				std::cout << "Invalid type!. Expeected: " + (lastSymbol->type == Symbol::INT) ? "int" : "float";
				error_line = "Invalid type!. Expeected: " + (lastSymbol->type == Symbol::INT) ? "int" : "float";
				return false;
			}
			switch(idSymbol->type){
				case Symbol::FLOAT:
					if (operation.empty()) lastSymbol->as_float = idSymbol->as_float;
					else if (operation == "+") lastSymbol->as_float += idSymbol->as_float;
					else if (operation == "-") lastSymbol->as_float -= idSymbol->as_float;
					else if (operation == "*") lastSymbol->as_float *= idSymbol->as_float;
					else if (operation == "/") {
						if (idSymbol->as_float == 0.0f)
						{
							std::cerr << "Division by zero exception! #" << lineNumber << " terminating.\n";
							exit(10);
						}
						lastSymbol->as_float /= idSymbol->as_float;
					}
				break;
				case Symbol::INT:
					if (operation.empty()) lastSymbol->as_int = idSymbol->as_int;
					else if (operation == "+") lastSymbol->as_int += idSymbol->as_int;
					else if (operation == "-") lastSymbol->as_int -= idSymbol->as_int;
					else if (operation == "*") lastSymbol->as_int *= idSymbol->as_int;
					else if (operation == "/") {
						if (idSymbol->as_int == 0)
						{
							std::cerr << "Division by zero exception! #" << lineNumber << " terminating.\n";
							exit(10);
						}
						lastSymbol->as_int /= idSymbol->as_int;
					}
				break;
				default:
					// error?
					break;
			}
			operation = "";
			return true;
		}
		else
			return match(LPAREN) && expression() && match(RPAREN);
	}
}
