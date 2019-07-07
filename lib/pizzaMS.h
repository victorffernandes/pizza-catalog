#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pizza
{
    int codigo;
    char nome[50];
    char categoria[20];
    char descricao[50];
    float preco;
} TPizza;

typedef struct arvbm
{
    int nchaves, folha;
    int * codigo; //chave primaria
    int * pizza; // endereço da pizza dentro do arquivo dados
    int * filho;
    int prox;
} TABM;


// Imprime pizzanomeConcatenado
char * concatenarStrings(char * sufixo, char * prefixo);
FILE *criarIndexadorMS(char *nomeArquivo);
FILE *criarDadosMS(char *nomeArquivo);
FILE *criarRaizMS(char *nomeArquivo);
TABM * buscaRecursiva(FILE * indexador, FILE * dados, TABM * atual, int id, int t);
TABM * busca(char * nomeArquivo, int id, int t);
int *insere(char * nomeArquivo, int cod, char * nome, char * categoria,char * descricao, float preco, int t);
TPizza *buscaCategoria(char * categoria, char * dados);
void retiraPizza(TABM *ind, char * dados, char * indexador, int codigo, int t);
void alteraPizza(char * dados, char * indexador, int codigo);
TABM * acharNo(FILE * indexador, int i, int t);
void alteraPizza(char *dados, char *indexador, int codigo);
void alteraPizza(char *dados, char *indexador, int codigo);
TPizza * buscaPizza(char *dados, int codigo, int t);

int escreverRaiz(char *nomeArquivo, int p);
int recuperarRaiz(char *nomeArquivo);

