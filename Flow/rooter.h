#ifndef _ROOTER
#define _ROOTER

#include <iostream>
#include <string>
#include <array>
#include "../Modules/diskModule.h"
#include "../Graphics/graficador.h"
#include "../montadas.h"
#include "list"


void readArguments(std::array<std::string, 11> lista, list<montadas> *listaMontadas);

#endif