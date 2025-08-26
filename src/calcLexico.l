%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.c"
#include "calcSintaxis.tab.h"

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
"true"      { yylval.v.b = 1; 
				return T_TRUE;}
"false"     { yylval.v.b = 0; 
				return T_FALSE;}


{entero}+   { yylval.v.i = atol(yytext);
				return ENTERO;}

{letra}({letra}|{entero})* { yylval.v.s =  strdup(yytext);
                            	return ID;}

"+"		{ yylval.v.op = '+'; 
			return T_SUMA;}
"*"		{ yylval.v.op = '*';
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
"="		{ yylval.v.op = '=';
			return T_ASIGNACION;}		

.|\n                          ; /* ignore all the rest */

%%

void yyerror(){
    printf("%s%d\n","-> ERROR Sintactico en la linea: ",yylineno);
}

