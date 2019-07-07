#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pizzaMS.h"
#include <limits.h>
#define tam 1000

int tamanho_pizza_bytes()
{
  return sizeof(int) +       // cod
         sizeof(char) * 50 + // nome
         sizeof(char) * 20 + // categoria
         sizeof(char) * 50 + // descricao
         sizeof(float);      // preço
}

TPizza *pizza(int cod, char *nome, char* descricao, char * categoria, float preco)
{
  TPizza *p = (TPizza *)malloc(tamanho_pizza_bytes());
  if (p)
    memset(p, 0, tamanho_pizza_bytes());
  p->codigo = cod;
  strcpy(p->nome, nome);
  strcpy(p->descricao, descricao);
  p->preco = preco;
  return p;
}

int tamanho_arvbm_bytes(int t)
{
  return sizeof(int) * 3 +             // nchaves proximo folha
         sizeof(int) * ((t * 2) - 1) + // codigo
         sizeof(int) * ((t * 2) - 1) + //pizza
         sizeof(int) * ((t * 2));      //filho
}

void freePizza(TPizza * pizza)
{
  free(pizza);
}

void freeNo(TABM * no, int t)
{
  free(no->codigo);
  free(no->pizza);
  free(no->filho);
  free(no);
}

TABM* cria(int t)
{
  TABM * novo = (TABM *)malloc(sizeof(TABM));
  novo->nchaves = 0;
  novo->prox = -1;
  novo->folha = 1;
  novo->codigo = malloc(sizeof(int) * ((t * 2) - 1) + 1);
  novo->pizza = malloc(sizeof(int ) * ((t * 2) - 1) + 1);
  novo->filho = malloc(sizeof(int ) * t * 2);// filho é só 2t
  return novo;
}

TPizza * acharPizza(FILE *arquivoDados, int i){
    fseek(arquivoDados, i, SEEK_SET);
    TPizza * pizza = (TPizza *) malloc(tamanho_pizza_bytes());
    fread(pizza, tamanho_pizza_bytes(), 1, arquivoDados);
    return pizza;
}

TABM* acharNo(FILE *arquivoIndexador, int i, int t)
{
  fseek(arquivoIndexador, i, SEEK_SET);
  TABM* index = cria(t);
  fread(&index->folha, sizeof(int), 1, arquivoIndexador);
  fread(&index->nchaves, sizeof(int), 1, arquivoIndexador);
  fread(&index->prox, sizeof(int), 1, arquivoIndexador);

  for(int j = 0; j < ((t * 2) - 1); j++ ) {
    fread(&index->codigo[j], sizeof(int), 1,  arquivoIndexador);
    fread(&index->pizza[j], sizeof(int), 1,  arquivoIndexador);
  }

  for(int j = 0; j < ((t * 2)); j++ ) {
    fread(&index->filho[j], sizeof(int), 1,  arquivoIndexador);
  }

  return index;
}

int inserePizza(FILE * arquivoDados, TPizza * pizza){
    fseek(arquivoDados, 0, SEEK_END);
    int posDados = ftell(arquivoDados);
    fwrite(pizza, tamanho_pizza_bytes(), 1, arquivoDados);
    return posDados;
}

int insereNo(FILE * arquivoIndexador, TABM * index, int t){
    fseek(arquivoIndexador, 0, SEEK_END);
    int pos = ftell(arquivoIndexador);
    return alteraNo(arquivoIndexador, index, pos, t);
}

int alteraNo(FILE * arquivoIndexador, TABM * index,int i, int t){
  fseek(arquivoIndexador, i, SEEK_SET);
  int posInicial = ftell(arquivoIndexador);
  fwrite(&index->folha, sizeof(int), 1, arquivoIndexador);
  fwrite(&index->nchaves, sizeof(int), 1, arquivoIndexador);
  fwrite(&index->prox, sizeof(int), 1, arquivoIndexador);
  for(int j = 0; j < ((t * 2) - 1); j++ ){
    fwrite(&index->codigo[j], sizeof(int), 1,  arquivoIndexador);
    fwrite(&index->pizza[j], sizeof(int), 1,  arquivoIndexador);
  }

  for(int j = 0; j < ((t * 2)); j++ ){
    fwrite(&index->filho[j], sizeof(int), 1,  arquivoIndexador);
  }
  return posInicial;
}

