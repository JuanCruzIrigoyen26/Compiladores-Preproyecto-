#include <stdio.h>
#include <stdlib.h>
#include "calcSemantico.h"
#include "tabla_simbolos.h"
#include "ast.h"

static void chequeo_declaraciones(Nodo* declaracion);
static void chequeo_sentencias(Nodo* sentencias, tipoDef tipoFuncion);
static tipoDef chequeo_expresion(Nodo* expresion);

//Funcion que verifica tipos de variables y funcion
// con respecto a asignaciones y returns, utilizando funciones auxiliares
int verificar_tipos(Nodo* raiz) {
    if (!raiz) return 1;
    if (raiz->tipo != AST_FUNCION) return 1;

    Nodo* bloque = raiz->hd;
    if (!bloque) return 1;

    tipoDef tipoFuncion = raiz->v->tipoDef;

    chequeo_declaraciones(bloque->hi);

    chequeo_sentencias(bloque->hd, tipoFuncion);

    return 1;
}

//Funcion para insertar declaraciones en la tabla de simbolos
static void chequeo_declaraciones(Nodo* declaracion) {
    if (!declaracion) return;
    switch (declaracion->tipo) {
        case AST_SEQ:
            chequeo_declaraciones(declaracion->hi);
            chequeo_declaraciones(declaracion->hd);
            break;
        case AST_DECLS:
            chequeo_declaraciones(declaracion->hi);
            break;
        case AST_ID:
            insertarSimbolo(declaracion->v);
            break;
        default:
            break;
    }
}

//Funcion que verifica las sentencias con chequeo de tipos (asignacion y return)
static void chequeo_sentencias(Nodo* sentencias, tipoDef tipoFuncion) {
    if (!sentencias) return;
    switch (sentencias->tipo) {
        case AST_ASIGNACION: {
            Nodo* id   = sentencias->hi;
            Nodo* expresion = sentencias->hd;

            if (!id || id->tipo != AST_ID || !id->v || !id->v->s) {
                fprintf(stderr, "Error: lado izquierdo invalido en asignacion\n");
                exit(1);
            }

            Simbolo* s = buscarSimbolo(id->v->s);
            if (!s) {
                fprintf(stderr, "Error: variable '%s' no declarada\n", id->v->s);
                exit(1);
            }

            tipoDef tipoExpresion = chequeo_expresion(expresion);

            if (s->v->tipoDef != tipoExpresion) {
                fprintf(stderr,
                        "Error: asignacion incompatible en '%s' (esperado %s, dado %s)\n",
                        id->v->s,
                        (s->v->tipoDef == INT ? "int" :
                         s->v->tipoDef == BOOL ? "bool" : "void"),
                        (tipoExpresion == INT ? "int" :
                         tipoExpresion == BOOL ? "bool" : "void"));
                exit(1);
            }
            break;
        }

        case AST_RETURN: {
            tipoDef tipoReturn = sentencias->hi ? chequeo_expresion(sentencias->hi) : VOID;

            if (tipoReturn != tipoFuncion) {
                fprintf(stderr,
                        "Error: tipo de return incorrecto (esperado %s, dado %s)\n",
                        (tipoFuncion == INT ? "int" :
                         tipoFuncion == BOOL ? "bool" : "void"),
                        (tipoReturn == INT ? "int" :
                         tipoReturn == BOOL ? "bool" : "void"));
                exit(1);
            }
            break;
        }

        case AST_SEQ:
            chequeo_sentencias(sentencias->hi, tipoFuncion);
            chequeo_sentencias(sentencias->hd, tipoFuncion);
            break;

        case AST_STMTS:
            chequeo_sentencias(sentencias->hi, tipoFuncion);
            chequeo_sentencias(sentencias->hd, tipoFuncion);
            break;

        default:
            break;
    }
}

static tipoDef chequeo_expresion(Nodo* expresion) {
    if (!expresion) return VOID;

    switch (expresion->tipo) {
        case AST_INT:
            return INT;

        case AST_BOOL:
            return BOOL;

        case AST_ID: {
            if (!expresion->v || !expresion->v->s) {
                fprintf(stderr, "Error: identificador invalido en expresion\n");
                exit(1);
            }
            Simbolo* s = buscarSimbolo(expresion->v->s);
            if (!s) {
                fprintf(stderr, "Error: variable '%s' no declarada\n", expresion->v->s);
                exit(1);
            }
            return s->v->tipoDef;
        }

        case AST_OP: {
            tipoDef izq = chequeo_expresion(expresion->hi);
            tipoDef der = chequeo_expresion(expresion->hd);

            if (izq != INT || der != INT) {
                fprintf(stderr, "Error: operador %c requiere enteros\n", expresion->v->op);
                exit(1);
            }
            return INT;
        }

        default:
            return VOID;
    }
}
