%option noyywrap
%{
#include "bison.tab.h"
/*
    yytext + 1 para ignorar o "#"
*/
%}

NUMERO          [0-9]
LETRA           [a-zA-Z]
NOME            {LETRA}({LETRA})*
NOME_AGENTE     "#"{NOME}
CRENCAS         "crencas: "
OBJETIVOS       "objetivos: "
PLANOS          "planos: "
E               "E"
OU              "OU"
NAO             "NAO"

%%
"%"                    { return PORCENTAGEM; }
"{"                    { return ABRE_CHAVE; }
"}"                    { return FECHA_CHAVE; }
";"                    { return PONTO_VIRGULA; }
"("                    { return ABRE_PARENTESES; }
")"                    { return FECHA_PARENTESES; }
{CRENCAS}              { return CRENCAS; }
{OBJETIVOS}            { return OBJETIVOS; }
{PLANOS}               { return PLANOS; }
{OU}                   { return OU; }
{E}                    { return E; }
{NAO}                  { return NAO; }
{NOME}                 { sscanf(yytext, "%s", yylval.s); return NOME; }
{NOME_AGENTE}          { sscanf(yytext + 1, "%s", yylval.s); return NOME_AGENTE; }
[ \t\r\n] 
.
%%
