#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <array>
#include "Analizer/scanner.h"
#include "Analizer/parser.h"
#include "Flow/rooter.h"


using namespace std;
extern int yyparse();
extern std::array<std::string, 11> getDatos();
extern void cleanEs();


void mostrarMenu();
string pedirEntrada();
void leerEntrada(string entrada);
void mostrarDatos(std::array<std::string, 11> lista);
void readFile(string path);

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
    // cout << "       leyendo--" << entrada  << endl;
    YY_BUFFER_STATE buffer = yy_scan_string(entrada.c_str());
    if (yyparse() == 0)
    {
        std::array<std::string, 11> datos;
        datos = getDatos();
        // pidiendo array de los datos almacenados por los analizadores
        mostrarDatos(datos);

        
        // vemos si se trata de una lextura de archivo
        if (datos[0] == "7")
        {

            readFile(datos[4]);
        }
        else{
            // mandando el array para su procesamiento
            readArguments(datos);
        }
        
        
    }else{
        cout << "!!! ocurrio un error al leer !! \n";
    }
    cleanEs();
}

void mostrarDatos(std::array<std::string, 11> lista){
    for (int i = 0; i < lista.size(); i++)
    {
       cout << i << ". " <<lista[i] << "|";
    }
    cout << "\n";
}

void readFile(string path){
    ifstream archivo(path);
    if (archivo.good())
    {
        for (std::string linia; getline(archivo, linia);)
        {   
            cout << "Leyendo:->" <<  linia  <<  endl;
            if (linia.size() > 4)
            {
                YY_BUFFER_STATE buffer = yy_scan_string(linia.c_str());
                if (yyparse() == 0)
                {
                    // std::array<std::string, 11> datos;
                    // datos = getDatos();
                    // // pidiendo array de los datos almacenados por los analizadores
                    // mostrarDatos(datos);
                    // mandando el array para su procesamiento
                    readArguments( getDatos());
                    cout << endl;
                }
                else{
                    cout << "\n!!! ocurrio un error al leer !!\n";
                }
            }
            cleanEs();
        }
        
    }
    else{
        cout << "       -> El archivo de Entrada NO Existe <- " << endl;
    }
    
}
