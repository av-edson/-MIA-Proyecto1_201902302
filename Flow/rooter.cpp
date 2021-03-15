#include "rooter.h"
#include<bits/stdc++.h>
#include "list"
#include "../montadas.h"
#include "../Graphics/graficador.h"
#include "../Modules/usserModule.h"

using namespace std;

bool rootMkdisk(std::array<std::string, 12> args);
bool rootFdisk(std::array<std::string, 12> args);
bool rootGraphics(std::array<std::string, 12>args, list<montadas> *listaMontadas);
bool rootMount(std::array<std::string, 12>args, list<montadas> *listaMontadas);
bool rootUnMount(std::array<std::string, 12>args, list<montadas> *listaMontadas);
bool rootMkfs(std::array<std::string, 12>args, list<montadas> *listaMontadas);

void readArguments(std::array<std::string, 12> lista, list<montadas> *listaMontadas, string *usuarioActual,string *idParticionActiba){
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
            break;
        case 6:
            resultFuction = rootMkfs(lista, listaMontadas);
        case 66:
            resultFuction = rootGraphics(lista, listaMontadas);
            break;
        case 8:
            if (lista[7] != "-1"){
                resultFuction = loginUsser(lista[6], lista[7], lista[8], listaMontadas, usuarioActual, idParticionActiba);
            }else{
                resultFuction = loginUsser(lista[6], lista[4],lista[8], listaMontadas, usuarioActual, idParticionActiba);
            }
            break;
        case 9:
            resultFuction = logoutUsser(usuarioActual, idParticionActiba);
            break;
        case 10:
            if (lista[7] != "-1"){
                resultFuction = mkUsserVoid(lista[6], lista[8], lista[7], listaMontadas, usuarioActual, idParticionActiba);
            }else{
                resultFuction = mkUsserVoid(lista[6], lista[8],lista[4], listaMontadas, usuarioActual, idParticionActiba);
            }
            break;
        case 11:
                resultFuction = mkGroupUsser(lista[6], listaMontadas, usuarioActual, idParticionActiba);
            break;
        case 12:
            listarMontadas(listaMontadas);
            resultFuction = true;
            break;
        default:
            resultFuction = true;
    }

    if (resultFuction)
    {
        cout << "       ->>> Operacion Exitosa! <<<- ";
    }
    
}

// llama al disk module para crear un disco
bool rootMkdisk(std::array<std::string, 12>args){
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

bool rootFdisk(std::array<std::string, 12>args){
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

bool rootGraphics(std::array<std::string, 12>args, list<montadas> *listaMontadas){
    if (listaMontadas->empty()){
        cout << "   - No ha montado ninguna particion -"<< endl;
        return false;
    }

    bool encontrado = false; montadas aux;
    for (montadas mt: *listaMontadas){
        if (mt.getId() == args[8]){
            encontrado = true;
            aux = mt;
            break;
        }
    }
    if(!encontrado){
        cout << "   -No se ha montado la particion con el id ingresado- "<< endl;
        return false;
    }

    try{
        if (toLower(args[6]) == "disk"){
            graficarDisk(aux.getPath(), args[4]);
        }else if (toLower(args[6]) == "mbr"){
            graficarMBR(aux.getPath(), args[4]);
        }else if (toLower(args[6])=="sb"){
            graficarSB(aux.getPath(), aux.getName(), args[4]);
        }else if (toLower(args[6])=="bm_inode"){
            graficarBmInode(args[4],aux.getName(),aux.getPath());
        }else if (toLower(args[6])=="bm_block"){
            graficarBmBlock(args[4], aux.getName(), aux.getPath());
        }
        else if (toLower(args[6])=="file"){
            graficarUssersTxt(args[4], aux.getId(), args[11], listaMontadas);
        }
        return true;
    }catch (char pr){
        cout << "   -- Ocurrio un error al generar Grafica --" << endl;
        return false;
    }
}

bool rootMount(std::array<std::string, 12>args, list<montadas> *listaMontadas){
    if (FILE *archivo = fopen(args[4].c_str(), "r"))
    {
        return montar(args[4],args[6] ,listaMontadas);
    }else{
        cout << "      -No existe disco para montar la particion-" << endl;
        return false;
    }
}

bool rootUnMount(std::array<std::string, 12>args, list<montadas> *listaMontadas){
        return desmontar(args[8] ,listaMontadas);
}

bool rootMkfs(std::array<std::string, 12>args, list<montadas> *listaMontadas){
   try{
       return makefilesystem(args[8], args[10], args[9], listaMontadas);
   }catch (char pr){
       cout << "   -- Ocurrio un error al crear sistema --" << endl;
       return false;
   }
}
