#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pizzaMS.h"

int tamanho_pizza_bytes()
{
	return sizeof(int) + // cod
		sizeof(char) * 50 + // nome
		sizeof(char) * 20 + // categoria
		sizeof(float); // preço
}

int tamanho_arvbm_bytes(int t){
  return sizeof(int) * 3 + // nchaves proximo folha
         sizeof(int) * ((t*2)-1)  + // codigo
         sizeof(int) * ((t*2)-1) + //pizza
         sizeof(int) * ((t*2)) ;//filho
}



TABM *cria(int t){
  TABM* novo = (TABM*)malloc(sizeof(TABM));
  novo->nchaves = 0;
  novo->codigo =(int*)malloc(sizeof(int)*((t*2)-1));
  novo->pizza = (int*) malloc(sizeof(int)*((t*2)-1));
  novo->folha = 1;
  novo->filho = (int*)malloc(sizeof(int)*t*2);
  novo->prox = NULL;
  int i;
  for(i=0; i<(t*2); i++) novo->filho[i] = NULL;
  return novo;
}


TPizza *pizza(int cod, char *nome, char *descricao, float preco)
{
	TPizza *p = (TPizza *) malloc(sizeof(TPizza));
	if (p) memset(p, 0, sizeof(TPizza));
	p->codigo = cod;
	strcpy(p->nome, nome);
	strcpy(p->descricao, descricao);
	p->preco = preco;
	return p;
}

int isEmpty(FILE * file){
  fseek(file, 0, SEEK_END);
  int estaVazioDados = (int)ftell(file) <= 0;
  if(estaVazioDados){
      fseek(file, 0, SEEK_SET);
      return 1;
  }
  return 0;
}

int lerPosicao(FILE * file){
  fseek(file, 0, SEEK_END);
  return ftell(file);
}

int *insere(char * nomeArquivo, int cod, char * nome, char * descricao, float preco, int t){
  /* if(busca(T, mat)) return T; */ // tem que fazer a busca antes
  FILE * indexador = criarIndexadorMS(nomeArquivo);
  FILE * dados = criarDadosMS(nomeArquivo);
  
  if(isEmpty(dados)){
    TABM * raiz;

    TPizza * p = pizza(cod, nome, descricao, preco);
    fwrite(&p, tamanho_pizza_bytes(), 1, dados);

    raiz=cria(t);
    raiz->codigo[0] = cod;
    raiz->pizza[0] = 0;
    raiz->nchaves=1;
    int r = fwrite(&raiz,tamanho_arvbm_bytes(t), 1, indexador);
    /* escreverRaiz(nomeArquivo, 0);
    printf("%d", recuperarRaiz(nomeArquivo)); */
    fclose(indexador);
    fclose(dados);
    return r != 0 ;
  }

  /* if(raiz->nchaves == (2*t)-1){
    TABM *S = cria(t);
    S->nchaves=0;
    S->folha = 0;
    S->filho[0] = raiz;
    S = divisao(S,1,T,t);
    S = insere_nao_completo(S, mat, t);
    return S;
  }
  T = insere_nao_completo(T, mat, t);
  return T; */
}


char *concatenarStrings(char *sufixo, char *prefixo)
{
    int tamanhoByteString = strlen(sufixo) + strlen(prefixo);
    char *nomeConcatenado = (char *)malloc(sizeof(char) * tamanhoByteString);
    strcat(nomeConcatenado, sufixo);
    strcat(nomeConcatenado, prefixo);
    return nomeConcatenado;
}

FILE *criarDadosMS(char *nomeArquivo)
{
    char *nomeArquivoDados = concatenarStrings(nomeArquivo, "_dados.o");

    FILE *arquivoDados = fopen(nomeArquivoDados, "rb+");
    if (arquivoDados == NULL)
    {
        arquivoDados = fopen(nomeArquivoDados, "wb+");
        if (arquivoDados == NULL){
            printf("Ocorreu um erro na abertura do arquivo de dados");
            exit(1);
        }
    }

    return arquivoDados;
}

FILE *criarIndexadorMS(char *nomeArquivo)
{
    char *nomeArquivoIndexador = concatenarStrings(nomeArquivo, "_indexador.o");

    FILE *arquivoIndexador = fopen(nomeArquivoIndexador, "rb+");
    if (arquivoIndexador == NULL)
    {
        arquivoIndexador = fopen(nomeArquivoIndexador, "wb+");
        if (arquivoIndexador == NULL){
            exit(1);
        }
    }
    return arquivoIndexador;
}

FILE *criarRaizMS(char *nomeArquivo)
{
    char *nomeArquivoRaiz = concatenarStrings(nomeArquivo, "_raiz.o");
    printf("%s", nomeArquivoRaiz);

    FILE *arquivoRaiz = fopen(nomeArquivoRaiz, "rb+");
    if (arquivoRaiz == NULL)
    {
        arquivoRaiz = fopen(nomeArquivoRaiz, "wb+");
        if (arquivoRaiz == NULL){
            exit(1);
        }
    }
    return arquivoRaiz;
}

int recuperarRaiz(char *nomeArquivo){
  FILE * arquivoRaiz = criarRaizMS(nomeArquivo);
  int p;
  fread(&p, sizeof(int), 1, arquivoRaiz);
  fclose(arquivoRaiz);
  return p;
}

int escreverRaiz(char *nomeArquivo, int p){
  FILE * arquivoRaiz = criarRaizMS(nomeArquivo);
  fwrite(p, sizeof(int), 1, arquivoRaiz);
  fclose(arquivoRaiz);
  return p;
}

TABM * busca(char * nomeArquivo, int id){
  FILE * dados = criarDadosMS(nomeArquivo);
  FILE * indexador = criarIndexadorMS(nomeArquivo);

  fseek(dados, 0, SEEK_END);
  int estaVazioDados = ((int)ftell(dados)) <= 0;
  if(estaVazioDados){ // checa se o arquivo de dados  estiver vazio não existe o elemento na arvore
      return NULL;
  }


  fseek(indexador, 0, SEEK_END);
  int estaVazioIndexador = ((int)ftell(indexador)) <= 0;
  if(estaVazioIndexador){ //se o arquivo indexador está vazio procuramos em dados como se fosse uma lista normal
      fseek(dados, 0, SEEK_SET);
      TPizza * atual;
      int isOk = 0;
      while(isOk){
        if(atual->codigo == id){
          return atual;
        }
        isOk = fread(&atual,sizeof(TABM), 1, dados ) >= 1;
      }

      return NULL;
  }

  //descemos na arvore
  TABM * raiz;
  fread(&raiz,sizeof(TABM), 1, indexador );

  TABM * r = buscaRecursiva(indexador, dados, raiz ,id); // recupera o nó no indexador
  fclose(indexador);
  fclose(dados);
  return r;
}

TABM * buscaRecursiva(FILE * indexador, FILE * dados, TABM * atual, int id){
  int i = 0;
  while((i < atual->nchaves) && (id > atual->codigo[i]))i++;

  if ((i < atual->nchaves) && (atual->folha) && (id == atual->codigo[i])){  
    return atual;
  } 
  if(atual->folha) return NULL;
  if(atual->codigo[i] == id) i++;

  return buscaRecursiva(indexador, dados, atual->filho[i], id);
}
