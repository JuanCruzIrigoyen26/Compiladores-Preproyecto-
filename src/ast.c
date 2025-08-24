#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    AST_INT,
    AST_BOOL,
    AST_ID,
    AST_OP,
    AST_NONTERM
} AstTipo;

typedef union {
    long   i;
    int    b;
    char  *s;
    int    op;
} AstValor;

typedef struct Nodo {
    AstTipo tipo;
    AstValor v;
    struct Nodo *hi;
    struct Nodo *hd;
} Nodo;

Nodo* nodo_hoja(AstTipo t, AstValor v) {
    Nodo *n = (Nodo*)malloc(sizeof(Nodo));
    if (!n) { fprintf(stderr, "Out of memory\n"); exit(1); }
    n->tipo = t;
    n->hi = n->hd = NULL;
    n->v = v;

    if (t == AST_ID && v.s) {
        n->v.s = strdup(v.s);
    }
    return n;
}

Nodo* nodo_binario(AstTipo t, AstValor v, Nodo* hi, Nodo* hd) {
    Nodo* n = nodo_hoja(t, v);
    n->hi = hi;
    n->hd = hd;
    return n;
}
