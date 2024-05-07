#ifndef AST_CC65A_H
#define AST_CC65A_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// bison/yacc and flex/lex related declarations
int yyerror(const char *s, ...);    /*Esta função é chamada quando ocorre um erro de análise durante o processamento do arquivo de entrada. */
int yylex();                        /*Esta função é gerada pelo Flex e é responsável por retornar o próximo token do arquivo de entrada. */
int yyparse();                      /*Esta função é gerada pelo Bison e é responsável por iniciar o processo de análise sintática.*/
extern int yylineno;                /*Esta variável global é mantida pelo Flex e é usada para rastrear o número da linha atual no arquivo de entrada.*/
extern FILE *yyin;                  /*Esta variável global é usada para armazenar o ponteiro para o arquivo de entrada que está sendo analisado.*/
extern int yylineno;
extern FILE *yyin; 

#define TAMANHO_NOME 33

typedef enum
{
    _E,
    _OU,
    _NAO,
    _NOME,
} TIPO_CONTEXTO;

typedef struct Folha
{
    char nome_folha[TAMANHO_NOME];
    struct Folha *next;
} Folha;

typedef struct Contexto
{
    char first[TAMANHO_NOME];
    char second[TAMANHO_NOME];
    TIPO_CONTEXTO type;
} Contexto;

typedef struct Plano
{
    char nome_plano[TAMANHO_NOME];
    char condicao_nome[TAMANHO_NOME];
    Contexto *contexto;
    Folha *acoes;
    struct Plano *next;
} Plano;

typedef struct Agente
{
    char nome_agente[TAMANHO_NOME];
    Folha *crencas;
    Folha *objetivos;
    Plano *planos;
    struct Agente *next;
} Agente;

Folha *novaFolha(char name[], Folha *next);
Contexto *novoContexto(char first[], char second[], TIPO_CONTEXTO type);
Plano *novoPlano(char nome_plano[], char condicao_nome[], Contexto *Contexto, Folha *acoes, Plano *next);
Agente *novoAgente(char name[], Folha *crencas, Folha *objetivos, Plano *planos, Agente *next);
void eval(Agente *agentes);
void agenteASL(Agente *agentes);
void printContexto(FILE *asl_file, Contexto *Contexto);
void printAcoes(FILE *asl_file, Folha *acoes);
void* liberaArvore(Agente *agente);
void* liberaLista(Folha *lista);
void* liberaPlanos(Plano *planos);
void* liberaAgentes(Agente *agente);

#endif