int isEmpty(FILE *file)
{
  fseek(file, 0, SEEK_END);
  int estaVazioDados = (int)ftell(file) <= 0;
  if (estaVazioDados)
  {
    fseek(file, 0, SEEK_SET);
    return 1;
  }
  return 0;
}

int lerPosicao(FILE *file)
{
  fseek(file, 0, SEEK_END);
  return ftell(file);
}

void readAll(FILE * indexador,int t){
    int i = 0;
    fseek(indexador, 0, SEEK_END); int fim = ftell(indexador); fseek(indexador,0,SEEK_SET);

    TABM * raiz = cria(t);
    while(i < fim){
    raiz = acharNo(indexador, i, t);
    if(raiz->folha){
        printf("FOLHA ");
    }
    else{
        printf("NO INTERNO ");
    }
        printf("Index: %d ", i);
        printf("Codigos: ");
        for(int j = 0; j < raiz->nchaves; j++){
            printf("%d ", raiz->codigo[j]);
        }
        printf("Filhos: ");
        for(int j = 0; !raiz->folha && j < raiz->nchaves + 1; j++){
            printf("%d ", raiz->filho[j]);
        }

        printf("\n");
        i+=52;
    }
    printf("\n \n");
}


int *insere(char * nomeArquivo, int cod, char * nome, char * categoria,char * descricao, float preco, int t){
  TABM * procura = busca(nomeArquivo, cod, t);
  if(procura) return procura;
  FILE *indexador = criarIndexadorMS(nomeArquivo);
  FILE *dados = criarDadosMS(nomeArquivo);
  TPizza *p = pizza(cod, nome, descricao,categoria, preco);


  if (isEmpty(dados))
  {
    TABM *raiz = cria(t);
    raiz->codigo[0] = cod;
    raiz->pizza[0] = inserePizza(dados, p);
    raiz->nchaves = 1;
    int r = alteraNo(indexador,raiz, 0, t);

    fclose(indexador);
    fclose(dados);
    freeNo(raiz, t);
    freePizza(p);

    return escreverRaiz(nomeArquivo, r);
  }

  int raiz_int = recuperarRaiz(nomeArquivo);
  TABM *raiz = acharNo(indexador,raiz_int , t); // achar raiz

if (raiz->nchaves == (2 * t) - 1)
  {
    TABM *S = cria(t);
    S->nchaves = 0;
    S->folha = 0;
    S->filho[0] = raiz_int;
    int s_i = insereNo(indexador,S,t);
    s_i = divisao(indexador, dados, s_i, 1, raiz_int, t);
    readAll(indexador,t);
    s_i = insere_nao_completo(indexador, dados, s_i, p, t);
    readAll(indexador,t);
    freeNo(S, t);
    freeNo(raiz, t);
    return escreverRaiz(nomeArquivo, s_i);
  }
  raiz_int = insere_nao_completo(indexador, dados, raiz_int, p, t);
  escreverRaiz(nomeArquivo, raiz_int);
  fclose(indexador);
  fclose(dados);
  freeNo(raiz, t);
  freePizza(p);

  return raiz_int;
}

