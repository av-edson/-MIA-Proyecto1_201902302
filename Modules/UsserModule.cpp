//
// Created by edson on 13/03/21.
//
#include "usserModule.h"
#include "list"
#include "array"
using namespace std;

bool obtenerMontada(string id,list<montadas> *listaMontadas, montadas *montada){
    montadas aux;
    for (montadas mt: *listaMontadas){
        if (mt.getId() == id){
            *montada = mt;
            return true;
        }
    }
    return false;
}

SUPERBLOQUE getSuperBloques(string path, string nombreDisk){
    FILE *archivo = fopen(path.c_str(), "rb+");
    MBR tempDisk{};
    fread(&tempDisk, sizeof(tempDisk), 1, archivo);
    SUPERBLOQUE super{};
    for (PARTITION part:  tempDisk.mbr_partition){
        if (part.part_name == nombreDisk){
            fseek(archivo, part.part_start+1, SEEK_SET);
            fread(&super, sizeof(super), 1, archivo);
            break;
        }
    }
    fclose(archivo);
    return super;
}

list<array<string,5>> dividirString(list<char> contenido){
    list<array<string,5>> total;
    array<string,5> aux;
    int index=0;
    string anteriores;
    for (char ch:contenido){
        if (ch==','){
            aux[index] = anteriores;
            index++;
            anteriores ="";
        }else if (ch=='\n'){
            aux[index] = anteriores;
            total.push_back(aux);
            aux[0] = "";
            aux[1] = "";
            aux[2] = "";
            aux[3] = "";
            aux[4] = "";
            index=0;
        }else{
            anteriores.append(string(1,ch));
        }
    }
    return total;
}

bool loginUsser(string usser, string password, string idPart, list<montadas> *listaMontadas, std::string *usuarioActual){
    if (!usuarioActual->empty()){
        cout << "   - Se encuentra otra sesion activa-"<< endl;
        return false;
    }
    montadas montada{}; SUPERBLOQUE superbloque;
    if (!obtenerMontada(idPart, listaMontadas, &montada)){
        cout << "   - No existe la particion ingresada-"<< endl;
        return false;
    }
    FILE *archivo = fopen(montada.getPath().c_str(), "rb+");
    superbloque = getSuperBloques(montada.getPath(), montada.getName());
    fseek(archivo, superbloque.s_inode_start+ sizeof(INODO), SEEK_SET);
    INODO inodoArchivo{};
    fread(&inodoArchivo, sizeof(inodoArchivo), 1, archivo);
    // almacenamos el contenido
    list<char> contenido; int index=0;
    while (inodoArchivo.i_block[index] != -1){
        FILEBLOCK bloqueArchivo{};
        int inicio = superbloque.s_block_start+(sizeof(FILEBLOCK)*inodoArchivo.i_block[index]);
        fseek(archivo, inicio, SEEK_SET);
        fread(&bloqueArchivo, sizeof(bloqueArchivo), 1, archivo);
        index++;
        for (int i = 0; i < 64; ++i) {
            if (bloqueArchivo.b_content[i]=='\0'){
                break;
            }
            contenido.push_back(bloqueArchivo.b_content[i]);
        }
    }
    list<array<string,5>> matriz =dividirString(contenido);
    for (array<string,5> st: matriz){
        if (st[0]!="0" && st[1]=="U" && st[3]==usser){
            if (st[4]==password){
                cout << "       - Login exitoso-" << endl;
                usuarioActual->append(usser);
                return true;
            }else{
                cout << "       -Contraseña no coicide -" << endl;
                return false;
            }
        }
    }
    return false;
}

bool logoutUsser(string *usuarioActual){
    if (!usuarioActual->empty()){
        usuarioActual->clear();
        return true;
    }else{
        cout << "   - No existe sesion activa-"<<endl;
        return false;
    }
}
