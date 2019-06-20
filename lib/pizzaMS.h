#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pizza
{
    int codigo;
    char nome[50];
    char descricao[20];
    float preco;
} TPizza;

typedef struct arvbm
{
    int nchaves, folha;
    int * codigo; //chave primaria
    int * pizza; // endereço da pizza dentro do arquivo dados
    int * filho, prox;
} TABM;


// Imprime pizzanomeConcatenado
char * concatenarStrings(char * sufixo, char * prefixo);
FILE *criarIndexadorMS(char *nomeArquivo);
FILE *criarDadosMS(char *nomeArquivo);
FILE *criarRaizMS(char *nomeArquivo);
TABM * buscaRecursiva(FILE * indexador, FILE * dados, TABM * atual, int id);
int *insere(char * nomeArquivo, int cod, char *nome, char *descricao, float preco, int t);

