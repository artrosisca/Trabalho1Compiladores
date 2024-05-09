#ifndef AST_CC65A_H // Verifica se o arquivo de cabeçalho AST_CC65A_H ainda não foi incluído
#define AST_CC65A_H // Define o arquivo de cabeçalho AST_CC65A_H para evitar inclusões múltiplas

#include <stdio.h> // Inclui o cabeçalho da biblioteca de entrada e saída padrão
#include <string.h> // Inclui o cabeçalho da biblioteca para manipulação de strings
#include <stdlib.h> // Inclui o cabeçalho da biblioteca padrão de alocação de memória

// declaracoes bison e flex
int yyerror(const char *s, ...); // Declaração da função de erro do analisador sintático gerado pelo Bison
int yylex(); // Declaração da função gerada pelo Flex para retornar o próximo token
int yyparse(); // Declaração da função gerada pelo Bison para iniciar a análise sintática
extern int yylineno; // Declaração da variável global mantida pelo Flex para rastrear o número da linha atual
extern FILE *yyin; // Declaração da variável global para armazenar o ponteiro para o arquivo de entrada

#define TAMANHO_NOME 50 // Define o tamanho máximo para o nome

// Declaração de uma enumeração para os tipos de contexto
typedef enum
{
    _E, // Conjunção
    _OU, // Disjunção
    _NAO, // Negação
    _NOME, // Nome
} TIPO_CONTEXTO;

// Declaração da estrutura para representar uma folha na árvore
typedef struct Folha
{
    char nome_folha[TAMANHO_NOME]; // Nome da folha
    struct Folha *proximo; // Ponteiro para a próxima folha
} Folha;

// Declaração da estrutura para representar um contexto
typedef struct Contexto
{
    char primeiro[TAMANHO_NOME]; // Primeiro elemento do contexto
    char segundo[TAMANHO_NOME]; // Segundo elemento do contexto
    TIPO_CONTEXTO type; // Tipo de contexto
} Contexto;

// Declaração da estrutura para representar um plano
typedef struct Plano
{
    char nome_plano[TAMANHO_NOME]; // Nome do plano
    char condicao_nome[TAMANHO_NOME]; // Nome da condição
    Contexto *contexto; // Contexto do plano
    Folha *acoes; // Ações do plano
    struct Plano *proximo; // Ponteiro para o próximo plano
} Plano;

// Declaração da estrutura para representar um agente
typedef struct Agente
{
    char nome_agente[TAMANHO_NOME]; // Nome do agente
    Folha *crencas; // Lista de crenças do agente
    Folha *objetivos; // Lista de objetivos do agente
    Plano *planos; // Lista de planos do agente
    struct Agente *proximo; // Ponteiro para o próximo agente
} Agente;

// Protótipos das funções para manipulação da árvore
Folha *novaFolha(char name[], Folha *proximo);
Contexto *novoContexto(char primeiro[], char segundo[], TIPO_CONTEXTO type);
Plano *novoPlano(char nome_plano[], char condicao_nome[], Contexto *contexto, Folha *acoes, Plano *proximo);
Agente *novoAgente(char name[], Folha *crencas, Folha *objetivos, Plano *planos, Agente *proximo);
void eval(Agente *agentes);

#endif // Finaliza a verificação da inclusão múltipla do arquivo de cabeçalho