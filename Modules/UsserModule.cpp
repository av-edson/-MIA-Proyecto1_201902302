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

int getInicioParticion(string path, string nombreDisk){
    FILE *archivo = fopen(path.c_str(), "rb+");
    MBR tempDisk{};
    fread(&tempDisk, sizeof(tempDisk), 1, archivo);
    for (PARTITION part:  tempDisk.mbr_partition){
        if (part.part_name == nombreDisk){
            return part.part_start;
            break;
        }
    }
    fclose(archivo);
    return 0;
}

list<char> getContenidoUsser(string idPart, montadas montadaE){
    SUPERBLOQUE superbloque;
    FILE *archivo = fopen(montadaE.getPath().c_str(), "rb+");
    superbloque = getSuperBloques(montadaE.getPath(), montadaE.getName());
    fseek(archivo, superbloque.s_inode_start+ sizeof(INODO), SEEK_SET);
    INODO inodoArchivo{};
    fread(&inodoArchivo, sizeof(inodoArchivo), 1, archivo);
    // almacenamos el contenido
    list<char> contenido; int index=0;
    while (inodoArchivo.i_block[index] != -1){
        FILEBLOCK bloqueArchivo{};
        int inicio = superbloque.s_block_start + (sizeof(FILEBLOCK)*inodoArchivo.i_block[index]);
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
    return contenido;
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
            anteriores="";
        }else{
            anteriores.append(string(1,ch));
        }
    }
    return total;
}

bool loginUsser(string usser, string password, string idPart, list<montadas> *listaMontadas, std::string *usuarioActual, string *idParticionActiba){
    if (!usuarioActual->empty()){
        cout << "   - Se encuentra otra sesion activa-"<< endl;
        return false;
    }
    montadas montada{};
    if (!obtenerMontada(idPart, listaMontadas, &montada)){
        cout << "   - No existe la particion ingresada-"<< endl;
        return false;
    }
    list<char> contenido = getContenidoUsser(idPart, montada);
    list<array<string,5>> matriz =dividirString(contenido);
    for (array<string,5> st: matriz){
        if (st[0]!="0" && st[1]=="U" && st[3]==usser){
            if (st[4]==password){
                cout << "       - Login exitoso-" << endl;
                usuarioActual->append(usser);
                idParticionActiba->append(montada.getId());
                return true;
            }else{
                cout << "       -Contraseña no coicide -" << endl;
                return false;
            }
        }
    }
    return false;
}

bool logoutUsser(string *usuarioActual, string *idParticionActiba){
    if (!usuarioActual->empty()){
        usuarioActual->clear();
        idParticionActiba->clear();
        return true;
    }else{
        cout << "   - No existe sesion activa-"<<endl;
        return false;
    }
}

string contenidoUssers(int NoBloques, list<char>contenido ){
    int indixe = 0;
    int inicio = 64*NoBloques;
    string aux="";
    for (char ch: contenido){
        if (indixe >= inicio && indixe < inicio+64){
            aux.append(string(1,ch));
        }
        indixe++;
    }
    return aux;
}

FILEBLOCK llenarBloque(string contenido){
    FILEBLOCK auxB{};
    strcpy(auxB.b_content, contenido.c_str());
    return auxB;
}

void reescribirUsers(montadas montada, list<char> contenido) {
    try{
        SUPERBLOQUE super = getSuperBloques(montada.getPath(), montada.getName());
        INODO inodo{};
        FILEBLOCK bloque{};
        FILE *archivo = fopen(montada.getPath().c_str(), "rb+");
        fseek(archivo, super.s_inode_start + sizeof(INODO), SEEK_SET);
        fread(&inodo, sizeof(inodo), 1, archivo);
        double noBloquesNecesarios =0;
        noBloquesNecesarios=(double) contenido.size() / 64;
        int bloquesFaltantes=0;
        for (int i = 0; i < 10; ++i) {
            if (noBloquesNecesarios < i){
                bloquesFaltantes=i;
                break;
            }
        }
        fseek(archivo, super.s_block_start + sizeof(CARPETABLOCK) * inodo.i_block[0], SEEK_SET);
        fread(&bloque, sizeof(bloque), 1, archivo);
        for (int i = 0; i < 15; ++i) {
            if (inodo.i_block[i] == -1 && bloquesFaltantes == 0) {
                break;
            } else if (inodo.i_block[i] == -1 && bloquesFaltantes > 0) {
                // reescribir blouques
                FILEBLOCK nuevoBloque{};
                strcpy(nuevoBloque.b_content, contenidoUssers(i, contenido).c_str());
                inodo.i_block[i] = super.s_first_blo;
                fseek(archivo, super.s_block_start + sizeof(CARPETABLOCK) * inodo.i_block[i], SEEK_SET);
                fwrite(&nuevoBloque, sizeof(nuevoBloque), 1,archivo);
                fseek(archivo, super.s_bm_block_start+super.s_first_blo, SEEK_SET);
                fwrite("1",1,1,archivo);
                super.s_first_blo +=1;
                super.s_free_blocks_count--;
                bloquesFaltantes--;
                // escribo bloque
            } else {
                //strcpy(bloque.b_content, contenidoUssers(i, contenido).c_str());
                FILEBLOCK temp = llenarBloque(contenidoUssers(i, contenido));
                fseek(archivo, super.s_block_start + sizeof(CARPETABLOCK) * inodo.i_block[i], SEEK_SET);
                fwrite(&temp, sizeof(temp), 1,archivo);
                bloquesFaltantes--;

            }
        }
        // reescribo inodo
        fseek(archivo, super.s_inode_start + sizeof(INODO), SEEK_SET);
        fwrite(&inodo, sizeof(inodo),1,archivo);
        // reescribir superbloque
        fseek(archivo, getInicioParticion(montada.getPath(),montada.getName())+1, SEEK_SET);
        fwrite(&super, sizeof(super), 1, archivo);
        fclose(archivo);
    }catch (char pr){
        cout << "   -- Ocurrio un error al crear la particion --" << endl;
    }
}

