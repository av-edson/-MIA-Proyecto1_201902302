#include <FlexLexer.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

int main(){
    ifstream in("entrada.txt");
    yyFlexLexer* lexer = new yyFlexLexer(&in);
    while (lexer->yylex() != 0)
            ;
    return 0;
}
