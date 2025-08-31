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
Simbolo* insertarSimbolo(const char *nombre, const char *tipo, AstTipo flag) {
    // Primero, verificar si ya existe
    if (buscarSimbolo(nombre)) {
        fprintf(stderr, "Error: identificador '%s' ya declarado\n", nombre);
        return NULL;
    }

    // Crear un nuevo símbolo
    Simbolo *nuevo = (Simbolo*)malloc(sizeof(Simbolo));
    if (!nuevo) {
        fprintf(stderr, "Error: sin memoria para símbolo\n");
        exit(1);
    }
    nuevo->nombre = strdup(nombre);
    nuevo->tipo = strdup(tipo);
    nuevo->flag = flag;
    nuevo->valor = 0;
    nuevo->sig = tabla; // Insertar al inicio de la lista
    tabla = nuevo;

    return nuevo;
}

// Busca un símbolo por nombre
Simbolo* buscarSimbolo(const char *nombre) {
    Simbolo *s = tabla;
    while (s) {
        if (strcmp(s->nombre, nombre) == 0)
            return s;
        s = s->sig;
    }
    return NULL; // No encontrado
}

void imprimir_tabla() {
    Simbolo *s = tabla;  // tabla es el puntero global a la lista
    while (s) {
        printf("%s = %d\n", s->nombre, s->valor);
        s = s->sig;
    }
}