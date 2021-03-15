//
// Created by edson on 13/03/21.
//

#ifndef PROYECTO_USSERMODULE_H
#define PROYECTO_USSERMODULE_H
#include <iostream>
#include <cstring>
#include "../Estructs/diskStructs.h"
#include "../Estructs/fileSistemStructs.h"
#include "string"
#include "../montadas.h"

using namespace std;

bool loginUsser(string usser, string password, string idPart, list<montadas> *listaMontadas, std::string *usuarioActual, string *idParticionActiba);

bool logoutUsser(string *usuarioActual, string *idParticionActiba);

bool mkGroupUsser(string nombreGrupo, list<montadas> *listaMontadas, std::string *usuarioActual, string *idParticionActiba);
bool mkUsserVoid(string nombreUsuario, string grupoUsuario, string passUsuario, list<montadas> *listaMontadas, std::string *usuarioActual, string *idParticionActiba);

#endif //PROYECTO_USSERMODULE_H
