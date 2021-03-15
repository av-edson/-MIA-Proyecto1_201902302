%{
#include <stdio.h>
#include <iostream>
#include <array>
#include "../Estructs/arguments.cpp"

struct argumentos data;
using namespace std;

std::array<std::string, 12> getDatos();
void cleanEs();
int Pause();
int yylex();
int yyerror(const char* msg){
        cout<< "Syntax Error" << msg << endl;
        return 0;
}
%}

%define parse.error verbose
%token <number> show
%token <number> pausado
%token <text> comentario



%token <text> mkdisk
%token <text> rmdisk
%token <text> fdisk
%token <text> mount
%token <text> unmount
%token <text> mkfs
%token <entrace> size
%token <entrace> fit
%token <entrace> units
%token <text> path
%token <entrace> igual
%token <other> type
%token <other> name
%token <other> add
%token <other> id
%token <other> dele
%token <other> readfile
%token <other> rep
%token <text> r_mbr
%token <text> r_disk
%token <other> fs
%token <text> sb
%token <other> p_pass
%token <other> p_ussr
%token <other> login
%token <other> logout
%token <other> mkusr
%token <other> mkgrp
%token <other> p_grp
%token <text> r_file
%token <other> p_ruta

/* entradas */
%token <number> number
%token <text> e_path
%token <text> e_pdf_path
%token <entrace> e_fit
%token <entrace> e_units
%token <entrace> e_type
%token <entrace> e_delete
%token <text> e_name
%token <entrace> e_id
%token <text> fs_option
%token <text> bm_inode
%token <text> bm_block

%start ACTION



%union{
    int number;
    char text[200];
    char entrace[12];
    int other;
}

%%

ACTION: show{cleanStruct(&data, 12); addSize(3, &data, 12);}
          | pausado {Pause();}
          | comentario {printf(" com: %s\n", $1);}
          | READFILE
          | MKDISK_F
          | RMDISK
          | FDISK_F
          | MOUNT_F
          | UNMOUNT_F
          | MKFS
          | REPORTS
          | LOGIN_F
          | LOGOUT_F
          | MKUSR_F
          | MKGRP_F
        ;

MKDISK_F: mkdisk MKPARAMS;

MKPARAMS: MKPARAM MKPARAMS
          | MKPARAM
    ;

MKPARAM: size igual number  { 
                            cleanStruct(&data, 1); addSize($3, &data, 1);}
        | path igual e_path {cleanStruct(&data, 1); addPath($3, &data, 1);}
        | fit igual e_fit   {cleanStruct(&data, 1); addFit($3, &data, 1);}
        | units igual e_units {cleanStruct(&data, 1); addUnits($3, &data, 1);
        };

RMDISK: rmdisk path igual e_path {
                          cleanStruct(&data, 2); addPath($4, &data, 2);
};

FDISK_F: fdisk FDPARAMS;
FDPARAMS: FDPARAM FDPARAMS
          | FDPARAM
  ;

FDPARAM: size igual number  {cleanStruct(&data, 3); addSize($3, &data, 3);}
        | units igual e_units {cleanStruct(&data, 3); addUnits($3, &data, 3);}
        | path igual e_path {cleanStruct(&data, 3); addPath($3, &data, 3);}
        | type igual e_type {cleanStruct(&data, 3); addType($3, &data, 3);}
        | fit igual e_fit   {cleanStruct(&data, 3); addFit($3, &data, 3);}
        | dele igual e_delete  {cleanStruct(&data, 3); addDelete($3, &data, 3);}
        | name igual e_name  {cleanStruct(&data, 3); addNameDisk($3, &data, 3);}
        | add igual number  {cleanStruct(&data, 3); addSizeAdd($3, &data, 3);
        }
;


MOUNT_F: mount path igual e_path name igual e_name {
            cleanStruct(&data, 4); addPath($4, &data, 4); addNameDisk($7, &data, 4);}
          | mount name igual e_name path igual e_path{
            cleanStruct(&data, 4); addPath($7, &data, 4); addNameDisk($4, &data, 4); };

UNMOUNT_F: unmount id igual e_id { cleanStruct(&data, 5); addDiskIde($4, &data, 5);};

READFILE: readfile path igual e_path{ cleanStruct(&data, 7); addPath($4, &data, 7);};

REPORTS: rep REPORTSPARAMS;
REPORTSPARAMS: REPORTSPARAM REPORTSPARAMS
		| REPORTSPARAM;
REPORTSPARAM: name igual r_mbr {cleanStruct(&data, 66); addNameDisk($3, &data, 66);}
		| name igual r_disk {cleanStruct(&data, 66); addNameDisk($3, &data, 66);}
		| name igual sb {cleanStruct(&data, 66); addNameDisk($3, &data, 66);}
		| name igual bm_block {cleanStruct(&data, 66); addNameDisk($3, &data, 66);}
		| name igual bm_inode {cleanStruct(&data, 66); addNameDisk($3, &data, 66);}
		| name igual r_file {cleanStruct(&data, 66); addNameDisk($3, &data, 66);}
		| p_ruta igual e_pdf_path {cleanStruct(&data, 66); addRuta($3, &data, 66);}
		| path igual e_pdf_path {cleanStruct(&data, 66); addPath($3, &data, 66);}
		| id igual e_id {cleanStruct(&data, 66); addDiskIde($3, &data, 66);};

MKFS: mkfs MKFS_PARAMS;
MKFS_PARAMS: MKFS_PARAM MKFS_PARAMS
		|MKFS_PARAM;
MKFS_PARAM: id igual e_id{cleanStruct(&data, 6); addDiskIde($3, &data, 6);}
		| type igual e_delete{cleanStruct(&data, 6); addType($3, &data,6);}
		| fs igual fs_option{cleanStruct(&data, 6); addFormat($3, &data,6);};

/* ------------------------------------  		USUARIOS Y GRUPOS		--------------------*/
LOGIN_F: login LOGINPARAMS;
LOGINPARAMS: LOGINPARAM LOGINPARAMS
		| LOGINPARAM;
LOGINPARAM : p_ussr igual e_name{cleanStruct(&data, 8); addNameDisk($3, &data, 8);}
		| p_pass igual e_name{cleanStruct(&data, 8); addPath($3, &data, 8);}
		| p_pass igual number{cleanStruct(&data, 8); addSizeAdd($3, &data, 8);}
		| id igual e_id{cleanStruct(&data, 8); addDiskIde($3, &data,8);};
LOGOUT_F: logout{cleanStruct(&data, 9);addSizeAdd(1, &data, 9);};

MKUSR_F: mkusr MKUSR_PARAMS;
MKUSR_PARAMS: MKUSR_PARAM MKUSR_PARAMS
		|MKUSR_PARAM;
MKUSR_PARAM: p_ussr igual e_name{cleanStruct(&data, 10); addNameDisk($3, &data, 10);}
             	| p_pass igual e_name{cleanStruct(&data, 10); addPath($3, &data, 10);}
             	| p_pass igual number{cleanStruct(&data, 10); addSizeAdd($3, &data, 10);}
             	| p_grp igual e_name{cleanStruct(&data, 10); addDiskIde($3, &data, 10);};

MKGRP_F: mkgrp name igual e_name{cleanStruct(&data, 11); addNameDisk($4, &data, 11);};
%%
std::array<std::string, 12> getDatos(){
    std::array<std::string, 12> datos;
    datos = privateData(&data);
    return datos;
}

void cleanEs(){
        cleanStruct(&data, -1);
}

int Pause(){
    std::cout << "Precione enter para continuar  ";
    std::cin.get();
    return 0;
}