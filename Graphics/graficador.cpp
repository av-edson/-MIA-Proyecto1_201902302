//
// Created by edson on 10/03/21.
//
#include "graficador.h"
#include "../Estructs/diskStructs.h"
#include "../Estructs/fileSistemStructs.h"
#include <stdio.h>
#include "list"
#include "fstream"
using namespace std;
string getNombrePath(string path);
extern bool obtenerMontada(string id,list<montadas> *listaMontadas, montadas *montada);
extern list<char> getContenidoUsser(string idPart, montadas montadaE);

void exportar(string contenido,string nombre){
    ofstream salida;
    salida.open("/home/edson/CLionProjects/Proyecto/resultados/"+nombre);
    salida << contenido;
    salida.close();
}

EBR getLogica(std::string path, int inicioExtendida){
    FILE *archivo = fopen(path.c_str(), "rb+");
    EBR temp{};
    fseek(archivo, inicioExtendida , SEEK_SET);
    fread(&temp, sizeof(temp), 1, archivo);
    fclose(archivo);
    return temp;
}

void graficarMBR(std::string path, std::string idDisk){
    FILE *archivo = fopen(path.c_str(), "rb+");
    MBR tempDisk{};
    fread(&tempDisk, sizeof(tempDisk), 1, archivo);
    string contenido = "digraph D{node [ shape=none, margin=0 ]\n";
    contenido.append("nuevo [label=<<TABLE  BORDER=\"1\" CELLBORDER=\"1\">\n<tr><td>Nombre</td><td>Valor</td></tr>\n");
    contenido.append("<tr><td colspan=\"2\" BGCOLOR=\"#9b9b9b\">"+getNombrePath(path)+"</td></tr>\n");
    contenido.append("<tr><td>mbr_fecha_creacion</td><td>");
    contenido.append(asctime(gmtime(&tempDisk.mbr_fecha_creacion))); contenido.append("</td></tr>\n");
    contenido.append("<tr><td>mbr_disk_signature</td><td>"+to_string(tempDisk.mbr_disk_signature)+"</td></tr>\n");
    contenido.append("<tr><td>disk_fit</td><td>"+to_string(tempDisk.disk_fit)+"</td></tr>\n");
    string aux; PARTITION extendida{};
    for (PARTITION particion: tempDisk.mbr_partition){
        contenido.append("<tr><td>part_name</td><td>");
        contenido.append(particion.part_name); contenido.append("</td></tr>\n");
        contenido.append("<tr><td>part_status</td><td>"+std::string(1,particion.part_status)+"</td></tr>\n");
        contenido.append("<tr><td>part_type</td><td>"+std::string(1,particion.part_type)+"</td></tr>\n");
        contenido.append("<tr><td>part_fit</td><td>"+std::string(1,particion.part_fit)+"</td></tr>\n");
        contenido.append("<tr><td>part_start</td><td>"+to_string(particion.part_start)+"</td></tr>\n");
        contenido.append("<tr><td>part_size</td><td>"+to_string(particion.part_size)+"</td></tr>\n");
        if (particion.part_type=='e'){
            extendida = particion;
        }
    }

    EBR logica = getLogica(path, extendida.part_start+1);
    if (logica.part_star != 0 && (logica.part_fit=='b' || logica.part_fit=='f' || logica.part_fit == 'w')){
        contenido.append("</TABLE>>]\n");
    }else{
        contenido.append("</TABLE>>]\n");
    }
    while (logica.part_star != 0 && (logica.part_fit=='b' || logica.part_fit=='f' || logica.part_fit == 'w')){
        contenido.append(logica.part_name);
        contenido.append(" [label=<<TABLE  BORDER=\"1\" CELLBORDER=\"1\">\n<tr><td>Nombre</td><td>Valor</td></tr>\n");
        contenido.append("<tr><td colspan=\"2\" BGCOLOR=\"#9b9b9b\">LOGICA</td></tr>\n");
        contenido.append("<tr><td>part_status</td><td>"+string(1,logica.part_status)+"</td></tr>\n");
        contenido.append("<tr><td>part_fit</td><td>"+string(1,logica.part_fit)+"</td></tr>\n");
        contenido.append("<tr><td>part_star</td><td>"+to_string(logica.part_star)+"</td></tr>\n");
        contenido.append("<tr><td>part_size</td><td>"+to_string(logica.part_size)+"</td></tr>\n");
        contenido.append("<tr><td>part_next</td><td>"+to_string(logica.part_next)+"</td></tr>\n");
        contenido.append("<tr><td>part_name</td><td>");
        contenido.append(logica.part_name); contenido.append("</td></tr>\n");
        contenido.append("</TABLE>>]\n");
        logica = getLogica(path, logica.part_next);
    }
    contenido.append("}");
    fclose(archivo);
    exportar(contenido, "mbr.dot");
}

