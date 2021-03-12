#include "rooter.h"
#include<bits/stdc++.h>
#include "list"
#include "../montadas.h"

using namespace std;

bool rootMkdisk(std::array<std::string, 11> args);
bool rootFdisk(std::array<std::string, 11> args);
bool rootGraphics(std::array<std::string, 11>args);
bool rootMount(std::array<std::string, 11>args, list<montadas> *listaMontadas);
bool rootUnMount(std::array<std::string, 11>args, list<montadas> *listaMontadas);

void readArguments(std::array<std::string, 11> lista, list<montadas> *listaMontadas){
    string opcionFuncion = lista[0];
    bool resultFuction = false;
    switch (atoi(opcionFuncion.c_str()))
    {
        case 1:
            resultFuction = rootMkdisk(lista);
            break;
        case 2:
            resultFuction = rmdiskF(lista[4]);
            break;
        case 3:
            resultFuction = rootFdisk(lista);
            break;
        case 4:
            resultFuction = rootMount(lista, listaMontadas);
            break;
        case 5:
            resultFuction = rootUnMount(lista, listaMontadas);
        case 66:
            resultFuction = rootGraphics(lista);
    }

    if (resultFuction)
    {
        cout << "       ->>> Operacion Exitosa! <<<- ";
    }
    
}

// llama al disk module para crear un disco
bool rootMkdisk(std::array<std::string, 11> args){
    int tam = atoi(args[1].c_str());
    if (tam < 1)
    {
        cout << " ----> size no adecuado para el disco <---- " << endl;
        return false;
    }
    else if (args[4] == "")
    {
                cout << " ----> No se Ingreso Ruta Para el Disco <---- " << endl; 
                return false;
    }
    else if (FILE *archivo = fopen(args[4].c_str(), "r"))
    {
                cout << " ----> El disco ya existe <---- " << endl; 
                return false;
    }
    else{
       return mkdiskF(tam, args[2], args[3], args[4]);
    }
    
    
}
std::string toLower(std::string entrada){
    transform(entrada.begin(), entrada.end(), entrada.begin(), ::tolower);
    return entrada;
}

bool rootFdisk(std::array<std::string, 11> args){
    try{
        int tam = atoi(args[1].c_str());
        args[3] = toLower(args[3]);
        args[5] = toLower(args[5]);
        args[2] = toLower(args[2]);
        return fdiskF(tam, args[3], args[4],args[5],args[2],args[6],atoi(args[7].c_str()),args[10]);
    }catch (char pr){
        cout << "   -- Ocurrio un error al crear la particion --" << endl;
        return false;
    }
}

bool rootGraphics(std::array<std::string, 11>args){
    try{
        if (args[6] == ""){

        }
        return true;
    }catch (char pr){
        cout << "   -- Ocurrio un error al generar Grafica --" << endl;
        return false;
    }
}

bool rootMount(std::array<std::string, 11>args, list<montadas> *listaMontadas){
    if (FILE *archivo = fopen(args[4].c_str(), "r"))
    {
        return montar(args[4],args[6] ,listaMontadas);
    }else{
        cout << "      -No existe disco para montar la particion-" << endl;
        return false;
    }
}

bool rootUnMount(std::array<std::string, 11>args, list<montadas> *listaMontadas){
        return desmontar(args[4],args[6] ,listaMontadas);
}
