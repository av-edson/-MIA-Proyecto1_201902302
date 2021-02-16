%{

  #include <stdio.h>
  #include <stdlib.h>
  extern int yylex(void);
  extern char *yytext;
  extern FILE *yyin;
  void yyerror(char *s);
%}

%union

{
  float real;
}

%start Exp_l

%token <real> NUMERO

%token MAS
%token MENOS
%token IGUAL
%token PTOCOMA
%token POR
%token DIV
%token PARENTABIER
%token PARENTCERR


%type <real> Exp
%type <real> Calc
%type <real> Exp_l

%left MAS MENOS   /* AQUI COLOCAMOS PARA EL MAS Y MENOS */
%left POR DIV	  /* AQUI COLOCAMOS PARA LA MULTIPLICACION Y DIVISION */

%%
 

Exp_l:      Exp_l Calc  
                    |Calc
                     ;
Calc  :  Exp PTOCOMA {printf ("%4.1f\n",$1);}                              
                                 ;

Exp :       NUMERO {$$=$1;}
                    |Exp MAS Exp {$$=$1+$3;}
                    |Exp MENOS Exp {$$=$1-$3;}
                    |Exp POR Exp {$$=$1*$3;}
                    |Exp DIV Exp {$$=$1/$3;}
                    |PARENTABIER Exp PARENTCERR {$$=$2;}
                                               ;
%%

void yyerror(char *s)
{
  printf("Error sintactico %s",s);
}

int main(int argc,char **argv)
{
  if (argc>1)
                yyin=fopen(argv[1],"rt");
  else
                yyin=stdin;

  yyparse();
  return 0;
}