int divisao(FILE* indexador, FILE * dados, int x_i, int i, int y_i, int t){
  TABM * x = acharNo(indexador, x_i, t);
  TABM * y = acharNo(indexador, y_i, t);
  TABM *z = cria(t);
  int z_i = insereNo(indexador,z,t);
  z->folha = y->folha;
  int j;
  if(!y->folha){
    z->nchaves = t-1;
    for(j=0;j<t-1;j++){
     z->codigo[j] = y->codigo[j+t];
     z->pizza[j] = y->pizza[j+t];
    }
    for(j=0;j<t;j++){
      z->filho[j] = y->filho[j+t];
      y->filho[j+t] = NULL;
    }
  }
  else {
    z->nchaves = t; //z possuir� uma chave a mais que y se for folha
    for(j=0;j < t;j++){
        z->codigo[j] = y->codigo[j+t-1];//Caso em que y � folha, temos q passar a info para o n� da direita
        z->pizza[j] = y->pizza[j+t-1];
    }
    y->prox = z_i;
  }
  y->nchaves = t-1;
  for(j=x->nchaves; j>=i; j--) x->filho[j+1]=x->filho[j];

  x->filho[i] = z_i;
  for(j=x->nchaves; j>=i; j--){
    x->codigo[j] = x->codigo[j-1];
    x->pizza[j] = x->pizza[j-1];
  }
  x->codigo[i-1] = y->codigo[t-1];
  x->pizza[i-1] = y->pizza[t-1];
  x->nchaves++;

  alteraNo(indexador,z,z_i, t);
  alteraNo(indexador,x, x_i, t);
  alteraNo(indexador,y, y_i, t);
  freeNo(z,t);
  freeNo(y,t);
  freeNo(x,t);
  return x_i;
}

//insere no nó que não está completo
int insere_nao_completo(FILE *arquivoIndexador, FILE *arquivoDados, int x_i, TPizza *pizza, int t)
{
  TABM* x = acharNo(arquivoIndexador, x_i, t);
  int i = x->nchaves - 1;
  if (x->folha)
  {
    while ((i >= 0) && (pizza->codigo < x->codigo[i]))
    {
      x->codigo[i + 1] = x->codigo[i];
      x->pizza[i + 1] = x->pizza[i];
      i--;
    }
    x->codigo[i + 1] = pizza->codigo;
    x->pizza[i + 1] = inserePizza(arquivoDados, pizza); // adiciona a pizza no arquivo dados e retorna sua posição no mesmo
    x->nchaves++;
    return alteraNo(arquivoIndexador, x, x_i, t);
  }
  while ((i >= 0) && (pizza->codigo < x->codigo[i]))
    i--;
  i++;
  TABM * filho = acharNo(arquivoIndexador, x->filho[i], t);
  if (filho->nchaves == ((2 * t) - 1))
  {
    x_i = divisao(arquivoIndexador, arquivoDados, x_i, i + 1, x->filho[i], t);
    x = acharNo(arquivoIndexador, x_i, t); // só pra atualizar o x
    if (pizza->codigo > x->codigo[i])
      i++;
  }
  x = acharNo(arquivoIndexador, x_i, t); // só pra atualizar o x
  x->filho[i] = insere_nao_completo(arquivoIndexador, arquivoDados,  x->filho[i], pizza, t);
  int endereco = alteraNo(arquivoIndexador, x, x_i, t);
  freeNo(filho,t);
  freeNo(x,t);
  return endereco;
}

char *concatenarStrings(char *sufixo, char *prefixo)
{
  int tamanhoByteString = strlen(sufixo) + strlen(prefixo) + 1;
  char *nomeConcatenado = (char *)malloc(sizeof(char) * tamanhoByteString);
  nomeConcatenado[0] = '\0';
  strcat(nomeConcatenado, sufixo);
  strcat(nomeConcatenado, prefixo);
  return nomeConcatenado;
}

