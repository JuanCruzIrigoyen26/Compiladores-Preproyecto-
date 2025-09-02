#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static void generar_funcion(Nodo* funcion, FILE* out);
static void generar_sentencias(Nodo* sentencias, FILE* out);
static void generar_declaracion(Nodo* declaracion, FILE* out);
static void generar_expresion(Nodo* expresion, FILE* out);

Nodo* nodo_hoja(AstTipo t, AstValor v) {
    Nodo *n = (Nodo*)malloc(sizeof(Nodo));
    if (!n) { 
        fprintf(stderr, "Out of memory\n"); 
        exit(1); 
    }
    n->tipo = t;
    n->hi = n->hd = NULL;
    n->v = malloc(sizeof(AstValor));

    if (!n->v) { 
        fprintf(stderr, "Out of memory\n"); 
        exit(1); 
    }

    *n->v = v;

    if (t == AST_ID && v.s) {
        n->v->s = strdup(v.s);
    }
    return n;
}

Nodo* nodo_binario(AstTipo t, AstValor v, Nodo* hi, Nodo* hd) {
    Nodo *n = malloc(sizeof(Nodo));
    if (!n){
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    n->tipo = t;
    n->v = malloc(sizeof(AstValor));

    if (!n->v) { 
        fprintf(stderr, "Out of memory\n"); 
        exit(1); 
    }

    *n->v = v;
    n->hi = hi;
    n->hd = hd;
    return n;
}

void imprimir_ast(Nodo* nodo, int nivel) {
    if (!nodo) return;

    for (int i = 0; i < nivel; i++) printf("  ");

    switch (nodo->tipo) {
        case AST_INT:
            printf("INT: %ld\n", nodo->v->i);
            break;
        case AST_BOOL:
            printf("BOOL: %s\n", nodo->v->b ? "true" : "false");
            break;
        case AST_ID:
            if (nodo->v->tipoDec == VAR) {
                const char *tipoStr = (nodo->v->tipoDef == INT ? "int" : "bool");
                printf("ID (%s): %s\n", tipoStr, nodo->v->s ? nodo->v->s : "(null)");
            } else {
                printf("ID: %s\n", nodo->v->s ? nodo->v->s : "(null)");
            }
            break;
        case AST_OP:
            printf("OP: %c\n", nodo->v->op);
            break;
        case AST_DEC_TIPO:
            printf("DECL: %s\n", nodo->v->s ? nodo->v->s : "(null)");
            break;
        case AST_RETURN:
            printf("RETURN\n");
            break;
        case AST_FUNCION:
            printf("FUNCION\n");
            break;
        case AST_DECLS:
            printf("DECLS\n");
            break;
        case AST_STMTS:
            printf("STMTS\n");
            break;
        case AST_SEQ:
            printf("SEQ\n");
            break;
        case AST_ASIGNACION:
            printf("ASIGNACION\n");
            break;
        default:
            printf("Nodo desconocido\n");
    }

    imprimir_ast(nodo->hi, nivel + 1);
    imprimir_ast(nodo->hd, nivel + 1);
}

void generar_assembly(Nodo* raiz, FILE* out) {
    if (!raiz) {
        fprintf(out, ": AST vacío\n");
        return;
    }
    if (raiz->tipo == AST_FUNCION) {
        generar_funcion(raiz, out);
    } else {
        fprintf(out, ": nodo de raíz con tipo desconocido=%d\n", raiz->tipo);
    }
}

static void generar_funcion(Nodo* funcion, FILE* out) {
    if (!funcion) return;
    if (funcion->tipo != AST_FUNCION) {
        fprintf(out, "; nodo raíz no es función\n");
        return;
    }

    const char* nombreFuncion;
    if (funcion->hi && funcion->hi->v && funcion->hi->v->s) {
        nombreFuncion = funcion->hi->v->s;
        fprintf(out, "FUNC %s\n", nombreFuncion);
    } else {
        fprintf(out, "FUNC\n");
    }

    Nodo* bloque = funcion->hd;
    if (bloque && bloque->hi) generar_declaracion(bloque->hi, out); 
    if (bloque && bloque->hd) generar_sentencias(bloque->hd, out);  

    fprintf(out, "ENDFUNC\n");
}

static void generar_expresion(Nodo* expresion, FILE* out) {
    if (!expresion) return;
    switch (expresion->tipo) {
        case AST_INT:
            fprintf(out, "PUSHI %ld\n", expresion->v->i);
            break;
        case AST_BOOL:
            fprintf(out, "PUSHB %d\n", expresion->v->b);
            break;
        case AST_ID:
            fprintf(out, "LOAD %s\n", expresion->v->s);
            break;
        case AST_OP:
            if (expresion->hi) generar_expresion(expresion->hi, out);
            if (expresion->hd) generar_expresion(expresion->hd, out);
            if (expresion->v->op == '+') fprintf(out, "ADD\n");
            else if (expresion->v->op == '*') fprintf(out, "MUL\n");
            else fprintf(out, ": OP desconocida %c\n", expresion->v->op);
            break;
        default:
            fprintf(out, ": expr con tipo desconocido=%d\n", expresion->tipo);
            break;
    }
}


static void generar_sentencias(Nodo* sentencias, FILE* out) {
    if (!sentencias) return;
    switch (sentencias->tipo) {
        case AST_ASIGNACION: {
            Nodo* nodoId = sentencias->hi;
            Nodo* ladoDerecho = sentencias->hd;
            generar_expresion(ladoDerecho, out);
            fprintf(out, "STORE %s\n", nodoId->v->s);
            break;
        }
        case AST_RETURN:
            if (sentencias->hi) generar_expresion(sentencias->hi, out);
            else fprintf(out, "PUSHI 0\n");
            fprintf(out, "RET\n");
            break;
        case AST_SEQ:
        case AST_STMTS:
            generar_sentencias(sentencias->hi, out);
            generar_sentencias(sentencias->hd, out);
            break;
        default:
            fprintf(out, ": sentencia de tipo desconocido=%d\n", sentencias->tipo);
            break;
    }
}

static void generar_declaracion(Nodo* declaracion, FILE* out) {
    if (!declaracion) return;
    switch (declaracion->tipo) {
        case AST_SEQ:
        case AST_DECLS:
            generar_declaracion(declaracion->hi, out);
            generar_declaracion(declaracion->hd, out);
            break;
        case AST_ID:
            if (declaracion->v->tipoDef == INT) {
                fprintf(out, "DECL %s int\n", declaracion->v->s);
            } else {
                fprintf(out, "DECL %s bool\n", declaracion->v->s);
            }
            break;
        default:
            fprintf(out, "; declaración de tipo desconocido=%d\n", declaracion->tipo);
            break;
    }
}



