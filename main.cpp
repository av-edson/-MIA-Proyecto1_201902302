#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <array>
#include "Analizer/scanner.h"
#include "Analizer/parser.h"


using namespace std;
extern int yyparse();
extern std::array<std::string, 11> getDatos();


void mostrarMenu();
string pedirEntrada();
void leerEntrada(string entrada);
void mostrarDatos(std::array<std::string, 11> lista);

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
    file.open("Analizer/encabezado.txt", ios::in);

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
    cout << "\n-> ";
    getline(cin, input);
    return input;
}

void leerEntrada(string entrada){
    cout << "       leyendo--" << entrada  << endl;
    YY_BUFFER_STATE buffer = yy_scan_string(entrada.c_str());
    if (yyparse() == 0)
    {
        std::array<std::string, 11> datos;
        datos = getDatos();
        mostrarDatos(datos);
    }else{
        cout << "\n !!! ocurrio un error al leer !! \n";
    }
}

void mostrarDatos(std::array<std::string, 11> lista){
    cout << endl;
    for (int i = 0; i < 11; i++)
    {
       cout << lista[i] << "|";
    }
    cout << endl;
    
}
