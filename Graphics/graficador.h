//
// Created by edson on 10/03/21.
//

#ifndef PROYECTO_GRAFICADOR_H
#define PROYECTO_GRAFICADOR_H

#include <iostream>
#include <string>
#include <array>
#include "../montadas.h"
#include "../Modules/usserModule.h"

void graficarMBR(std::string path, std::string pathResultante);
void graficarDisk(std::string path, std::string pathResultante);
void graficarSB(std::string path, std::string nombre, string pathResultante);
void graficarBmInode(std::string path, std::string nombre, string pathDisco);
void graficarBmBlock(std::string path, std::string nombre, string pathDisco);
void graficarUssersTxt(std::string path, std::string nombre, std::string fileName, list<montadas> *listaMontadas);
void listarMontadas(list<montadas> *listaMontadas);

#endif //PROYECTO_GRAFICADOR_H
