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
    insere("arvorebinariamais", 55, "Pizza Calabreza", "Pizza Especial Boa demais","lalala", 40.2, 2);
    insere("arvorebinariamais", 24, "Pizza 1", "Foda", "Foda", 50.2, 2);
    insere("arvorebinariamais", 12, "Pizza 2", "Foda","Foda", 50.2, 2);
    insere("arvorebinariamais", 95, "Pizza 3", "Foda","Foda", 50.2, 2);
    insere("arvorebinariamais", 52, "Pizza Calabreza", "Especial Boa demais","lalala", 40.2, 2);
//    insere("arvorebinariamais", 26, "Pizza Robson", "Foda", "Foda", 50.2, 2);
//    insere("arvorebinariamais", 19, "Pizza Robson", "Foda","Foda", 50.2, 2);
//    insere("arvorebinariamais", 85, "Pizza Robson", "Foda","Foda", 50.2, 2);
//    insere("arvorebinariamais", 53, "Pizza Calabreza", "Pizza Especial Boa demais","lalala", 40.2, 2);
//    insere("arvorebinariamais", 27, "Pizza Robson", "Pizza Foda", "Foda", 50.2, 2);
//    insere("arvorebinariamais", 20, "Pizza Robson", "Pizza Foda","Foda", 50.2, 2);
//    insere("arvorebinariamais", 185, "Pizza Robson", "Pizza Foda","Foda", 50.2, 2);
//    insere("arvorebinariamais", 28, "Pizza Robson", "Pizza Foda", "Foda", 50.2, 2);
//    insere("arvorebinariamais", 33, "Pizza Robson", "Pizza Foda","Foda", 50.2, 2);
//    insere("arvorebinariamais", 195, "Pizza Robson", "Pizza Foda","Foda", 50.2, 2);

    int opcao = 1, codigo;
    char nome[50], categoria[20], descricao[50];
    nome[0] = '\0';
    descricao[0] = '\0';
    categoria[0] = '\0';
    float preco;
    int t = 2;
    printf("Insira seu t para a arvore b+ que sera usada no catalogo de pizzas ");
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
      printf("-9 para encerrar\n");

      TABM *ind;
      scanf("%d", &opcao);
      switch (opcao)
      {
      case 1:
          printf("Insira o codigo da pizza: ");
          scanf("%d", &codigo);
          printf("Insira o nome da pizza: ");
          scanf("%s", &nome);
          printf("Insira a categoria da pizza: ");
          scanf("%s", &categoria);
          printf("Insira a descricao da pizza: ");
          scanf("%s", &descricao);
          printf("Insira o preco da pizza: ");
          scanf("%f", &preco);
          insere("arvorebinariamais", codigo, nome, categoria, descricao, preco, t);
      break;

      /*case 2:
          printf("Digite o código da pizza que deseja retirar: ");
          scanf("%d", &codigo);
          retiraPizza(ind, "indexador","dados_iniciais.dat", codigo, 2);
      break;*/

      case 3:
          printf("Insira a categoria da pizza: ");
          scanf("%s", &categoria);
          buscaCategoria("arvorebinariamais", categoria);
      break;

      case 4:
          printf("Digite o código de uma pizza: ");
          scanf("%d", &codigo);
          buscaPizza("arvorebinariamais", codigo, t);
      break;
      case 5:
          printf("Digite o código da pizza a ser alterada: ");
          scanf("%d", &codigo);
          alteraPizza("dados_iniciais", "indexador", codigo);
      break;



      case -9:
          exit(-1);

      default:
          printf("Opção inválida, desculpe!");
          break;
      }
    }
    return 0;

}
