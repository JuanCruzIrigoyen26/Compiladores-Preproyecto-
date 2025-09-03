#ifndef AST_H
#define AST_H

// Tipos de nodos en el AST
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

// Tipo de declaración: variable o constante
typedef enum {
    CONST,
    VAR
} tipoDec;

// Tipo de dato: entero o booleano
typedef enum {
    INT,
    BOOL
} tipoDef;

// Valor asociado a cada nodo del AST
typedef struct {
    tipoDec tipoDec; // Si es VAR o CONST
    tipoDef tipoDef; // Tipo del valor (INT o BOOL)
    long   i;        // Valor si es entero
    int    b;        // Valor si es booleano
    char  *s;        // Nombre si es identificador
    char    op;      // Operador si es nodo de operación
    int flag;        // Marca si la variable fue inicializada (para el intérprete)
} AstValor;

// Nodo del AST
typedef struct Nodo {
    AstTipo tipo;    // Tipo de nodo (AST_INT, AST_OP, etc.)
    AstValor *v;     // Valor asociado al nodo
    struct Nodo *hi; // Hijo izquierdo (para binarios o secuencias)
    struct Nodo *hd; // Hijo derecho
} Nodo;


// Funciones auxiliares para construir el AST
Nodo* nodo_hoja(AstTipo t, AstValor v); // Crea un nodo hoja
Nodo* nodo_binario(AstTipo t, AstValor v, Nodo* hi, Nodo* hd); // Crea un nodo binario
void imprimir_ast(Nodo* nodo, int nivel); // Muestra el AST por pantalla
void generar_assembly(Nodo* raiz, FILE* out); // Genera código ensamblador a partir del AST

#endif