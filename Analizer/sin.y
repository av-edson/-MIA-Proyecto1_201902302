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
/* entradas */
%token <number> number
%token <text> e_path
%token <entrace> e_fit
%token <entrace> e_units

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

ACTION: MKDISK_F
          | RMDISK
          | show {showArguments(&data);}
        ;

MKDISK_F: mkdisk MKPARAMS
            ;

MKPARAMS: MKPARAM MKPARAMS
          | MKPARAM
    ;

MKPARAM: size igual number  { 
                            cleanStruct(&data, 1); addSize($3, &data, 1);}
        | path igual e_path {cleanStruct(&data, 1); addPath($3, &data, 1);}
        | fit igual e_fit   {cleanStruct(&data, 1); addFit($3, &data, 1);}
        | units igual e_units {cleanStruct(&data, 1); addUnits($3, &data, 1);
        }
    ;

RMDISK: rmdisk path igual e_path {
                          cleanStruct(&data, 2); addPath($4, &data, 1);
};


%%
