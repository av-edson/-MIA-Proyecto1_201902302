%{
#include <stdio.h>
#include "../Estructs/arguments.cpp"

struct argumentos data;

int yylex();
int yyerror(char* mens){
        printf(" Syntax Error %s", mens);
        return 0;
    }
%}
%token <number> show
%type <text>MKDISK_F
%type <text> MKPARAM



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
%token <other> type;
%token <other> delete;
%token <other> name;
%token <other> add;
%token <other> id;
/* entradas */
%token <number> number
%token <text> e_path
%token <entrace> e_fit
%token <entrace> e_units
%token <entrace> e_type
%token <entrace> e_delete
%token <text> e_name
%token <entrace> e_id;

%type ACTION

%start CONTENT



%union{
    int number;
    char text[200];
    char entrace[12];
    int other;
}

%%

CONTENT: ACTION CONTENT
        | ACTION;

ACTION: show{showArguments(&data);}
          | MKDISK_F
          | RMDISK
          | FDISK_F
          | MOUNT_F
          | UNMOUNT_F
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

FDPARAM: size igual number  {
          cleanStruct(&data, 3); addSize($3, &data, 3);}
        | units igual e_units {cleanStruct(&data, 3); addUnits($3, &data, 3);}
        | path igual e_path {cleanStruct(&data, 3); addPath($3, &data, 3);}
        | type igual e_type {cleanStruct(&data, 3); addType($3, &data, 3);}
        | fit igual e_fit   {cleanStruct(&data, 3); addFit($3, &data, 3);}
        | delete igual e_delete  {cleanStruct(&data, 3); addDelete($3, &data, 3);}
        | name igual e_name  {cleanStruct(&data, 3); addNameDisk($3, &data, 3);}
        | add igual number  {cleanStruct(&data, 3); addSizeAdd($3, &data, 3);
        }
;


MOUNT_F: mount path igual e_path name igual e_name {
            cleanStruct(&data, 4); addPath($4, &data, 4); addNameDisk($7, &data, 4);}
          | mount name igual e_name path igual e_path{
            cleanStruct(&data, 4); addPath($7, &data, 4); addNameDisk($4, &data, 4); }
        ;

UNMOUNT_F: unmount id igual e_id {
                cleanStruct(&data, 5); addDiskIde($4, &data, 5);
      };
%%
