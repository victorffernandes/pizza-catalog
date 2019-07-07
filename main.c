#include <stdio.h>
#include <stdlib.h>
#include "lib/pizzaMS.h"

void print(FILE * indexador, TABM * r, int t){
    if(!r->folha){
        for(int i = 0; i < r->nchaves; i++){
            printf("%d ", r->codigo[i]);
            //print(indexador,acharNo(indexador, r->filho[i],t), t);
        }
    }else{
        for(int i = 0; i < r->nchaves; i++){
            printf("%d ", r->codigo[i]);
        }
    }
}


int main()
{

    /*insere("arvorebinariamais", 47, "Pizza Calabreza", "Pizza Especial Boa demais","lalala", 40.2, 2);
    insere("arvorebinariamais", 3, "Pizza Robson", "Pizza Foda", "Foda", 50.2, 2);
    insere("arvorebinariamais", 5, "Pizza Robson", "Pizza Foda","Foda", 50.2, 2);
    FILE * fp = criarIndexadorMS("arvorebinariamais");
    int raiz_int = recuperarRaiz("arvorebinariamais");
    TABM * raiz = acharNo(fp,0 , 2);
    print(fp, raiz, 2);
    fclose(fp);*/

    int opcao = 1, codigo;
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
          insere("arvorebinariamais", 12, "Pizza Calabreza", "Pizza Especial Boa demais", "hjskdkshkdhskahda", 20, 2);
      break;

      /*case 2:
          printf("Digite o código da pizza que deseja retirar: ");
          scanf("%d", &codigo);
          retiraPizza(ind, "indexador","dados_iniciais.dat", codigo, 2);
      break;*/

      case 3:
          buscaCategoria("Especial","dados_iniciais.dat");
      break;

      case 4:
          printf("Digite o código de uma pizza: ");
          scanf("%d", &codigo);
          buscaPizza("dados_iniciais", codigo);
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
