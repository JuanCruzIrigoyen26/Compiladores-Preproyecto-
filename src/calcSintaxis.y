%{

#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

Nodo* raiz = NULL;

%}
 
%union { Nodo* t; AstValor v; }

%token <v> ENTERO
%token <v> ID
%token T_INT
%token T_MAIN
%token T_VOID
%token T_RETURN
%token T_BOOL
%token <v> T_SUMA
%token <v> T_MULT
%token <v> T_ASIGNACION
%token T_PA
%token T_PC
%token T_LA
%token T_LC
%token T_PUNTOC
%token <v> T_TRUE
%token <v> T_FALSE

%type <t> prog bloque declaraciones sentencias sentencia decl_var asignacion expr VALOR

%left T_SUMA
%left T_MULT


%%


prog:
      tipo_main T_MAIN T_PA T_PC bloque { printf("No hay errores \n");  raiz = $5; $$ = $5; }
    ;

tipo_main:
      T_VOID
    | T_INT
    | T_BOOL
    ;

bloque:
      T_LA declaraciones sentencias T_LC {$$ = nodo_binario(AST_NONTERM, (AstValor){0}, $2, $3);}
    ;

declaraciones:
      declaraciones decl_var {$$ = nodo_binario(AST_NONTERM, (AstValor){0}, $1, $2);}
    | decl_var {$$ = $1;}
    ;

sentencias:
    sentencias sentencia {$$ = nodo_binario(AST_NONTERM, (AstValor){0}, $1, $2);}
    | sentencia {$$ = $1;}
    ;

sentencia:
      asignacion {$$ = $1;}
    | T_RETURN expr T_PUNTOC {$$ = nodo_binario(AST_OP, (AstValor){.op='R'}, $2, NULL);}
    | T_RETURN T_PUNTOC {$$ = nodo_binario(AST_OP, (AstValor){.op='R'}, NULL, NULL);}


decl_var: 
      T_INT ID T_PUNTOC {$$ = nodo_hoja(AST_ID, $2);}
    | T_BOOL ID T_PUNTOC {$$ = nodo_hoja(AST_ID, $2);}
    ;

asignacion: ID T_ASIGNACION expr T_PUNTOC {$$ = nodo_binario(AST_OP, (AstValor){.op= '='}, nodo_hoja(AST_ID, $1), $3);}

expr: VALOR  {$$ = $1;}             

    | expr T_SUMA expr    {$$ = nodo_binario(AST_OP, $2, $1, $3);}
    
    | expr T_MULT expr  {$$ = nodo_binario(AST_OP, $2, $1, $3);}

    | T_PA expr T_PC  {$$ = $2;} 

    ;

VALOR : 
      ENTERO {$$ = nodo_hoja(AST_INT, $1);}
    | ID {$$ = nodo_hoja(AST_ID, $1);}
    | T_TRUE {$$ = nodo_hoja(AST_BOOL, $1);}
    | T_FALSE {$$ = nodo_hoja(AST_BOOL, $1);}       
    ;
 
%%