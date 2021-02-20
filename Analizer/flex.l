%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "parser.h"

using namespace std;

%}

/*----------------------------------------------------- SECCION DE DEFINICIONES----------------------------------- */
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
E_PATH2     \"{SLASH}({LETRAS}|{ENTERO}|{SLASH}|" ")+{NAME_FILE}\"
E_NAME1      {LETRAS}({LETRAS}|{ENTERO})*
E_NAME2     \"{LETRAS}({LETRAS}|{ENTERO}|" ")*\"
E_IDES    {ENTERO}[a-z]{1,2}

/* OTROS */
IGUAL       "="
SLASH       "/"
SEMIROUTE   {LETRAS}({LETRAS}|{DIGITO})*{SLASH}

DISK_EXTENTION1  "dk"
DISK_EXTENTION2  "disk"
SCRIPT_EXTENTION    "script"
NAME_FILE   {LETRAS}({LETRAS}|{ENTERO})*"."({DISK_EXTENTION2}|{DISK_EXTENTION1}|{SCRIPT_EXTENTION})
DELIMITADOR [\t\r\f" "]
COMENTARIO  #([^#|^\n|^\r\n])*(#)?

/* IDES  para discos*/



/*----------------------------------------------------- SECCION DE REGLAS -------------------------------------*/
%%

{ENTERO} {yylval.number = atoi(yytext); return(number);}
{REAL} {yylval.number = atoi(yytext); return(number);}
{NEGATIVO} {yylval.number = atoi(yytext); return(number);}

 "show"      {return(show);}
 "exec"     {return(readfile);}
{F_PAUSE}   {return(pausado);}
{F_MKDISK} {return(mkdisk);}
{F_RMDISK} {return(rmdisk);}
{F_FDISK} {return(fdisk);}
{F_MOUNT}   {return(mount);}   
{F_UNMOUNT} {return(unmount);}
{F_MKFS}    {return(mkfs);}

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

{P_SIZE}    {return(size);}
{P_FIT}     {return(fit);}
{P_UNITS}   {return(units);}
{P_PATH}    {return(path);}
{P_TYPE}    {return(type);}
{P_NAME}    {return(name);}
{P_DELETE}  {return(dele);}
{P_ADD}     {return(add);}
{P_ID}      {return(id);}
{P_FORMAT}  {}


{E_PATH1}    {strcpy(yylval.text, yytext); return(e_path);}
{E_PATH2}    {strcpy(yylval.text, yytext); return(e_path);}
{E_BF}      {sscanf(yytext, "%s", yylval.entrace); return(e_fit);}
{E_FF}      {sscanf(yytext, "%s", yylval.entrace); return(e_fit);}
{E_WF}      {sscanf(yytext, "%s", yylval.entrace); return(e_fit);}     
{E_KILOBY}  {sscanf(yytext, "%s", yylval.entrace); return(e_units);}   
{E_MEGBY}   {sscanf(yytext, "%s", yylval.entrace); return(e_units);}
{E_E}       {sscanf(yytext, "%s", yylval.entrace); return(e_type);}
{E_P}       {sscanf(yytext, "%s", yylval.entrace); return(e_type);}
{E_L}       {sscanf(yytext, "%s", yylval.entrace); return(e_type);}
{E_FAST}       {sscanf(yytext, "%s", yylval.entrace); return(e_delete);}
{E_FULL}       {sscanf(yytext, "%s", yylval.entrace); return(e_delete);}
{E_BYTE}    {sscanf(yytext, "%s", yylval.entrace); return(e_units);}
{E_NAME1}    {strcpy(yylval.text, yytext); return(e_name);}
{E_NAME2}    {strcpy(yylval.text, yytext); return(e_name);}
{E_IDES}      {strcpy(yylval.text, yytext); return(e_id);}



{DELIMITADOR}           {}
{COMENTARIO}    {strcpy(yylval.text, yytext); return(comentario);}
{IGUAL}     {return(igual);}
.               {printf("Caracter %s no admitido\n ", yytext);}


%%