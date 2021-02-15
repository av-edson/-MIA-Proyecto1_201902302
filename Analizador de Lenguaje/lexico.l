%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
%}

/*----------------------------------------------------- SECCION DE DEFINICIONES----------------------------------- */
%option c++
%option outfile="Scanner.cpp"
%option case-insensitive
%option noyywrap

/* NUMEROS  */
DIGITO [0-9]
ENTERO {DIGITO}+
REAL {DIGITO}+"."{DIGITO}+
SIGNO_MENOS     "-"
NEGATIVO    {SIGNO_MENOS}{REAL}

/* ADMINISTRADOR DE DISCOS*/
F_MKDISK    "mkdisk"
F_RMDISK    "rmdisk"
F_FDISK     "fdisk"
F_MOUNT     "mount"
F_UNMOUNT   "unmount"
F_MKFS      "mkfs"
/* GESTOR DE USUARIOS Y GRUPOS*/
F_LOGIN     "login"
F_LOGOUT    "logout"
F_MKGRP     "mkgrp"
F_RMGRP     "rmgrp"
F_MKURS     "mkurs"
F_RMUSR     "rmusr"
/*ADMIND DE PERSIMOS Y ARCHIVOS*/
F_CHMOD     "chmod"
F_MKFILE    "mkfile"
F_CAT       "cat"
F_REM       "rem"
F_EDIT      "edit"
F_REN       "ren"
F_MKDIR     "mkdir"
F_CP        "cp"
F_MV        "mv"
F_FIND      "find"
F_CHOWN     "chown"
F_CHGRP     "chgrp"
F_PAUSE     "pause"
/*  PARAMETROS  */
P_SIZE      "-size"
P_FIT       "-f"
P_UNITS     "-u"
P_PATH      "-path"
P_TYPE      "-type"
P_DELETE    "-delete"
P_NAME      "-name"
P_ADD       "-add"
P_ID        ""


DELIMITADOR [\t\r\f" "]

OTRO    [a-z]+
/* IDES */
IDES    {ENTERO}[a-z]

/*----------------------------------------------------- SECCION DE REGLAS -------------------------------------*/
%%

{ENTERO} {printf("Numero Entero %s\n", yytext);}
{REAL} {printf("Numero Real %s\n", yytext);}
{NEGATIVO} {printf("Numero Negativo %s\n", yytext);}


{F_MKDISK} {printf("Funcion mkdisk %s\n", yytext);}
{F_RMDISK} {printf("Funcion rmdisk %s\n", yytext);}
{F_FDISK} {printf("Funcion fdisk %s\n", yytext);}
{F_MOUNT}   {printf("Funcion mount %s\n", yytext);}   
{F_UNMOUNT} {printf("Funcion unmount %s\n", yytext);}
{F_MKFS}    {printf("Funcion mkfs %s\n", yytext);}

{F_LOGIN}   {printf("Funcion login %s\n", yytext);}
{F_LOGOUT}  {printf("Funcion logout %s\n", yytext);}
{F_MKGRP}   {printf("Funcion mkgrp %s\n", yytext);}
{F_RMGRP}   {printf("Funcion rmgrp %s\n", yytext);}
{F_MKURS}   {printf("Funcion mkurs %s\n", yytext);}
{F_RMUSR}   {printf("Funcion rmusr %s\n", yytext);}

{F_CHMOD}   {printf("Funcion chmod %s\n", yytext);}
{F_MKFILE}  {printf("Funcion mkfile %s\n", yytext);}
{F_CAT}     {printf("Funcion cat %s\n", yytext);}
{F_REM}     {printf("Funcion rem %s\n", yytext);}
{F_EDIT}    {printf("Funcion edit %s\n", yytext);}
{F_REN}     {printf("Funcion ren %s\n", yytext);}
{F_MKDIR}   {printf("Funcion mkdir %s\n", yytext);}
{F_CP}      {printf("Funcion cp %s\n", yytext);}
{F_MV}      {printf("Funcion mv %s\n", yytext);}
{F_FIND}    {printf("Funcion find %s\n", yytext);}
{F_CHOWN}   {printf("Funcion chown %s\n", yytext);}
{F_CHGRP}   {printf("Funcion chgrp %s\n", yytext);}
{F_PAUSE}   {printf("Funcion pause %s\n", yytext);}

{DELIMITADOR}           {}
{IDES}      {printf("Un IDE %s\n", yytext);}
{OTRO}      {printf("ERROR PAI %s\n", yytext);}
.               {printf("Caracter Invalido %s\n", yytext);}


%%