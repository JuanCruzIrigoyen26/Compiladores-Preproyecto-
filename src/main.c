#include <stdio.h>
#include "ast.h"
#include "calcSintaxis.tab.h"
#include "interprete.h"     
#include "tabla_simbolos.h" 

extern Nodo* raiz;
extern FILE *yyin;  

int main(int argc, char *argv[]) {
    if (argc > 1)
        yyin = fopen(argv[1],"r");
    else
        yyin = stdin;

    inicializarTS(); // inicializa la tabla de símbolos

    if (yyparse() == 0) {
        printf("\nÁrbol de sintaxis abstracta (AST):\n");
        imprimir_ast(raiz, 0);

        printf("\nEjecutando programa...\n");
        ejecutar(raiz);  // ejecuta el intérprete
        imprimir_tabla(); // Mostramos los valores de las variables
    }

    if (yyin != stdin){
        fclose(yyin);
    }

    return 0;
}
