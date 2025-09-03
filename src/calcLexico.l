%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"
#include "calcSintaxis.tab.h"
%}


%option noyywrap
%option yylineno


entero [0-9]
letra [a-zA-Z]


%%

"int"       { return T_INT; }

"bool"      { return T_BOOL; }

"void"      { return T_VOID; }

"main"      { yylval.valor.s = strdup("main"); return ID; }

"return"    { return T_RETURN; }

"true"      { yylval.valor.b = 1;
              return T_TRUE; }

"false"     { yylval.valor.b = 0;
              return T_FALSE; }

{entero}+   { yylval.valor.i = atol(yytext);
              return ENTERO; }

{letra}({letra}|{entero})* {
              yylval.valor.s = strdup(yytext);
              return ID;
}

"+"         { yylval.valor.op = '+';
			  return T_SUMA; }
"*"         { yylval.valor.op = '*'; 
			  return T_MULT; }
"="         { yylval.valor.op = '='; 
			  return T_ASIGNACION; }

";"         { return T_PUNTOC; }
"("         { return T_PA; }
")"         { return T_PC; }
"{"         { return T_LA; }
"}"         { return T_LC; }

[ \t\r\n]+  ;   /* ignora espacios y saltos de línea */
.           ;   /* ignora cualquier otro caracter inválido */

%%
