#include "arvore.h"

Folha *novaFolha(char nome[], Folha *next)
{
    Folha *tmp = (Folha *)malloc(sizeof(Folha));
    if (!tmp)
    {
        printf("Nao foi possivel alocar folha %s em %i.\n", nome, yylineno);
        exit(EXIT_FAILURE);
    }
    strncpy(tmp->nome_folha, nome, TAMANHO_NOME);
    tmp->next = next;
    return tmp;
}

Contexto *novoContexto(char first[], char second[], TIPO_CONTEXTO type)
{
    Contexto *tmp = (Contexto *)malloc(sizeof(Contexto));
    if (!tmp)
    {
        printf("Nao foi pssivel alocar contexto %i.\n", yylineno);
        exit(EXIT_FAILURE);
    }
    strncpy(tmp->first, first, TAMANHO_NOME);
    if (second)
    {
        strncpy(tmp->second, second, TAMANHO_NOME);
    }
    tmp->type = type;
    return tmp;
}

Plano *novoPlanoo(char nome_plano[], char condicao_nome[], Contexto *contexto, Folha *acoes, Plano *next)
{
    Plano *tmp = (Plano *)malloc(sizeof(Plano));
    if (!tmp)
    {
        printf("Nao foi pssivel alocar plano %s em %i.\n", nome_plano, yylineno);
        exit(EXIT_FAILURE);
    }
    strncpy(tmp->nome_plano, nome_plano, TAMANHO_NOME);
    strncpy(tmp->condicao_nome, condicao_nome, TAMANHO_NOME);
    tmp->contexto = contexto;
    tmp->acoes = acoes;
    tmp->next = next;
    return tmp;
}

Agente *novoAgente(char nome[], Folha *crencas, Folha *objetivos, Plano *planos, Agente *next)
{
    Agente *tmp = (Agente *)malloc(sizeof(Agente));
    if (!tmp)
    {
        printf("Couldn't allocate agent %s at %i.\n", nome, yylineno);
        exit(EXIT_FAILURE);
    }
    strncpy(tmp->nome_agente, nome, TAMANHO_NOME);
    tmp->crencas = crencas;
    tmp->objetivos = objetivos;
    tmp->planos = planos;
    tmp->next = next;
    return tmp;
}

void eval(Agente *agente)
{
    fclose(yyin);
    if (!agente)
    {
        // unreachable
        printf("Not valid agent.");
        exit(EXIT_FAILURE);
    }
    FILE *jason_file = fopen("./jason/main.mas2j", "w");
    if (!jason_file)
    {
        printf("Could not open mas2j file");
        exit(EXIT_FAILURE);
    }
    fprintf(jason_file, "MAS cc64a {");
    fprintf(jason_file, "\n\tagents: ");
    Agente *agente_temporario = agente;
    for (; agente; agente = agente->next)
    {
        fprintf(jason_file, "%s; ", agente->nome_agente);
        agenteASL(agente);
        liberaArvore(agente);
    }
    liberaAgentes(agente_temporario);
    fprintf(jason_file, "\n}");
    fclose(jason_file);
}

void agenteASL(Agente *agente)
{
    char buffer[64];
    char jason_path[] = "./jason/";
    int string_len = strlen(jason_path) + TAMANHO_NOME + strlen(".asl");
    snprintf(buffer, string_len, "%s%s.asl", jason_path, agente->nome_agente);
    FILE *asl_file = fopen(buffer, "w");
    if (!asl_file)
    {
        printf("Nao foi possivel abrir o arquivo asl %s", agente->nome_agente);
        exit(EXIT_FAILURE);
    }
    for (Folha *crencas = agente->crencas; crencas; crencas = crencas->next)
    {
        fprintf(asl_file, "%s.\n", crencas->nome_folha);
    }
    fprintf(asl_file, "\n");
    for (Folha *objetivos = agente->objetivos; objetivos; objetivos = objetivos->next)
    {
        fprintf(asl_file, "!%s.\n", objetivos->nome_folha);
    }
    fprintf(asl_file, "\n");
    for (Plano *planos = agente->planos; planos; planos = planos->next)
    {
        fprintf(asl_file, "+!%s: ", planos->condicao_nome);
        printContexto(asl_file, planos->contexto);
        printAcoes(asl_file, planos->acoes);
    }
    fclose(asl_file);
}

void printContexto(FILE *asl_file, Contexto *contexto)
{
    switch (contexto->type)
    {
    case _E:
        fprintf(asl_file, "%s & %s\n", contexto->first, contexto->second);
        break;
    case _OU:
        fprintf(asl_file, "%s | %s\n", contexto->first, contexto->second);
        break;
    case _NAO:
        fprintf(asl_file, "not %s\n", contexto->first);
        break;
    case _NOME:
        fprintf(asl_file, "%s\n", contexto->first);
        break;
    default:
        printf("Contexto nao identificado"); 
        exit(EXIT_FAILURE);
        break;
    }
}

void printAcoes(FILE *asl_file, Folha *acoes)
{
    fprintf(asl_file, "\t<- ");
    for (; acoes->next; acoes = acoes->next)
    {
        fprintf(asl_file, "\t%s;\n", acoes->nome_folha);
    }
    fprintf(asl_file, "\t%s.\n\n", acoes->nome_folha);
}

void *liberaArvore(Agente *agente)
{
    if (!agente)
        return NULL;
    agente->crencas = liberaLista(agente->crencas);
    agente->objetivos = liberaLista(agente->objetivos);
    agente->planos = liberaPlanoos(agente->planos);
    return NULL;
}

void *liberaLista(Folha *lista)
{
    if (!lista)
        return NULL;
    if (lista->next)
        lista->next = liberaLista(lista->next);
    free(lista);
    return NULL;
}

void *freePlanoos(Plano *planos)
{
    if (!planos)
        return NULL;
    if (planos->next)
        planos->next = freePlanoos(planos->next);
    planos->acoes = freeLista(planos->acoes);
    free(planos->contexto);
    free(planos);
    return NULL;
}

void *liberaAgentes(Agente *agente)
{
    if (!agente)
        return NULL;
    if (agente->next)
        agente->next = (agente->next);
    free(agente);
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage ./cc64a <FILENAME>");
        exit(EXIT_FAILURE);
    }
    yyin = fopen(argv[1], "r");
    if (!yyin)
    {
        printf("Bad nag file");
        exit(EXIT_FAILURE);
    }
    yyparse();
}

int yyerror(char const *, ...)
{
    return 0;
}
