#include <stdio.h>
#include "ast.h"
#include "calcSintaxis.tab.h"
#include "interprete.h"     
#include "tabla_simbolos.h" 
#include "calcSemantico.h"

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
        if (verificar_tipos(raiz)) {
            ejecutar(raiz);  // ejecuta el intérprete
        }
        
        imprimir_tabla(); // Mostramos los valores de las variables

        printf("\nPseudo-Assembly\n");
        generar_assembly(raiz, stdout); // se genera el pseudo-assembly
    }

    if (yyin != stdin){
        fclose(yyin);
    }

    return 0;
}
