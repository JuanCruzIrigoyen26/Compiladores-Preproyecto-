#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "tabla_simbolos.h"
#include "interprete.h"

// Evalúa expresiones y asignaciones
static int evaluar(Nodo* nodo) {
    if (!nodo) return 0;

    switch(nodo->tipo) {
        case AST_INT: return nodo->v->i;

        case AST_BOOL: return nodo->v->b;

        case AST_ID: {
            Simbolo* s = buscarSimbolo(nodo->v->s);
            if (!s) {
                fprintf(stderr, "Error: variable '%s' no declarada\n", nodo->v->s);
                exit(1);
            }
            return (s->v->tipoDef == INT) ? s->v->i : s->v->b;
        }

        case AST_ASIGNACION: {
            Nodo* nodoId = nodo->hi;
            Simbolo* s = buscarSimbolo(nodoId->v->s);
            if (!s) {
                fprintf(stderr, "Error: variable '%s' no declarada\n", nodoId->v->s);
                exit(1);
            }
            int valor = evaluar(nodo->hd);
            if (s->v->tipoDef == INT)
                s->v->i = valor;
            else
                s->v->b = valor;
            s->v->flag = 1; // opcional: marca como inicializada
            return valor;
        }

        case AST_OP: {
            int izquierda = evaluar(nodo->hi);
            int derecha = evaluar(nodo->hd);
            if (nodo->v->op == '+') return izquierda + derecha;
            if (nodo->v->op == '*') return izquierda * derecha;
            return 0;
        }

        case AST_RETURN: {
            int valor = nodo->hi ? evaluar(nodo->hi) : 0;
            printf("RETURN: %d\n", valor);
            return valor;
        }

       case AST_SEQ:
        case AST_STMTS:
        case AST_DECLS:
            if (nodo->hi) evaluar(nodo->hi);
            if (nodo->hd) evaluar(nodo->hd);
            return 0;

        case AST_FUNCION:
            return evaluar(nodo->hd); // evaluamos el bloque de la función

        case AST_DEC_TIPO:
            return 0; // no hace nada en la ejecución

        default:
            fprintf(stderr, "Error: nodo desconocido en evaluar (tipo=%d)\n", nodo->tipo);
            exit(1);
    }    
       
}


// Procesa declaraciones de variables
static void procesar_declaraciones(Nodo* nodo) {
    if (!nodo) return;

    if (nodo->tipo == AST_ID) {
        // Inserta solo si no existe
        if (!buscarSimbolo(nodo->v->s)) {
            insertarSimbolo(nodo->v);
        }
    } else {
        if (nodo->hi) procesar_declaraciones(nodo->hi);
        if (nodo->hd) procesar_declaraciones(nodo->hd);
    }
}

// Función pública para ejecutar el AST
void ejecutar(Nodo* raiz) {
    if (!raiz) return;
    inicializarTS();           // iniciar tabla vacía
    procesar_declaraciones(raiz); // agregar variables declaradas
    evaluar(raiz);             // ejecutar/interpretar programa
}
