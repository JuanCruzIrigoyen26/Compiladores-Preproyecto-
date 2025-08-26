#include <stdio.h>
#include "calcSintaxis.tab.h"
#include "ast.h"

extern Nodo* raiz;
extern FILE *yyin;  

int main(int argc, char *argv[]) {
    ++argv, --argc;
    if (argc > 0)
        yyin = fopen(argv[0],"r");
    else
        yyin = stdin;

    if (yyparse() == 0) {
        printf("\nÁrbol de sintaxis abstracta (AST):\n");
        imprimir_ast(raiz, 0);
    }
    return 0;
}
