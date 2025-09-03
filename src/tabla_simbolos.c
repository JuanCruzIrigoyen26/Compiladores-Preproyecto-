#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabla_simbolos.h"

// Puntero global a la tabla de símbolos
static Simbolo *tabla = NULL;

// Inicializa la tabla de símbolos vacía
void inicializarTS() {
    tabla = NULL;
}

// Inserta un símbolo en la tabla
Simbolo* insertarSimbolo(AstValor *valor) {
    // Primero, verificar si ya existe
    if (buscarSimbolo(valor->s)) {
        fprintf(stderr, "Error: identificador '%s' ya declarado\n", valor->s);
        return NULL;
    }

    // Crear un nuevo símbolo
    Simbolo *nuevo = (Simbolo*)malloc(sizeof(Simbolo));
    if (!nuevo) {
        fprintf(stderr, "Error: sin memoria para símbolo\n");
        exit(1);
    }

    // Marcar como "no inicializada" al principio
    valor->flag = 0;

    nuevo->v = valor;
    nuevo->sig = tabla; // Insertar al inicio de la lista
    tabla = nuevo;

    return nuevo;
}

// Busca un símbolo por nombre
Simbolo* buscarSimbolo(char *nombre) {
    Simbolo *simbolo = tabla;
    while (simbolo) {
        if (strcmp(simbolo->v->s, nombre) == 0)
            return simbolo;
        simbolo = simbolo->sig;
    }
    return NULL; // No encontrado
}

void imprimir_tabla() {
    Simbolo *simbolo = tabla;  // tabla es el puntero global a la lista

    while (simbolo) {
        if (simbolo->v->tipoDef == INT)
        {
            printf("%s = %ld\n", simbolo->v->s, simbolo->v->i);
        }else if (simbolo->v->tipoDef == BOOL)
        {
            printf("%s = %d\n", simbolo->v->s, simbolo->v->b);
        }
        simbolo = simbolo->sig;
    }
}