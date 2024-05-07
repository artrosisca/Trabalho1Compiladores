%{
#include "parser.tab.h"
/*
    yytext + 1 para ignorar o "#"
*/
%}

E               "E"
OU              "OU"
NAO             "NAO"
NUMERO          [0-9]
LETRA           [a-zA-Z]
NOME            {LETRA}({LETRA})?*
AGENTE          "#"{NOME}
CRENCAS         "crencas: "
objetivos       "objetivos: "
PLANOS          "planos: "

%%
{AGENTE}           { sscanf(yytext + 1, "%s", yylval.s); return AGENTE; }
{CRENCAS}              { return CRENCAS; }
{OBJETIVOS}            { return OBJETIVOS; }
{PLANOS}               { return PLANOS; }
"{"                    { return ABRE_CHAVE; }
"}"                    { return FECHA_CHAVE; }
";"                    { return PONTO_VIRGULA; }
"("                    { return ABRE_PARENTESE; }
")"                    { return FECHA_PARENTESE; }
{NOME}                 { sscanf(yytext, "%s", yylval.s); return NOME; }
{OU}                   { return OU; }
{E}                    { return E; }
{NAO}                  { return NAO; }
[ \t\r\n] 
.
%%
