#include <stdio.h>

extern int lexAnalysis(const char* outFile, const char* inputFile);
extern int yaccParse(const char* outFile, const char* inputFile);

int main()
{
    // lexAnalysis("test.txt", "/media/seif-sallam/0EEC783DEC78215F/C--Compiler/Yacc/input.c");
    yaccParse("Parser.txt", "text.txt");
    return 0;
}