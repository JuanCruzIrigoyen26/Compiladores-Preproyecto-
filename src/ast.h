#ifndef AST_H
#define AST_H

typedef enum {
    AST_INT,
    AST_BOOL,
    AST_ID,
    AST_OP,
    AST_DEC_TIPO,
    AST_RETURN,
    AST_FUNCION,
    AST_DECLS,
    AST_STMTS,
    AST_SEQ,
    AST_ASIGNACION
} AstTipo;

typedef enum {
    CONST,
    VAR
} tipoDec;

typedef enum {
    INT,
    BOOL
} tipoDef;

typedef struct {
    tipoDec tipoDec;
    tipoDef tipoDef;
    long   i;
    int    b;
    char  *s;
    char    op;
    int flag;
} AstValor;

typedef struct Nodo {
    AstTipo tipo;
    AstValor *v;       
    struct Nodo *hi;
    struct Nodo *hd;
} Nodo;

Nodo* nodo_hoja(AstTipo t, AstValor v);
Nodo* nodo_binario(AstTipo t, AstValor v, Nodo* hi, Nodo* hd);
void imprimir_ast(Nodo* nodo, int nivel);
void generar_assembly(Nodo* raiz, FILE* out);
#endif