//
// Created by edson on 11/03/21.
//

#include <string>
#include "partitionMount.h"
using namespace std;

void partitionMount::setMbr(std::string path) {
    FILE *archivo = fopen(path.c_str(), "rb+");
    fseek(archivo, 0  ,SEEK_SET);
    MBR aux{};
    fread(&aux, sizeof(aux), 1, archivo);
    this->mbrDisco = aux;
}

MBR partitionMount::getMbt() {
    return this->mbrDisco;
}

void partitionMount::setIdDisco(char letra, int numero) {
    string resultadoID="02";
    string le = to_string(letra);
    string num = to_string(numero);
    resultadoID.append(le);
    resultadoID.append(num);
    this->idDisco = resultadoID;
}
