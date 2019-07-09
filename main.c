#include <stdio.h>
#include <stdlib.h>
#include "lib/pizzaMS.h"


void printRecur(FILE * indexador, TABM * r, int t){
    if(!r->folha){
        for(int i = 0; i < r->nchaves; i++){
            printf("No Interno: %d \n", r->codigo[i]);
        }

        for(int i = 0; i < r->nchaves + 1; i++){
            printRecur(indexador,acharNo(indexador, r->filho[i],t), t);
        }
    }else{
        for(int i = 0; i < r->nchaves; i++){
            printf("Folha: %d \n", r->codigo[i]);
        }
    }
}


int main()
{
    int opcao = 1, codigo;
    char nome[50], categoria[20], descricao[50];
    char nomeLoja[60];
    float preco;
    int t = 2;
    printf("Insira o nome da sua loja: ");
    scanf("%[^\n]%*c", nomeLoja);

    printf("Insira seu t para a arvore b+ que sera usada no catalogo de pizzas: ");
      scanf("%d", &t);
      if(t < 2) t = 2;

    while(opcao != -9){
      printf("Sistema gerenciador da Pizzaria\n");
      printf("Escolha:\n");
      printf("1 para adicionar uma nova pizza\n");
      printf("2 para retirar uma pizza\n");
      printf("3 para buscar pizzas por categoria\n");
      printf("4 para buscar uma pizza\n");
      printf("5 para alterar uma pizza\n");
			printf("6 para ver o catalogo completo\n");
      printf("-9 para encerrar\n");

      TABM *ind;
      scanf("%d", &opcao);
      switch (opcao)
      {
      case 1:
          printf("Insira o codigo da pizza: ");
          scanf("%d", &codigo);
          while (getchar() != '\n')
            continue;
          printf("Insira o nome da pizza: ");
          scanf(" %c", nome);
          while (getchar() != '\n')
            continue;
          printf("Insira a categoria da pizza: ");
          scanf(" %c", categoria);
           while (getchar() != '\n')
            continue;
          printf("Insira a descricao da pizza: ");
          scanf(" %c", descricao);
          while (getchar() != '\n')
            continue;
          printf("Insira o preco da pizza: ");
          scanf("%f", &preco);
          insere(nomeLoja, codigo, nome, categoria, descricao, preco, t);
      break;

      case 2:
          printf("Digite o código da pizza que deseja retirar: ");
          scanf("%d", &codigo);
          remocao(nomeLoja, codigo, t);
      break;

      case 3:
          printf("Insira a categoria da pizza: ");
          scanf("%[^\n]%*c", categoria);
          buscaCategoria(nomeLoja, categoria);
      break;

      case 4:
          printf("Digite o codigo de uma pizza: ");
          scanf("%d", &codigo);
          buscaPizza(nomeLoja, codigo, t);
      break;
      case 5:
          printf("Digite o codigo da pizza a ser alterada: ");
          scanf("%d", &codigo);
          alteraPizza(nomeLoja, codigo);
      break;

      case 6:
          printf("\n \n");
          imprimeCatalogo(nomeLoja, t);
          printf("\n \n");
      break;
      case 7:
        removePizzasPorCategoria(nomeLoja, t);
      break;


      case -9:
          printf("Obrigada por utilizar o nosso sistema!\nBye, bye\n");
          printf("Saindo...\n");
          exit(-1);

      default:
          printf("Opção inválida, desculpe!");
          break;
      }
    }
    return 0;

}
