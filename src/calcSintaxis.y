%{

#include <stdlib.h>
#include <stdio.h>

%}
 
%token ENTERO
%token ID
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
%token T_TRUE
%token T_FALSE

%type expr
%type VALOR

%left T_SUMA
%left T_MULT

/* %union {tree t ; int v ; %char var;} */

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