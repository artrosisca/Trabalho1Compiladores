#include "arvore.h"// Inclui o arquivo de cabeçalho arvore.h, que contém as definições de structs e protótipos de funções.

// Protótipos das funções para manipulação da árvore
void agenteASL(Agente *agentes);
void printContexto(FILE *arquivo_asl, Contexto *contexto);
void printAcoes(FILE *arquivo_asl, Folha *acoes);
void* liberaArvore(Agente *agente);
void* liberaLista(Folha *lista);
void* liberaPlanos(Plano *planos);
void* liberaAgentes(Agente *agente);

// Função principal que lê o arquivo de entrada e inicia o parser.
int main(int argc, char **argv) {
    // Verifica se o nome do arquivo de entrada foi passado como argumento.
    if (argc < 2)
    {
        printf("Usagem: ./trab <FILENAME> (no caso, teste.nag)");
        exit(EXIT_FAILURE);
    }
    // Abre o arquivo de entrada.
    yyin = fopen(argv[1], "r");
    if (!yyin)
    {
        printf("Arquivo nag nao encontrado");
        exit(EXIT_FAILURE);
    }
    // Chama o parser.
    yyparse();
}

// Função de erro do parser.
int yyerror(char const *, ...) {
    return 0;
}

// Função para criar uma nova folha na árvore.
Folha *novaFolha(char nome[], Folha *proximo) {
    // Aloca memória para uma nova folha.
    Folha *tmp = (Folha *)malloc(sizeof(Folha));
    if (!tmp)
    {
        printf("Nao foi possivel alocar folha %s em %i.\n", nome, yylineno);
        exit(EXIT_FAILURE);
    }
    // Copia o nome para a folha.
    strncpy(tmp->nome_folha, nome, TAMANHO_NOME);
    tmp->proximo = proximo; // Define o próximo ponteiro da folha.
    return tmp; // Retorna a nova folha.
}

// Função para criar um novo contexto.
Contexto *novoContexto(char primeiro[], char segundo[], TIPO_CONTEXTO type) {
    // Aloca memória para um novo contexto.
    Contexto *tmp = (Contexto *)malloc(sizeof(Contexto));
    if (!tmp)
    {
        printf("Nao foi pssivel alocar contexto %i.\n", yylineno);
        exit(EXIT_FAILURE);
    }
    // Copia os valores para o contexto.
    strncpy(tmp->primeiro, primeiro, TAMANHO_NOME);
    if (segundo)
    {
        strncpy(tmp->segundo, segundo, TAMANHO_NOME);
    }
    tmp->type = type; // Define o tipo do contexto.
    return tmp; // Retorna o novo contexto.
}

// Função para criar um novo plano.
Plano *novoPlano(char nome_plano[], char condicao_nome[], Contexto *contexto, Folha *acoes, Plano *proximo) {
    // Aloca memória para um novo plano.
    Plano *tmp = (Plano *)malloc(sizeof(Plano));
    if (!tmp)
    {
        printf("Nao foi pssivel alocar o plano %s em %i.\n", nome_plano, yylineno);
        exit(EXIT_FAILURE);
    }
    // Copia os valores para o plano.
    strncpy(tmp->nome_plano, nome_plano, TAMANHO_NOME);
    strncpy(tmp->condicao_nome, condicao_nome, TAMANHO_NOME);
    tmp->contexto = contexto;
    tmp->acoes = acoes;
    tmp->proximo = proximo; // Define o próximo ponteiro do plano.
    return tmp; // Retorna o novo plano.
}

// Função para criar um novo agente.
Agente *novoAgente(char nome[], Folha *crencas, Folha *objetivos, Plano *planos, Agente *proximo) {
    // Aloca memória para um novo agente.
    Agente *tmp = (Agente *)malloc(sizeof(Agente));
    if (!tmp)
    {
        printf("Nao foi possivel alocar o agente %s em %i.\n", nome, yylineno);
        exit(EXIT_FAILURE);
    }
    // Copia os valores para o agente.
    strncpy(tmp->nome_agente, nome, TAMANHO_NOME);
    tmp->crencas = crencas;
    tmp->objetivos = objetivos;
    tmp->planos = planos;
    tmp->proximo = proximo; // Define o próximo ponteiro do agente.
    return tmp; // Retorna o novo agente.
}

