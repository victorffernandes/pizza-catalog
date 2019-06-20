#include <stdio.h>
#include <stdlib.h>
#include "lib/pizzaMS.h"

int main()
{
    insere("arvorebinariamais", 12, "Pizza Calabreza", "Pizza Especial Boa demais", 40.2, 2);
    buscaCategoria("Especial","dados_iniciais.dat");
}