bool mkGroupUsser(string nombreGrupo, list<montadas> *listaMontadas, std::string *usuarioActual, string *idParticionActiba){
    try {
        montadas montada{};
        string id = *idParticionActiba;
        if (!obtenerMontada(id, listaMontadas, &montada)){
            cout << "   - No existe la particion ingresada-"<< endl;
            return false;
        }
        if (*usuarioActual != "root"){
            cout << "   -Comando Solo Admitido para usuario root-"<< endl;
            return false;
        }
        list<char> contenido = getContenidoUsser(*idParticionActiba, montada);
        list<array<string,5>> matriz =dividirString(contenido);
        int noGrupo=0;
        for (array<string,5> temp: matriz){
            if (temp[1] == "G" && temp[2]==nombreGrupo){
                cout << "   -Ya existe l grupo ingresado-"<<endl;
                return false;
            }
            if (temp[0] != "0" && noGrupo < atoi(temp[0].c_str()) && temp[1] == "G"){
                noGrupo = atoi(temp[0].c_str());
            }
        }
        noGrupo++;
        string nuevoGrupo = to_string(noGrupo) + ",G,"+nombreGrupo+"\n";
        char charNuevo[nuevoGrupo.length()];
        strcpy(charNuevo, nuevoGrupo.c_str());
        for (char c:charNuevo){
            contenido.push_back(c);
        }
        reescribirUsers(montada,contenido);
        return true;
    }
    catch (char pr){
        cout << "   -- Ocurrio un error al crear la particion --" << endl;
        return false;
    }
}

bool mkUsserVoid(string nombreUsuario, string grupoUsuario, string passUsuario, list<montadas> *listaMontadas, std::string *usuarioActual, string *idParticionActiba){
    try{
        montadas montada{};
        string id = *idParticionActiba;
        if (!obtenerMontada(id, listaMontadas, &montada)){
            cout << "   - No existe la particion ingresada-"<< endl;
            return false;
        }
        if (*usuarioActual != "root"){
            cout << "   -Comando Solo Admitido para usuario root-"<< endl;
            return false;
        }
        list<char> contenido = getContenidoUsser(*idParticionActiba, montada);
        list<array<string,5>> matriz =dividirString(contenido);
        int noUsuario=0; bool existeGrupo=false;
        for (array<string,5> temp: matriz){
            if (temp[1]=="G" && temp[2]==grupoUsuario){
                existeGrupo = true;
            }
            if (temp[1] == "U" && temp[3]==nombreUsuario){
                cout << "   -Ya existe el usuario ingresado-"<<endl;
                return false;
            }
            if (temp[0] != "0" && noUsuario < atoi(temp[0].c_str()) && temp[1] == "U"){
                noUsuario = atoi(temp[0].c_str());
            }
        }
        if (!existeGrupo){
            cout <<"    -No existe grupo ingresado para el usuario-"<< endl;
            return false;
        }
        noUsuario++;
        string nuevoGrupo = to_string(noUsuario) + ",U,"+grupoUsuario+","+nombreUsuario+","+passUsuario+"\n";
        char charNuevo[nuevoGrupo.length()];
        strcpy(charNuevo, nuevoGrupo.c_str());
        for (char c:charNuevo){
            contenido.push_back(c);
        }
        reescribirUsers(montada,contenido);
        return true;
    }
    catch (char pr){
        cout << "   -- Ocurrio un error al crear la particion --" << endl;
        return false;
    }
}
