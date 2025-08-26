#ifndef AST_H
#define AST_H

typedef enum {
    AST_INT,
    AST_BOOL,
    AST_ID,
    AST_OP,
    AST_NONTERM
} AstTipo;

typedef struct {
    long   i;
    int    b;
    char  *s;
    char    op;
} AstValor;

typedef struct Nodo {
    AstTipo tipo;
    AstValor v;
    struct Nodo *hi;
    struct Nodo *hd;
} Nodo;

Nodo* nodo_hoja(AstTipo t, AstValor v);
Nodo* nodo_binario(AstTipo t, AstValor v, Nodo* hi, Nodo* hd);
void imprimir_ast(Nodo* nodo, int nivel);

#endif