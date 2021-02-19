#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include "Analizer/parser.h"
#include "Analizer/scanner.h"


using namespace std;
extern int yyparse();

void mostrarMenu();
string pedirEntrada();
void leerEntrada(string entrada);

int main(){
    mostrarMenu();
    while (true)
    {
        leerEntrada(pedirEntrada());
    }
    
    return 0;
}

void mostrarMenu(){
    ifstream file;
    string content;
    file.open("encabezado.txt", ios::in);

    if (file.fail())
    {
        cout << " -----------------   " << endl;
    }else{
        while (!file.eof())
        {
            getline(file, content);
            cout << content << endl;
        }
        
    }
}

string pedirEntrada(){
    string input;
    cout << "-> ";
    getline(cin, input);
    return input;
}

void leerEntrada(string entrada){
    cout << "       leyendo--" << entrada  << endl;
    YY_BUFFER_STATE buffer = yy_scan_string(entrada.c_str());
    if (yyparse() == 0)
    {
        // cout << "jalo" << endl;
    }
}

