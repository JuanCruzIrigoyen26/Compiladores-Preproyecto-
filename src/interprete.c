#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "tabla_simbolos.h"
#include "interprete.h"

// Evalúa expresiones y asignaciones
static int evaluar(Nodo* nodo) {
    if (!nodo) return 0;

    switch(nodo->tipo) {
        case AST_INT: return nodo->v.i;
        case AST_BOOL: return nodo->v.b;

        case AST_ID: {
            Simbolo* s = buscarSimbolo(nodo->v.s);
            if (!s) {
                fprintf(stderr, "Error: variable '%s' no declarada\n", nodo->v.s);
                exit(1);
            }
            return s->valor;
        }

        case AST_OP: {
            int izquierda = 0, derecha = 0;
            if (nodo->hi) izquierda = evaluar(nodo->hi);
            if (nodo->hd) derecha = evaluar(nodo->hd);

            switch(nodo->v.op) {
                case '+': return izquierda + derecha;
                case '*': return izquierda * derecha;
                case '=': {
                    if (nodo->hi->tipo != AST_ID) {
                        fprintf(stderr, "Error: lado izquierdo de asignación no es variable\n");
                        exit(1);
                    }
                    Simbolo* s = buscarSimbolo(nodo->hi->v.s);
                    if (!s) {
                        fprintf(stderr, "Error: variable '%s' no declarada\n", nodo->hi->v.s);
                        exit(1);
                    }
                    s->valor = evaluar(nodo->hd);
                    return s->valor;
                }
                case 'R': return derecha; // return
                default:
                    fprintf(stderr, "Error: operación desconocida '%c'\n", nodo->v.op);
                    exit(1);
            }
        }

        case AST_NONTERM:
            if (nodo->hi) evaluar(nodo->hi);
            if (nodo->hd) evaluar(nodo->hd);
            return 0;

        default:
            fprintf(stderr, "Error: nodo desconocido\n");
            exit(1);
    }
}

// Procesa declaraciones de variables
static void procesar_declaraciones(Nodo* nodo) {
    if (!nodo) return;

    if (nodo->tipo == AST_ID) {
        if (!insertarSimbolo(nodo->v.s, "int", AST_ID)) {
            fprintf(stderr, "Error: variable '%s' redeclarada\n", nodo->v.s);
            exit(1);
        }
    } else if (nodo->tipo == AST_NONTERM) {
        if (nodo->hi) procesar_declaraciones(nodo->hi);
        if (nodo->hd) procesar_declaraciones(nodo->hd);
    }
}

// Función pública para ejecutar el AST
void ejecutar(Nodo* raiz) {
    if (!raiz) return;

    procesar_declaraciones(raiz);
    evaluar(raiz);
}
