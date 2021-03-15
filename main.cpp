#include <iostream>
#include <fstream>
#include <array>
#include "Analizer/scanner.h"
#include "Analizer/parser.h"
#include "Flow/rooter.h"
#include "list"
#include "montadas.h"

using namespace std;
extern std::array<std::string, 12> getDatos();
extern void cleanEs();

void mostrarMenu();
string pedirEntrada();
void leerEntrada(string entrada, list<montadas> *listaMontadas, string *usuarioActual, string *idParticionActiba);
void mostrarDatos(std::array<std::string, 12> lista);
void readFile(string path, list<montadas> *listaMontadas, string *usuarioActual, string *idParticionActiba);

int main(){
    list<montadas> listaMontadas;
    string usuarioActual="";
    string idPartActiva="";
    mostrarMenu();
    while (true)
    {
        leerEntrada(pedirEntrada(), &listaMontadas, &usuarioActual, &idPartActiva);
    }
    return 0;
}

void mostrarMenu(){
   cout << "             __^__                                      __^__"<< endl;
   cout << "            ( ___ )------------------------------------( ___ )"<< endl;
   cout << "             | / |             MIA 1S 2021              | \\ |"<< endl;
   cout << "             | / |       EDSON SAUL AVILA ORTIZ         | \\ |"<< endl;
   cout << "             |___|         PROYECTO 1 201902302         |___|"<< endl;
   cout << "            (_____)------------------------------------(_____) " << endl;
}

string pedirEntrada(){
    string input;
    cout << "\n-> ";
    getline(cin, input);
    return input;
}

void leerEntrada(string entrada, list<montadas> *lista, string *usuarioActual, string *idParticion){
    // cout << "       leyendo--" << entrada  << endl;
    YY_BUFFER_STATE buffer = yy_scan_string(entrada.c_str());
    if (yyparse() == 0)
    {
        std::array<std::string, 12> datos;
        datos = getDatos();
        if (datos[0] == "7")
        {
            readFile(datos[4], lista, usuarioActual, idParticion);
        }
        else{
            // mandando el array para su procesamiento

            readArguments(datos, lista, usuarioActual, idParticion);
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

void readFile(string path, list<montadas> *lista, string *usuarioActual, string *idParticion){
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
                    readArguments( getDatos(), lista, usuarioActual, idParticion);
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