string recuperarLogicas(string path, EBR logica, int id, int tamano);

string getNombrePath(string path){
    list<string> listaAux;
    string aux=""; int inicio = 1; int fin=0;
    while (fin = path.find('/', inicio), fin>=0){
        aux = path.substr(inicio, fin - inicio);
        inicio = fin+1;
        listaAux.push_back(aux);
    }
    aux = path.substr(inicio, fin - inicio);
    listaAux.push_back(aux);
    return listaAux.back();
}

float getPorcentaje(int tamano1, int grande){
    float num = (float)tamano1*100;
    float res = num / (float) grande;
    return res;
}

void graficarDisk(std::string path, std::string pathResultado){

    FILE *archivo = fopen(path.c_str(), "rb+");
    MBR tempDisk{};
    fread(&tempDisk, sizeof(tempDisk), 1, archivo);
    fclose(archivo);
    string contenid="digraph {\nnode [shape=rec style=filled height=2];\ngraph [ nodesep=0.05];\n";
    contenid.append("label=\""+getNombrePath(path)+"\"\n");
    contenid.append("MBR\n");
    int inicioAnterior = 136;
    string aux = ""; int id = 1;
    for (PARTITION particion: tempDisk.mbr_partition){
        aux = "";
        if (particion.part_status == '1'){
            if (particion.part_start != inicioAnterior+1){
                float division = getPorcentaje(particion.part_start-inicioAnterior, tempDisk.mbr_tamano);
                contenid.append("\""+to_string(id)+"LIBRE\n"+to_string(division)+"% del disco\"\n");
                id++;
            }
            if (particion.part_type=='e'){
                FILE *otro = fopen(path.c_str(), "rb+");
                EBR logica{};
                fseek(otro, particion.part_start+1, SEEK_SET);
                fread(&logica, sizeof(logica), 1, otro);
                fclose(otro);
                if (logica.part_star != 0 && (logica.part_fit=='b' || logica.part_fit=='f' || logica.part_fit == 'w')){
                    contenid.append(recuperarLogicas(path, logica, id, tempDisk.mbr_tamano));
                    id++;
                }else{
                    aux.append("\""+to_string(id)+"Extendida\n");
                    aux.append(particion.part_name); aux.append("\n");
                    float division = getPorcentaje(particion.part_size, tempDisk.mbr_tamano);
                    aux.append(to_string(division)+"% del disco\"\n");
                    contenid.append(aux);
                }
            }
            if (particion.part_type=='p'){
                aux.append("\""+to_string(id)+"Primaria\n");
                aux.append(particion.part_name); aux.append("\n");
                float division = getPorcentaje(particion.part_size, tempDisk.mbr_tamano);
                aux.append(to_string(division)+"% del disco\"\n");
                contenid.append(aux);
            }
            inicioAnterior = particion.part_size + particion.part_start;
        }
    }
    float division = getPorcentaje(tempDisk.mbr_tamano-inicioAnterior, tempDisk.mbr_tamano);
    contenid.append("\""+to_string(id)+"LIBRE\n"+to_string(division)+"% del disco\"\n}");
    exportar(contenid,"disk.dot");
}

string recuperarLogicas(string path, EBR logica, int id, int tamano){
    FILE *archivo = fopen(path.c_str(), "rb+");
    string aux="subgraph cluster"+to_string(id)+"{ label=\"Extendida\"\n";
    while (logica.part_star != 0 && (logica.part_fit=='b' || logica.part_fit=='f' || logica.part_fit == 'w')){
        aux .append("\""); aux.append(logica.part_name);
        float division = getPorcentaje(logica.part_size, tamano);
        aux.append("\n"+to_string(division)+"% \"");
        fseek(archivo, logica.part_next, SEEK_SET);
        fread(&logica, sizeof(logica), 1, archivo);
    }
    fclose(archivo);
    aux.append("}\n");
    return aux;
}

