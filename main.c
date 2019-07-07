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
    insere("arvorebinariamais", 55, "Pizza Calabreza", "Pizza Especial Boa demais","lalala", 40.2, 2);
    insere("arvorebinariamais", 24, "Pizza Robson", "Pizza Foda", "Foda", 50.2, 2);
    insere("arvorebinariamais", 12, "Pizza Robson", "Pizza Foda","Foda", 50.2, 2);
    insere("arvorebinariamais", 95, "Pizza Robson", "Pizza Foda","Foda", 50.2, 2);
    insere("arvorebinariamais", 52, "Pizza Calabreza", "Pizza Especial Boa demais","lalala", 40.2, 2);
    insere("arvorebinariamais", 26, "Pizza Robson", "Pizza Foda", "Foda", 50.2, 2);
    insere("arvorebinariamais", 19, "Pizza Robson", "Pizza Foda","Foda", 50.2, 2);
    insere("arvorebinariamais", 85, "Pizza Robson", "Pizza Foda","Foda", 50.2, 2);
    insere("arvorebinariamais", 53, "Pizza Calabreza", "Pizza Especial Boa demais","lalala", 40.2, 2);
    insere("arvorebinariamais", 27, "Pizza Robson", "Pizza Foda", "Foda", 50.2, 2);
    insere("arvorebinariamais", 20, "Pizza Robson", "Pizza Foda","Foda", 50.2, 2);
    insere("arvorebinariamais", 185, "Pizza Robson", "Pizza Foda","Foda", 50.2, 2);
    insere("arvorebinariamais", 28, "Pizza Robson", "Pizza Foda", "Foda", 50.2, 2);
    insere("arvorebinariamais", 33, "Pizza Robson", "Pizza Foda","Foda", 50.2, 2);
    insere("arvorebinariamais", 195, "Pizza Robson", "Pizza Foda","Foda", 50.2, 2);
    FILE * fp = criarIndexadorMS("arvorebinariamais");
    int raiz_int = recuperarRaiz("arvorebinariamais");
    TABM * raiz = acharNo(fp,raiz_int , 2);
    printRecur(fp, raiz, 2);
    readAll(fp, 2);
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
