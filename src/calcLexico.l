%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "calcSintaxis.tab.h"
#include "ast.c"

%}


%option noyywrap
%option yylineno


entero [0-9]
letra [a-zA-Z]


%%


"int"       { printf("Tipo int : %s\n", yytext);
				return T_INT;}
"bool"      { printf("Tipo bool : %s\n", yytext);
				return T_BOOL;}
"void"      { printf("void : %s\n", yytext);
				return T_VOID;}
"main"      { printf("main : %s\n", yytext);
				return T_MAIN;}
"return"    { printf("return : %s\n", yytext);
				return T_RETURN;}
"true"      { yyival.v.b = 1; 
				return T_TRUE;}
"false"     { yyival.v.b = 0; 
				return T_FALSE;}


{entero}+   { yyival.v.i = atol(yytext));
				return ENTERO;}

{letra}({letra}|{entero})* { yyival.v.s =  strdup(yytext);
                            	return ID;}

"+"		{ yyival.v.op = "+"; 
			return T_SUMA;}
"*"		{ yyival.v.op = "*";
			return T_MULT;}
";"		{printf("%s\n",yytext);
			return T_PUNTOC;}
"("		{printf("%s\n",yytext);
			return T_PA;}
")"		{printf("%s\n",yytext);
			return T_PC;}
"{"		{printf("%s\n",yytext);
			return T_LA;}
"}"		{printf("%s\n",yytext);
			return T_LC;}
"="		{printf("%s\n",yytext);
			return T_ASIGNACION;}		

.|\n                          ; /* ignore all the rest */

%%

void yyerror(){
    printf("%s%d\n","-> ERROR Sintactico en la linea: ",yylineno);
}

int main(int argc,char *argv[]){
    ++argv,--argc;
    if (argc > 0)
        yyin = fopen(argv[0],"r");
    else
        yyin = stdin;
    yyparse(); // llama al parser de Bison
}