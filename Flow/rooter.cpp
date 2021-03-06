#include "rooter.h"

using namespace std;

bool rootMkdisk(std::array<std::string, 11> args);
bool rootFdisk(std::array<std::string, 11> args);


void readArguments(std::array<std::string, 11> lista){
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

bool rootFdisk(std::array<std::string, 11> args){
    try{
        int tam = atoi(args[1].c_str());
        return fdiskF(tam, args[3], args[4],args[5],args[2],args[6],atoi(args[7].c_str()),args[10]);
    }catch (char pr){
        cout << "   -- Ocurrio un error al crear la particion --" << endl;
        return false;
    }
}