#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

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


