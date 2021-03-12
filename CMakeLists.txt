cmake_minimum_required(VERSION 3.17)
project(Proyecto)

set(CMAKE_CXX_STANDARD 14)

add_executable(Proyecto main.cpp
        Analizer/parser.cpp
        Analizer/scanner.cpp
        Flow/rooter.cpp
        Modules/diskModule.cpp
        Graphics/graficador.h Graphics/graficador.cpp Estructs/partitionMount.cpp Estructs/partitionMount.h montadas.cpp montadas.h)