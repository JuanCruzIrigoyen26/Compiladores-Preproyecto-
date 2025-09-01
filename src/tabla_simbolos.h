#ifndef TABLA_SIMBOLOS_H
#define TABLA_SIMBOLOS_H

#include "ast.h"

typedef struct Simbolo {
    AstValor *v;
    struct Simbolo *sig;
} Simbolo;

// Operaciones sobre la tabla de símbolos
void inicializarTS();
Simbolo* insertarSimbolo(AstValor *valor);
Simbolo* buscarSimbolo(char *nombre);
void imprimir_tabla();

#endif
