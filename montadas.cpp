//
// Created by edson on 11/03/21.
//

#include "montadas.h"
char getLetraMontadas(char letraAnterior);
string makeId(char letra, int numero);

void montadas::savePartition(string path, list<montadas> *listaMontadas, string _nombre) {
    char letra='0'; string carnet="02"; int numero=0;
    if (listaMontadas->empty()){
        montadas nuevaMontada;
        nuevaMontada.letra = 'a';
        nuevaMontada.carnet = 32;
        nuevaMontada.numero = 1;
        nuevaMontada.path = path;
        nuevaMontada.nombre = _nombre;
        nuevaMontada.id = makeId(nuevaMontada.letra, nuevaMontada.numero);
        listaMontadas->push_back(nuevaMontada);
        return;
    }

    bool mismoDisco = false;
    for(montadas mt: *listaMontadas){
        if (path == mt.path) {
            letra = mt.letra;
            numero = mt.numero;
            mismoDisco = true;
        }
        if (mt.numero > numero) numero = mt.numero;
    }
    if (!mismoDisco){
        letra='0';
        numero++;
    }
    montadas nuevaMontada;
    nuevaMontada.letra = getLetraMontadas(letra);
    nuevaMontada.carnet = 32;
    nuevaMontada.numero = numero;
    nuevaMontada.path = path;
    nuevaMontada.nombre = _nombre;
    nuevaMontada.id = makeId(nuevaMontada.letra, nuevaMontada.numero);
    listaMontadas->push_back(nuevaMontada);
}

string montadas::getName() {
    return this->nombre;
}

string montadas::getId() {
    return std::string();
}

char getLetraMontadas(char letraAnterior){
    switch (letraAnterior) {
        case '0':
            return 'a';
        case 'a':
            return 'b';
        case 'b':
            return 'c';
        case 'c':
            return 'd';
        case 'd':
            return 'e';
        case 'e':
            return 'f';
        case 'f':
            return 'g';
        case 'g':
            return 'h';
        case 'h':
            return 'i';
        default:
            return 'z';
    }
}

string makeId(char letra, int numero){
    string nm = to_string(numero);
    string le(1, letra);
    string res = "32";
    res.append(nm); res.append(le);
    return res;
}
