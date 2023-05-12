#include <iostream>
#include <string>
#include <fstream>

#include "parser.h"

extern int lexAnalysis(const char *outFile, const char *inputFile);

int main(int argc, const char *argv[])
{
    const char *intermediateFile = "testFile.txt";
    const char *inputFile = argc >= 2 ? argv[1] : "input.c";
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
        std::cerr << "Syntax error line # " << lookAheadToken->lineNumber << ", expected token (" << expectedToken << "), but got "
                  << "(" << lookAheadToken->txt << ")\n";
    }
    else
        std::cout << "Parsing has completed successfully!\n";

    remove(intermediateFile);

    return 0;
}