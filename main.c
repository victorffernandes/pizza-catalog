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

    insere("arvorebinariamais", 14, "Pizza Calabreza", "Pizza Especial Boa demais","Especial", 40.2, 2);
    insere("arvorebinariamais", 3, "Pizza Robson", "Pizza Foda","Foda", 50.2, 2);
//    insere("arvorebinariamais", 5, "Pizza Robson", "Pizza Foda","Foda", 50.2, 2);
    FILE * fp = criarIndexadorMS("arvorebinariamais");
    int raiz_int = recuperarRaiz("arvorebinariamais");
    TABM * raiz = acharNo(fp,0 , 2);
    print(fp, raiz, 2);
    fclose(fp);


    /* printf("Sistema gerenciador da Pizzaria\n");
    printf("Escolha:\n");
    printf("1 para adicionar uma nova pizza");
    printf("2 para retirar uma pizza");
    printf("3 para busca pizzas por categoria");
    printf("-9 para encerrar");

    int opcao;
    TABM *ind;
    scanf("%d", &opcao);
    switch (opcao)
    {
    case 1:
        insere("arvorebinariamais", 12, "Pizza Calabreza", "Pizza Especial Boa demais", 40.2, 2);
    break;

    case 2:
        retiraPizza(ind, "indexador","dados_iniciais.dat", 5);
    break;

    case 3:
        buscaCategoria("Especial","dados_iniciais.dat");
    break;

    case -9:
        exit(-1);

    default:
        printf("Opção inválida, desculpe!");
        break;
    }

    return 0; */

}
