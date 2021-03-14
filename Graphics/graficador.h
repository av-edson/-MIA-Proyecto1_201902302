//
// Created by edson on 10/03/21.
//

#ifndef PROYECTO_GRAFICADOR_H
#define PROYECTO_GRAFICADOR_H

#include <iostream>
#include <string>
#include <array>

void graficarMBR(std::string path, std::string idDisk);
void graficarDisk(std::string path, std::string id);
void graficarSB(std::string path, std::string nombre);
void graficarBmInode(std::string path, std::string nombre);
void graficarBmBlock(std::string path, std::string nombre);

#endif //PROYECTO_GRAFICADOR_H
