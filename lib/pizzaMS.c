#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pizzaMS.h"
#include <limits.h>

int tamanho_pizza_bytes()
{
  return sizeof(int) +       // cod
         sizeof(char) * 50 + // nome
         sizeof(char) * 20 + // categoria
         sizeof(char) * 50 + // descricao
         sizeof(float);      // preço
}

TPizza *pizza(int cod, char *nome, char *descricao, char *categoria, float preco)
{
  TPizza *p = (TPizza *)malloc(tamanho_pizza_bytes());
  if (p)
    memset(p, 0, tamanho_pizza_bytes());
  p->codigo = cod;
  strcpy(p->nome, nome);
  strcpy(p->categoria, categoria);
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

void freePizza(TPizza *pizza)
{
  free(pizza);
}

void freeNo(TABM *no, int t)
{
  free(no->codigo);
  free(no->pizza);
  free(no->filho);
  free(no);
}

TABM *cria(int t)
{
  TABM *novo = (TABM *)malloc(sizeof(TABM));
  novo->nchaves = 0;
  novo->prox = -1;
  novo->folha = 1;
  novo->codigo = malloc(sizeof(int) * ((t * 2) - 1) + 1);
  novo->pizza = malloc(sizeof(int) * ((t * 2) - 1) + 1);
  novo->filho = malloc(sizeof(int) * t * 2); // filho é só 2t
  return novo;
}

TPizza *acharPizza(FILE *arquivoDados, int i)
{
  fseek(arquivoDados, i, SEEK_SET);
  TPizza *pizza = (TPizza *)malloc(tamanho_pizza_bytes());
  fread(pizza, tamanho_pizza_bytes(), 1, arquivoDados);
  return pizza;
}

TABM *acharNo(FILE *arquivoIndexador, int i, int t)
{
  fseek(arquivoIndexador, i, SEEK_SET);
  TABM *index = cria(t);
  fread(&index->folha, sizeof(int), 1, arquivoIndexador);
  fread(&index->nchaves, sizeof(int), 1, arquivoIndexador);
  fread(&index->prox, sizeof(int), 1, arquivoIndexador);

  for (int j = 0; j < ((t * 2) - 1); j++)
  {
    fread(&index->codigo[j], sizeof(int), 1, arquivoIndexador);
    fread(&index->pizza[j], sizeof(int), 1, arquivoIndexador);
  }

  for (int j = 0; j < ((t * 2)); j++)
  {
    fread(&index->filho[j], sizeof(int), 1, arquivoIndexador);
  }

  return index;
}

int alterarPizza(FILE *arquivoDados,int x_i, TPizza *pizza)
{
  fseek(arquivoDados, x_i, SEEK_SET);
  int posDados = ftell(arquivoDados);
  fwrite(pizza, tamanho_pizza_bytes(), 1, arquivoDados);
  return posDados;
}

int inserePizza(FILE *arquivoDados, TPizza *pizza)
{
  fseek(arquivoDados, 0, SEEK_END);
  int posDados = ftell(arquivoDados);
  fwrite(pizza, tamanho_pizza_bytes(), 1, arquivoDados);
  return posDados;
}

int insereNo(FILE *arquivoIndexador, TABM *index, int t)
{
  fseek(arquivoIndexador, 0, SEEK_END);
  int pos = ftell(arquivoIndexador);
  return alteraNo(arquivoIndexador, index, pos, t);
}

int alteraNo(FILE *arquivoIndexador, TABM *index, int i, int t)
{
  fseek(arquivoIndexador, i, SEEK_SET);
  int posInicial = ftell(arquivoIndexador);
  fwrite(&index->folha, sizeof(int), 1, arquivoIndexador);
  fwrite(&index->nchaves, sizeof(int), 1, arquivoIndexador);
  fwrite(&index->prox, sizeof(int), 1, arquivoIndexador);
  for (int j = 0; j < ((t * 2) - 1); j++)
  {
    fwrite(&index->codigo[j], sizeof(int), 1, arquivoIndexador);
    fwrite(&index->pizza[j], sizeof(int), 1, arquivoIndexador);
  }

  for (int j = 0; j < ((t * 2)); j++)
  {
    fwrite(&index->filho[j], sizeof(int), 1, arquivoIndexador);
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
    int r = alteraNo(indexador, raiz, 0, t);

    fclose(indexador);
    fclose(dados);
    freeNo(raiz, t);
    freePizza(p);

    return escreverRaiz(nomeArquivo, r);
  }

  int raiz_int = recuperarRaiz(nomeArquivo);
  TABM *raiz = acharNo(indexador, raiz_int, t); // achar raiz

  if (raiz->nchaves == (2 * t) - 1)
  {
    TABM *S = cria(t);
    S->nchaves = 0;
    S->folha = 0;
    S->filho[0] = raiz_int;
    int s_i = insereNo(indexador, S, t);
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
  if (!y->folha)
  {
    z->nchaves = t - 1;
    for (j = 0; j < t - 1; j++)
    {
      z->codigo[j] = y->codigo[j + t];
      z->pizza[j] = y->pizza[j + t];
    }
    for (j = 0; j < t; j++)
    {
      z->filho[j] = y->filho[j + t];
      y->filho[j + t] = NULL;
    }
  }
  else
  {
    z->nchaves = t; //z possuir� uma chave a mais que y se for folha
    for (j = 0; j < t; j++)
    {
      z->codigo[j] = y->codigo[j + t - 1]; //Caso em que y � folha, temos q passar a info para o n� da direita
      z->pizza[j] = y->pizza[j + t - 1];
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
  x->codigo[i - 1] = y->codigo[t - 1];
  x->pizza[i - 1] = y->pizza[t - 1];
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
  TABM *x = acharNo(arquivoIndexador, x_i, t);
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
  freeNo(filho, t);
  freeNo(x, t);
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
  int *valor = (int *)malloc(sizeof(int));
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

int tamanhoArquivo(FILE * arquivo){
    fseek(arquivo, 0, SEEK_END);
    int t = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);
    return t;
}

TPizza * buscaCategoria( char *nomeArquivo, char *categoria)
{
  FILE *arquivoDados = criarDadosMS(nomeArquivo);

  int tam = tamanhoArquivo(arquivoDados);

  TPizza *pizza = (TPizza *) malloc(tamanho_pizza_bytes());
  TPizza *pizzaVet = (TPizza *)malloc(tam);
  int i = 0, check;

  while (i < 1 + (tam / tamanho_pizza_bytes()) && (arquivoDados))
  {
    check = fread(pizza, tamanho_pizza_bytes(), 1, arquivoDados);
    if (check)
    {
      int resp = strcmp(categoria, pizza->categoria);
      if (resp == 0)
      {
        pizzaVet[i] = *pizza;
        printf("Categoria: %s | Nome: %s \n", pizza->categoria, pizza->nome);
      }
    }
    i++;
  }
  i++;
  fclose(arquivoDados);
  return pizzaVet;
}

/*void retiraPizza(TABM *ind, char *dados, char *indexador, int codigo, int t)
{
  if (!ind)
  {
    printf("Ocorreu um erro\n");
    exit(-1);
  }
  TABM *aux = buscaRecursiva(ind, dados, indexador, codigo, t);
  if (aux)
  {
    FILE *ent = fopen(indexador, "rb");
    if (!ent){
      printf("Ocorreu um erro!\n");
      exit(-1);
      }
    int i;
    for (i = 0; i < (2 * t) - 1; i++)
    {
      if (ind->codigo[i] == codigo)
      {
        if (ind->nchaves == t - 1)
        {
        }
      }
    }
  }
}*/


void alteraPizza(char *dados,  int codigo){
  FILE *ent = criarIndexadorMS(dados);
  fseek(ent, 0L, SEEK_END);
  int fileSize = ftell(ent), i = 0;
  rewind(ent);
  int ind = 0, resp = 1, verify = 0;
  while((resp) && (i < fileSize) && !feof(ent)){
    resp = fread(&ind, sizeof(int), 1, ent);
    if(ind == codigo){
      fseek(ent, -1L, SEEK_CUR);
      ent = criarDadosMS(dados);
      TPizza aux, pizza;
      resp = fread(&pizza, tamanho_pizza_bytes(), 1, ent);
      printf("O nome da pizza que voce deseja remover eh: %s\n", pizza.nome);
      printf("Para alterar o nome da pizza, escreva um novo nome\n");
      scanf("%s", aux.nome);
      fwrite(aux.nome, sizeof(char)*50, 1, ent);
      printf("A categoria da pizza que voce deseja alterar eh: %s\n", pizza.categoria);
      printf("Para trocar a categoria, escreva uma nova\n");
      scanf("%s", aux.categoria);
      fwrite(aux.categoria, sizeof(char)*20, 1, ent);
      printf("Para alterar a descricao da pizza, escreva uma nova: \n");
      scanf("%s", aux.descricao);
      fwrite(aux.descricao, sizeof(char)*50, 1, ent);
      verify = 1;
    }
    i++;
  }
  if(verify != 1) printf("O código informado eh invalido!\n");
  fclose(ent);
}


int remocao(char * nomeArquivo, int cod, int t){
    FILE * indexador = criarIndexadorMS(nomeArquivo);
    FILE * dados = criarDadosMS(nomeArquivo);
    readAll(indexador, t);
    int raiz_i = recuperarRaiz(nomeArquivo);

    TABM * raiz = acharNo(indexador, raiz_i, t);
    if(!raiz->folha){
        int i = 0;
        while(cod > raiz->codigo[i] && i <= raiz->nchaves) i++;
        int ls = i - 1, rs = i + 1;
        if(i == 0) ls = -1;// faz parte do raciocínio
        if(i == raiz->nchaves) rs = -1;
        int removido = remocaoRecur(indexador, dados, raiz->filho[i], raiz->filho[ls], raiz->filho[rs],cod, t);
    }
    else {
        remocaoRecur(indexador, dados, raiz_i, -1, -1, cod, t);
    }
}

int recuperarMaiorFilho(TABM * a){
    return a->filho[a->nchaves-1];
}

int recuperarMenorFilho(TABM * a){
    return a->filho[0];
}



int remocaoRecur(FILE * indexador, FILE * dados, int x, int ls, int rs, int cod, int t){
    TABM * atual = acharNo(indexador,x,t);
    TABM * irmaoDireita;
    TABM * irmaoEsquerda;
    int ls_atual, rs_atual;

    if(ls != -1) irmaoEsquerda = acharNo(indexador, ls,t);
    if(rs != -1) irmaoDireita = acharNo(indexador, rs, t);

    if(!atual->folha){
        int i = 0;
        while(cod > atual->codigo[i] && i <= atual->nchaves) i++; // recuperar o filho certo
        ls_atual = i - 1; rs_atual = i + 1;
        if(i == atual->nchaves) rs = -1;

        TABM * filho = acharNo(indexador, atual->filho[i], t);
        if(filho->folha){
            if(filho->nchaves >= t){// caso 1 remove normalmente
                remocaoRecur(indexador, dados, atual->filho[i], -1, -1, cod, t);
                filho = acharNo(indexador, atual->filho[i], t);
                if(i == atual->nchaves) i--;
                atual->codigo[i] = filho->codigo[0]; //atualizar o nó interno com o valor alterado na folha
                alteraNo(indexador, atual, x, t);
            }
            else if((ls != -1 && irmaoDireita->nchaves >= t) || (rs != -1 && irmaoEsquerda->nchaves >= t)){ // caso 3a
                printf("CASO 3A - NÃO IMPLEMENTADO");
            }
            else{// caso 3b
                printf("CASO 3B - NÃO IMPLEMENTADO");
            }
        }
        else{
            int removido = remocaoRecur(indexador, dados,atual->filho[i], atual->filho[ls_atual], atual->filho[rs_atual], cod, t);
        }
    }
    else{
        int p = 0;
        while(atual->codigo[p] != cod && p < atual->nchaves) p++;

        if(p >= atual->nchaves) return -1;

        TPizza * p2 = pizza(-1,"_", "_","_", 12);
        alterarPizza(dados, atual->pizza[p], p2);
        // só entra aqui se for folha
        for(int j = p; j < atual->nchaves - 1; j++){
            atual->codigo[j] = atual->codigo[j + 1];
            atual->pizza[j] = atual->pizza[j + 1];
        }
        atual->nchaves--;
        alteraNo(indexador, atual, x, t);
        freeNo(atual, t);
        return x;
    }
}


TPizza * buscaPizza(char *nomeArquivo, int codigo, int t){
  FILE *arquivoDados = criarDadosMS(nomeArquivo);
  TABM * no = busca(nomeArquivo, codigo, t);
  if(no){
      int i = 0;
      while(codigo != no->codigo[i] && i < no->nchaves) i++;

      TPizza * pizza = acharPizza(arquivoDados, no->pizza[i]);
      printf("Nome: %s | Descricao: %s | Categoria: %s | Preco: %.2f \n", pizza->nome, pizza->descricao, pizza->categoria, pizza->preco);
      fclose(arquivoDados);
      return pizza;
  }

  printf("Não existe pizza com esse código \n");
  fclose(arquivoDados);
  return NULL;
}

void removePizzasPorCategoria(char *dados, int t){
  FILE *ent = fopen(dados, "rb");
  int tam = tamanhoArquivo(ent);
  TPizza *vet = (TPizza *) malloc(tam);
  char categoria[20];
  printf("Digite a categoria que deseja remover: \n");
  scanf("%s", categoria);
  categoria[19] = '\0';
  vet = buscaCategoria(dados, categoria);
  int i = 0;
  while (i < tam)
  {
    remocao(dados, vet[i].codigo, t);
    i++;
  }
  free(vet);
  fclose(ent);
}

void imprimeCatalogo(char *dados, int t){
  FILE *ent = criarDadosMS(dados);
  if(!ent){
    printf("Ocorreu um erro\n");
    exit(-1);
  }
  TPizza pizza;
  int tam = tamanhoArquivo(ent), i = 0, check = 1;
  while((i < tam) && check){
    check = fread(&pizza, tamanho_pizza_bytes(), 1, ent);
    if(check){
      printf("%d, ", pizza.codigo);
      printf("%s ", pizza.nome);
      printf("(%s), ", pizza.categoria);
      printf("R$ %.2f", pizza.preco);
      printf("\n");
    }
    i++;
  }
}