void eval(Agente *agente) {
    // Fecha o arquivo de entrada.
    fclose(yyin);
    // Verifica se o agente é válido.
    if (!agente)
    {
        printf("Agente não válido.");
        exit(EXIT_FAILURE);
    }
    // Abre o arquivo main.mas2j no modo de escrita.
    FILE *jason_file = fopen("./jason/main.mas2j", "w");
    if (!jason_file)
    {
        printf("Não foi possível abrir o arquivo main.mas2j");
        exit(EXIT_FAILURE);
    }
    // Escreve o cabeçalho do arquivo .mas2j.
    fprintf(jason_file, "MAS trabalho {");
    fprintf(jason_file, "\n\tagentes: ");
    // Salva o ponteiro inicial do agente para liberar a memória posteriormente.
    Agente *agente_temporario = agente;
    // Loop sobre todos os agentes.
    for (; agente; agente = agente->proximo)
    {
        // Escreve o nome do agente no arquivo .mas2j.
        fprintf(jason_file, "%s; ", agente->nome_agente);
        // Gera o arquivo .asl correspondente ao agente.
        agenteASL(agente);
        // Libera a memória alocada para a árvore de cada agente.
        liberaArvore(agente);
    }
    // Libera a memória alocada para a lista de agentes.
    liberaAgentes(agente_temporario);
    // Escreve o final do arquivo .mas2j.
    fprintf(jason_file, "\n}");
    // Fecha o arquivo .mas2j.
    fclose(jason_file);
}

void agenteASL(Agente *agente) {
    char buffer[64];
    char jason_path[] = "./jason/";
    // Calcula o tamanho da string para o nome do arquivo .asl.
    int string_len = strlen(jason_path) + TAMANHO_NOME + strlen(".asl");
    snprintf(buffer, string_len, "%s%s.asl", jason_path, agente->nome_agente);
    // Abre o arquivo .asl correspondente ao agente no modo de escrita.
    FILE *arquivo_asl = fopen(buffer, "w");
    if (!arquivo_asl)
    {
        printf("Não foi possível abrir o arquivo .asl para o agente %s", agente->nome_agente);
        exit(EXIT_FAILURE);
    }
    // Escreve as crenças do agente no arquivo .asl.
    for (Folha *crencas = agente->crencas; crencas; crencas = crencas->proximo)
    {
        fprintf(arquivo_asl, "%s.\n", crencas->nome_folha);
    }
    fprintf(arquivo_asl, "\n");
    // Escreve os objetivos do agente no arquivo .asl.
    for (Folha *objetivos = agente->objetivos; objetivos; objetivos = objetivos->proximo)
    {
        fprintf(arquivo_asl, "!%s.\n", objetivos->nome_folha);
    }
    fprintf(arquivo_asl, "\n");
    // Escreve os planos do agente no arquivo .asl.
    for (Plano *planos = agente->planos; planos; planos = planos->proximo)
    {
        fprintf(arquivo_asl, "+!%s: ", planos->condicao_nome);
        // Escreve o contexto do plano no arquivo .asl.
        printContexto(arquivo_asl, planos->contexto);
        // Escreve as ações do plano no arquivo .asl.
        printAcoes(arquivo_asl, planos->acoes);
    }
    // Fecha o arquivo .asl.
    fclose(arquivo_asl);
}

void printContexto(FILE *arquivo_asl, Contexto *contexto) {
    // Switch para imprimir o contexto com base no tipo.
    switch (contexto->type)
    {
    case _E:
        fprintf(arquivo_asl, "%s & %s\n", contexto->primeiro, contexto->segundo);
        break;
    case _OU:
        fprintf(arquivo_asl, "%s | %s\n", contexto->primeiro, contexto->segundo);
        break;
    case _NAO:
        fprintf(arquivo_asl, "não %s\n", contexto->primeiro);
        break;
    case _NOME:
        fprintf(arquivo_asl, "%s\n", contexto->primeiro);
        break;
    default:
        printf("Contexto não identificado");
        exit(EXIT_FAILURE);
        break;
    }
}

void printAcoes(FILE *arquivo_asl, Folha *acoes) {
    // Escreve as ações no arquivo .asl.
    fprintf(arquivo_asl, "\t<- ");
    for (; acoes->proximo; acoes = acoes->proximo)
    {
        fprintf(arquivo_asl, "\t%s;\n", acoes->nome_folha);
    }
    fprintf(arquivo_asl, "\t%s.\n\n", acoes->nome_folha);
}

// Função para liberar a memória alocada para a árvore.
void *liberaArvore(Agente *agente) {
    if (!agente)
        return NULL;
    // Libera as listas de crenças, objetivos e planos do agente.
    agente->crencas = liberaLista(agente->crencas);
    agente->objetivos = liberaLista(agente->objetivos);
    agente->planos = liberaPlanos(agente->planos);
    return NULL;
}

// Função para liberar a memória alocada para uma lista de folhas.
void *liberaLista(Folha *lista) {
    if (!lista)
        return NULL;
    if (lista->proximo)
        lista->proximo = liberaLista(lista->proximo);
    free(lista);
    return NULL;
}

// Função para liberar a memória alocada para uma lista de planos.
void *liberaPlanos(Plano *planos) {
    if (!planos)
        return NULL;
    if (planos->proximo)
        planos->proximo = liberaPlanos(planos->proximo);
    // Libera as ações do plano.
    planos->acoes = liberaLista(planos->acoes);
    free(planos->contexto);
    free(planos);
    return NULL;
}

// Função para liberar a memória alocada para uma lista de agentes.
void *liberaAgentes(Agente *agente) {
    if (!agente)
        return NULL;
    if (agente->proximo)
        agente->proximo = (agente->proximo);
    free(agente);
    return NULL;
}