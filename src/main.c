#include <stdio.h>
#include "ast.h"
#include "calcSintaxis.tab.h"

extern Nodo* raiz;
extern FILE *yyin;  

int main(int argc, char *argv[]) {
    if (argc > 1)
        yyin = fopen(argv[1],"r");
    else
        yyin = stdin;

    if (yyparse() == 0) {
        printf("\nÁrbol de sintaxis abstracta (AST):\n");
        imprimir_ast(raiz, 0);
    }

    if (yyin != stdin){
        fclose(yyin);
    }

    return 0;
}
