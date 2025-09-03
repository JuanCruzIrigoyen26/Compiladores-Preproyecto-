%{
#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
Nodo* raiz = NULL;

extern int yylineno;
int yylex(void);
void yyerror(const char *s);
%}


%union {
    Nodo* nodo;      
    AstValor valor;
}


%token <valor> ENTERO
%token <valor> ID
%token T_SUMA
%token T_MULT
%token T_ASIGNACION
%token <valor> T_TRUE
%token <valor> T_FALSE

%token <valor> T_INT
%token <valor> T_MAIN
%token <valor> T_VOID
%token <valor> T_RETURN
%token <valor> T_BOOL
%token T_PA
%token T_PC
%token T_LA
%token T_LC
%token T_PUNTOC

%type <nodo> prog tipo_main bloque declaraciones sentencias sentencia decl_var asignacion expr VALOR

%left T_SUMA
%left T_MULT
%right T_ASIGNACION

%%

prog:
      tipo_main T_MAIN T_PA T_PC bloque
        { 
          AstValor v = {0};
          raiz = nodo_binario(AST_FUNCION, v, nodo_hoja(AST_ID, (AstValor){.s="main"}), $5);
          $$ = raiz;
        }
    ;

tipo_main:
      T_VOID { AstValor v = {.s="void"}; $$ = nodo_hoja(AST_DEC_TIPO, v); }
    | T_INT  { AstValor v = {.s="int"}; $$ = nodo_hoja(AST_DEC_TIPO, v); }
    | T_BOOL { AstValor v = {.s="bool"}; $$ = nodo_hoja(AST_DEC_TIPO, v); }
    ;

bloque:
      T_LA declaraciones sentencias T_LC 
        {
          AstValor v = {0};
          Nodo* decls = nodo_binario(AST_DECLS, v, $2, NULL);
          Nodo* stmts = nodo_binario(AST_STMTS, v, $3, NULL);
          $$ = nodo_binario(AST_SEQ, v, decls, stmts);
        }
    ;

declaraciones:
      /* vacío */ { $$ = NULL; }
    | declaraciones decl_var 
        { $$ = nodo_binario(AST_SEQ, (AstValor){0}, $1, $2); }
    ;

sentencias:
      /* vacío */ { $$ = NULL; }
    | sentencias sentencia 
        { $$ = nodo_binario(AST_SEQ, (AstValor){0}, $1, $2); }
    ;


sentencia:
      asignacion 
        { $$ = $1; }
    | T_RETURN expr T_PUNTOC 
        { $$ = nodo_binario(AST_RETURN, (AstValor){0}, $2, NULL); }
    | T_RETURN T_PUNTOC 
        { $$ = nodo_binario(AST_RETURN, (AstValor){0}, NULL, NULL); }
    ;

decl_var:
      T_INT ID T_PUNTOC  
        { AstValor v = { .tipoDec = VAR, .tipoDef = INT, .s = $2.s }; 
          $$ = nodo_hoja(AST_ID, v); }
    | T_BOOL ID T_PUNTOC 
        { AstValor v = { .tipoDec = VAR, .tipoDef = BOOL, .s = $2.s }; 
          $$ = nodo_hoja(AST_ID, v); }
    ;


asignacion:
      ID T_ASIGNACION expr T_PUNTOC 
        { 
            AstValor v = {0};
            $$ = nodo_binario(AST_ASIGNACION, (AstValor){0}, nodo_hoja(AST_ID, $1), $3); 
        }
    ;

expr: 
    VALOR { $$ = $1; }
    | expr T_SUMA expr 
        { $$ = nodo_binario(AST_OP, (AstValor){.op='+'}, $1, $3); }
    | expr T_MULT expr 
        { $$ = nodo_binario(AST_OP, (AstValor){.op='*'}, $1, $3); }
    | T_PA expr T_PC 
        { $$ = $2; }
    ;

VALOR:
      ENTERO   { $$ = nodo_hoja(AST_INT, $1); }
    | ID       { $$ = nodo_hoja(AST_ID, $1); }
    | T_TRUE   { $$ = nodo_hoja(AST_BOOL, $1); }
    | T_FALSE  { $$ = nodo_hoja(AST_BOOL, $1); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "-> ERROR Sintáctico en la línea %d: %s\n", yylineno, s);
}
