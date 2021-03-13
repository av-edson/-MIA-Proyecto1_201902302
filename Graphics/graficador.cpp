//
// Created by edson on 10/03/21.
//
#include "graficador.h"
#include "../Estructs/diskStructs.h"
#include <stdio.h>
#include "list"
#include "fstream"
using namespace std;
string getNombrePath(string path);
void exportar(string contenido,string nombre){
    ofstream salida;
    salida.open("/home/edson/CLionProjects/Proyecto/resultados/"+nombre);
    salida << contenido;
    salida.close();
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
    string aux;
    for (PARTITION particion: tempDisk.mbr_partition){
        contenido.append("<tr><td>part_name</td><td>");
        contenido.append(particion.part_name); contenido.append("</td></tr>\n");
        contenido.append("<tr><td>part_status</td><td>"+std::string(1,particion.part_status)+"</td></tr>\n");
        contenido.append("<tr><td>part_type</td><td>"+std::string(1,particion.part_type)+"</td></tr>\n");
        contenido.append("<tr><td>part_fit</td><td>"+std::string(1,particion.part_fit)+"</td></tr>\n");
        contenido.append("<tr><td>part_start</td><td>"+to_string(particion.part_start)+"</td></tr>\n");
        contenido.append("<tr><td>part_size</td><td>"+to_string(particion.part_size)+"</td></tr>\n");
    }
    contenido.append("</TABLE>>]\n}");
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