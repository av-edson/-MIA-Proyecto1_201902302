//
// Created by edson on 11/03/21.
//

#ifndef PROYECTO_MONTADAS_H
#define PROYECTO_MONTADAS_H
#include "string"
#include "list"
using namespace std;

class montadas {
    int numero;
    string path;
    char letra;
    string id;
    string nombre;
    int carnet = 02;
public:
    void savePartition(string path, list<montadas> *listaMontadas, string _nombre);
   string getName();
   string getId();
};


#endif //PROYECTO_MONTADAS_H