FILE *criarDadosMS(char *nomeArquivo)
{
  char *nomeArquivoDados = concatenarStrings(nomeArquivo, "_dados.o");

  FILE * arquivoDados = fopen(nomeArquivoDados, "rb+");
  int boolean = arquivoDados == NULL;
  if (boolean)
  {
    arquivoDados = fopen(nomeArquivoDados, "wb+");
    if (arquivoDados == NULL)
    {
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
    if (arquivoIndexador == NULL)
    {
      exit(1);
    }
  }
  return arquivoIndexador;
}

FILE *criarRaizMS(char *nomeArquivo)
{
  char *nomeArquivoRaiz = concatenarStrings(nomeArquivo, "_raiz.o");

  FILE *arquivoRaiz = fopen(nomeArquivoRaiz, "rb+");
  if (arquivoRaiz == NULL)
  {
    arquivoRaiz = fopen(nomeArquivoRaiz, "wb+");
    if (arquivoRaiz == NULL)
    {
      exit(1);
    }
  }
  return arquivoRaiz;
}

int recuperarRaiz(char *nomeArquivo)
{
  FILE *arquivoRaiz = criarRaizMS(nomeArquivo);
  int p;
  fread(&p, sizeof(int), 1, arquivoRaiz);
  fclose(arquivoRaiz);
  return p;
}

int escreverRaiz(char *nomeArquivo, int p)
{
  FILE *arquivoRaiz = criarRaizMS(nomeArquivo);
  int * valor = (int *) malloc(sizeof(int));
  valor = &p;
  fwrite(valor, sizeof(int), 1, arquivoRaiz);
  fclose(arquivoRaiz);
  return *valor;
}

TABM * busca(char *nomeArquivo, int id, int t)
{
  FILE *dados = criarDadosMS(nomeArquivo);
  FILE *indexador = criarIndexadorMS(nomeArquivo);
  int raiz_i = recuperarRaiz(nomeArquivo);

  fseek(dados, 0, SEEK_END);
  int estaVazioDados = ((int)ftell(dados)) <= 0;
  if (estaVazioDados)
  { // checa se o arquivo de dados  estiver vazio não existe o elemento na arvore
    return NULL;
  }


  //descemos na arvore
  TABM *raiz = acharNo(indexador, raiz_i,t);
    readAll(indexador, t);
  TABM *r = buscaRecursiva(indexador, dados, raiz, id, t); // recupera o nó no indexador

  fclose(indexador);
  fclose(dados);
  return r;
}

TABM * buscaRecursiva(FILE * indexador, FILE * dados, TABM * atual, int id, int t){

  if(!atual) return atual;
  int i = 0;
  while ((i < atual->nchaves) && (id > atual->codigo[i]))
    i++;

  if ((i < atual->nchaves) && (atual->folha) && (id == atual->codigo[i]))
  {
    return atual;
  }
  if (atual->folha)
    return NULL;
  if (atual->codigo[i] == id)
    i++;

  return buscaRecursiva(indexador, dados, acharNo(indexador, atual->filho[i], t), id, t);
}

TPizza *buscaCategoria(char *categoria, char *dados)
{
  FILE *ent = fopen(dados, "rb");
  if (!dados)
    exit(-1);
  TPizza *pizza = NULL;
  TPizza *pizzaVet = (TPizza *)malloc(sizeof(TPizza) * tam);
  int i = 0;
  int check;
  while (i < tam && (ent))
  {
    check = fread(&pizza, tamanho_pizza_bytes(), 1, ent);
    if (check)
    {
      int resp = strcmp(categoria, pizza->categoria);
      if (resp == 0)
      {
        pizzaVet[i] = *pizza;
      }
      i++;
    }
    fseek(ent, tamanho_pizza_bytes(), SEEK_CUR);
  }
  fclose(ent);
  return pizzaVet;
}


/* void retiraPizza(TABM *ind, char * dados, char * indexador, int codigo){
  if(!ind) exit(-1);
  TABM * aux = buscaRecursiva(ind, dados, indexador, codigo);
  if(aux){
    FILE * ent = fopen(indexador, "rb");
    if(!ent) exit(-1);

  }
} */

void alteraPizza(char * dados, char * indexador, int codigo){
  FILE * ent = fopen(indexador, "rb");
  if(!ent) exit(-1);
  int ind = 0;
  int resp = 1;
  while(resp){
    resp = fread(ind, sizeof(int), 1, ent);
    if(ind == codigo){
      FILE * opn = fopen(dados, "rb+");
      if(!opn) exit(-1);

    }
  }
}
