%{

#include <stdlib.h>
#include <stdio.h>
#include "ast.c"

%}
 
%token <v> ENTERO
%token <var> ID
%token T_INT
%token T_MAIN
%token T_VOID
%token T_RETURN
%token T_BOOL
%token T_SUMA
%token T_MULT
%token T_ASIGNACION
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

%union { Nodo* t; AstValor v }

%%


prog:
      tipo_main T_MAIN T_PA T_PC bloque { printf("No hay errores \n"); }
    ;

tipo_main:
      T_VOID
    | T_INT
    | T_BOOL
    ;

bloque:
      T_LA declaraciones sentencias T_LC
    ;

declaraciones:
      declaraciones decl_var
    | decl_var
    ;

sentencias:
    sentencias sentencia
    | sentencia
    ;

sentencia:
      asignacion
    | T_RETURN expr T_PUNTOC
    | T_RETURN T_PUNTOC
    ;

decl_var: 
      T_INT ID T_PUNTOC 
    | T_BOOL ID T_PUNTOC
    ;

asignacion: ID T_ASIGNACION expr T_PUNTOC

expr: VALOR               

    | expr T_SUMA expr    
    
    | expr T_MULT expr

    | T_PA expr T_PC   

    ;

VALOR : 
      ENTERO 
    | ID 
    | T_TRUE
    | T_FALSE           
    ;
 
%%