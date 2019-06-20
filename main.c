#include <stdio.h>
#include <stdlib.h>
#include "lib/pizzaMS.h"

int main()
{
    printf("Sistema gerenciador da Pizzaria\n");
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
        retiraPizza(ind, "dados_iniciais.dat");
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

    return 0;
    
}
