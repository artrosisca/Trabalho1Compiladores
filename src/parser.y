%{
#include "../src/arvore.h"
%}

%union {
    char s[33];
    struct folha *l;
    struct contexto *e;
    struct Plan *p;
    struct Agent *a;
}

%token<s> NOME NOME_AGENTE
%token CRENCAS OBJETIVOS PLANOS E OU NAO PORCENTAGEM ABRE_CHAVE FECHA_CHAVE ABRE_PARENTESES FECHA_PARENTESES PONTO_VIRGULA

%type<s> NOME_AGENTE list_element_name nome_plano condicao
%type<l> lista_simples CRENCAS OBJETIVOS acoes
%type<e> contexto
%type<p> PLANOS plano_set
%type<a> lista_agente

%start programa

%%
programa: lista_agente { eval($1); }

lista_agente: { $$ = NULL; }
    | NOME_AGENTE CRENCAS OBJETIVOS PLANOS PORCENTAGEM lista_agente { $$ = novo_agente($1, $2, $3, $4, $6); }
    ;

CRENCAS: CRENCAS ABRE_CHAVE lista_simples FECHA_CHAVE { $$ = $3; }
    ;

OBJETIVOS: OBJETIVOS ABRE_CHAVE lista_simples FECHA_CHAVE { $$ = $3; }
    ;

PLANOS: PLANOS ABRE_CHAVE plano_set FECHA_CHAVE { $$ = $3; }
    ;

plano_set: { $$ = NULL; }
    | nome_plano ABRE_PARENTESES condicao contexto acoes FECHA_PARENTESES PONTO_VIRGULA plano_set { $$ = novo_plano($1, $3, $4, $5, $8); }
    ;

condicao: NOME PONTO_VIRGULA
    ;

contexto: NOME E NOME PONTO_VIRGULA { $$ = novo_contexto($1, $3, E); }
    | NOME OU NOME PONTO_VIRGULA { $$ = novo_contexto($1, $3, OU); }
    | NAO NOME PONTO_VIRGULA { $$ = novo_contexto($2, NULL, NAO); }
    | NOME { $$ = novo_contexto($1, NULL, NOME); }
    ;

acoes: ABRE_CHAVE lista_simples FECHA_CHAVE { $$ = $2; }
    ;

lista_simples: { $$ = NULL; }
    | list_element_name PONTO_VIRGULA lista_simples { $$ = new_leaf(yyval.s, $3); }
    ;

nome_plano: NOME
    ;
    
NOME_AGENTE: NOME_AGENTE
    ;

list_element_name: NOME
    ;
%%