SUPERBLOQUE getSuperBloque(string path, string nombreDisk){
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

void graficarSB(std::string path, std::string nombre){
    SUPERBLOQUE super = getSuperBloque(path, nombre);
    // escribiendo contenido del superbloque
    string contenido = "digraph D{node [ shape=none, margin=0 ]\n";
    contenido.append("nuevo [label=<<TABLE  BORDER=\"1\" CELLBORDER=\"1\">\n<tr><td BGCOLOR=\"#229954\">Nombre</td><td BGCOLOR=\"#229954\">Valor</td></tr>\n");
    contenido.append("<tr><td>tipo sistema</td><td>"+to_string(super.s_filesystem_type)+"</td></tr>\n");
    contenido.append("<tr><td>s_inodes_count</td><td>"+to_string(super.s_inodes_count)+"</td></tr>\n");
    contenido.append("<tr><td>s_blocks_count</td><td>"+to_string(super.s_blocks_count)+"</td></tr>\n");
    contenido.append("<tr><td>s_free_blocks_count</td><td>"+to_string(super.s_free_blocks_count)+"</td></tr>\n");
    contenido.append("<tr><td>s_free_inodes_count</td><td>"+to_string(super.s_free_inodes_count)+"</td></tr>\n");
    contenido.append("<tr><td>s_mtime</td><td>");
    contenido.append(asctime(gmtime(&super.s_mtime))); contenido.append("</td></tr>\n");
    contenido.append("<tr><td>s_umtime</td><td>");
    contenido.append(asctime(gmtime(&super.s_umtime))); contenido.append("</td></tr>\n");
    contenido.append("<tr><td>s_mnt_count</td><td>"+to_string(super.s_mnt_count)+"</td></tr>\n");
    contenido.append("<tr><td>s_magic</td><td>"+to_string(super.s_magic)+"</td></tr>\n");
    contenido.append("<tr><td>s_inode_size</td><td>"+to_string(super.s_inode_size)+"</td></tr>\n");
    contenido.append("<tr><td>s_block_size</td><td>"+to_string(super.s_block_size)+"</td></tr>\n");
    contenido.append("<tr><td>s_firts_ino</td><td>"+to_string(super.s_firts_ino)+"</td></tr>\n");
    contenido.append("<tr><td>s_first_blo</td><td>"+to_string(super.s_first_blo)+"</td></tr>\n");
    contenido.append("<tr><td>s_bm_inode_start</td><td>"+to_string(super.s_bm_inode_start)+"</td></tr>\n");
    contenido.append("<tr><td>s_bm_block_start</td><td>"+to_string(super.s_bm_block_start)+"</td></tr>\n");
    contenido.append("<tr><td>s_inode_start</td><td>"+to_string(super.s_inode_start)+"</td></tr>\n");
    contenido.append("<tr><td>s_block_start</td><td>"+to_string(super.s_block_start)+"</td></tr>\n");
    contenido.append("</TABLE>>]\n}");
    exportar(contenido,"superBloque.dot");
}

void graficarBmInode(std::string path, std::string nombre){
    SUPERBLOQUE superbloque = getSuperBloque(path, nombre);
    FILE *archivo = fopen(path.c_str(), "rb+");
    fseek(archivo, superbloque.s_bm_inode_start, SEEK_SET);
    string contenido="  BITMAP INODOS\n";
    for (int i = 0; i < superbloque.s_inodes_count; ++i) {
        if (i%20 == 0 && i!= 0) contenido.append("\n");
        char aux;
        fread(&aux, sizeof(aux),1,archivo);
        contenido.append(string(1,aux)+" ");
    }
    exportar(contenido, "bm_inodos.txt");
}

void graficarBmBlock(std::string path, std::string nombre){
    SUPERBLOQUE superbloque = getSuperBloque(path, nombre);
    FILE *archivo = fopen(path.c_str(), "rb+");
    fseek(archivo, superbloque.s_bm_block_start, SEEK_SET);
    string contenido="  BITMAP BLOQUES\n";
    for (int i = 0; i < superbloque.s_blocks_count; ++i) {
        if (i%20 == 0 && i!= 0) contenido.append("\n");
        char aux;
        fread(&aux, sizeof(aux),1,archivo);
        contenido.append(string(1,aux)+" ");
    }
    exportar(contenido, "bm_bloques.txt");
}

string contenidoUssers(list<char> contenido){
    string aux;
    for (char ch: contenido){
        aux.append(string(1,ch));
    }
    return aux;
}

void graficarUssersTxt(std::string path, std::string idPart, std::string fileName, list<montadas> *listaMontadas){
    try {
        montadas montada{};
        if (!obtenerMontada(idPart,listaMontadas, &montada)){
            cout << "   -No existe particion montada con el id ingresado-"<<endl;
            return;
        }
        if (fileName=="/users.txt"){
            string contenido = contenidoUssers(getContenidoUsser(idPart, montada));
            int tamano = contenido.size();
            cout << tamano << endl;
            exportar(contenido, "users.txt");
        }else{
            cout << "   -Funcion no terminada de implementar-"<<endl;
            return;
        }
    }catch (char pr){
        cout << "   -- Ocurrio un error al crear la particion --" << endl;
    }
}
