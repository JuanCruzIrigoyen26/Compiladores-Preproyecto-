#ifndef TABLA_SIMBOLOS_H
#define TABLA_SIMBOLOS_H

#include "ast.h"

typedef struct Simbolo {
    AstTipo flag;       // Usamos el mismo enum que ya existe en ast.h
    char *nombre;       // Identificador
    char *tipo;         // Tipo de dato ("int", "bool")
    int valor;          
    struct Simbolo *sig;
} Simbolo;

// Operaciones sobre la tabla de símbolos
void inicializarTS();
Simbolo* insertarSimbolo(const char *nombre, const char *tipo, AstTipo flag);
Simbolo* buscarSimbolo(const char *nombre);
void imprimir_tabla();

#endif
