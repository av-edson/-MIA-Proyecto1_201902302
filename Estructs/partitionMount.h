//
// Created by edson on 11/03/21.
//

#ifndef PROYECTO_PARTITIONMOUNT_H
#define PROYECTO_PARTITIONMOUNT_H
#include "string"
#include "partitionMount.h"
#include "diskStructs.h"
using namespace std;

class partitionMount {
    MBR mbrDisco{};
    string idDisco;
    // faltan las clases de inodos, bloques etc
public:
    void setMbr(std::string path);
    MBR getMbt();
    void setIdDisco(char letra, int numero);
};


#endif //PROYECTO_PARTITIONMOUNT_H
