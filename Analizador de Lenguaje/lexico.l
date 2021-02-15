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
NEGATIVO    {SIGNO_MENOS}({REAL}|{ENTERO})
LETRAS  [a-z]+

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
P_ID        "-id"
P_FORMAT    "-fs"

/* ENTRADAS */
E_BF        "bf"
E_FF        "ff"
E_WF        "wf"
E_KILOBY    "k"
E_MEGBY     "m"
E_BYTE      "b"
E_P         "p"
E_E         "e"
E_L         "l"
E_FAST      "fast"
E_FULL      "full"
E_PATH1      {SLASH}{SEMIROUTE}+{NAME_FILE}
E_PATH2     \"({LETRAS}|{ENTERO}|{SLASH}|" ")+{NAME_FILE}\"
E_NAME1      {LETRAS}({LETRAS}|{ENTERO})*
E_NAME2     \"{LETRAS}({LETRAS}|{ENTERO}|" ")*\"
E_IDES    {ENTERO}[a-z]{1,2}

/* OTROS */
IGUAL       "="
SLASH       "/"
SEMIROUTE   {LETRAS}{SLASH}

DISK_EXTENTION1  "dk"
DISK_EXTENTION2  "disk"
NAME_FILE   {LETRAS}({LETRAS}|{ENTERO})+"."({DISK_EXTENTION2}|{DISK_EXTENTION1})
DELIMITADOR [\t\r\f" "]
COMENTARIO  #([^#|^\n|^\r\n])*(#)?

/* IDES  para discos*/



/*----------------------------------------------------- SECCION DE REGLAS -------------------------------------*/
%%

{ENTERO} {printf("Numero Entero %s ", yytext);}
{REAL} {printf("Numero Real %s ", yytext);}
{NEGATIVO} {printf("Numero Negativo %s ", yytext);}


{F_MKDISK} {printf("Funcion mkdisk %s", yytext);}
{F_RMDISK} {printf("Funcion rmdisk %s", yytext);}
{F_FDISK} {printf("Funcion fdisk %s", yytext);}
{F_MOUNT}   {printf("Funcion mount %s", yytext);}   
{F_UNMOUNT} {printf("Funcion unmount %s", yytext);}
{F_MKFS}    {printf("Funcion mkfs %s", yytext);}

{F_LOGIN}   {printf("Funcion login %s ", yytext);}
{F_LOGOUT}  {printf("Funcion logout %s ", yytext);}
{F_MKGRP}   {printf("Funcion mkgrp %s ", yytext);}
{F_RMGRP}   {printf("Funcion rmgrp %s ", yytext);}
{F_MKURS}   {printf("Funcion mkurs %s ", yytext);}
{F_RMUSR}   {printf("Funcion rmusr %s ", yytext);}

{F_CHMOD}   {printf("Funcion chmod %s ", yytext);}
{F_MKFILE}  {printf("Funcion mkfile %s ", yytext);}
{F_CAT}     {printf("Funcion cat %s ", yytext);}
{F_REM}     {printf("Funcion rem %s ", yytext);}
{F_EDIT}    {printf("Funcion edit %s ", yytext);}
{F_REN}     {printf("Funcion ren %s ", yytext);}
{F_MKDIR}   {printf("Funcion mkdir %s ", yytext);}
{F_CP}      {printf("Funcion cp %s ", yytext);}
{F_MV}      {printf("Funcion mv %s ", yytext);}
{F_FIND}    {printf("Funcion find %s", yytext);}
{F_CHOWN}   {printf("Funcion chown %s", yytext);}
{F_CHGRP}   {printf("Funcion chgrp %s", yytext);}
{F_PAUSE}   {printf("Funcion pause %s", yytext);}

{P_SIZE}    {printf("Parametro size %s ", yytext);}
{P_FIT}    {printf("Parametro fit %s ", yytext);}
{P_UNITS}    {printf("Parametro units %s ", yytext);}
{P_PATH}    {printf("Parametro path %s ", yytext);}
{P_TYPE}    {printf("Parametro type %s ", yytext);}
{P_DELETE}    {printf("Parametro delete %s ", yytext);}
{P_NAME}    {printf("Parametro name %s ", yytext);}
{P_ADD}    {printf("Parametro add %s ", yytext);}
{P_ID}    {printf("Parametro id %s ", yytext);}
{P_FORMAT}    {printf("Parametro format %s ", yytext);}


{E_PATH1}    {printf("ruta normal %s ", yytext);}
{E_PATH2}    {printf("ruta encerrada %s ", yytext);}
{E_BF}      {printf("Entrada bf %s ", yytext);}
{E_FF}      {printf("Entrada ff %s ", yytext);}
{E_WF}      {printf("Entrada  wf%s ", yytext);}   
{E_KILOBY}  {printf("Entrada kb %s ", yytext);}   
{E_MEGBY}   {printf("Entrada mb %s ", yytext);}
{E_E}       {printf("Entrada  E %s ", yytext);}
{E_P}       {printf("Entrada  P %s ", yytext);}
{E_L}       {printf("Entrada  L %s ", yytext);}
{E_FAST}       {printf("Entrada  fast %s ", yytext);}
{E_FULL}       {printf("Entrada  full %s ", yytext);}
{E_BYTE}    {printf("Entrada  by%s ", yytext);}
{E_NAME1}    {printf("Entrada  nombre1 %s ", yytext);}
{E_NAME2}    {printf("Entrada  nombre2 %s ", yytext);}
{E_IDES}      {printf("Un IDE %s\n", yytext);}



{DELIMITADOR}           {}
{COMENTARIO}    {printf("Comentario %s\n", yytext);}
{IGUAL}     {printf("IGUAL %s", yytext);}
.               {printf("Caracter %s\n", yytext);}


%%