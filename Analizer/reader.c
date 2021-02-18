#include <string.h>
#include <stdio.h>
#include "sin.tab.h"

extern int yyparse();

int  main(){
    while (1==1)
    {
       if (yyparse() == 0)
       {
        printf(" corrio o no \n");
       }
       else{
           printf(" regreso un error xd \n");
       }
       
    }
    
    return 0;
}
