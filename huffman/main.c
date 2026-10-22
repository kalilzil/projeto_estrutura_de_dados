#include <stdio.h>
#include "compactar.h"
#include "descompactar.h"

int main()
{
    int opcao = 0;
    char entrada[256], saida[256];

    while (opcao != 3) {
        printf("\n1 - Compactar\n");
        printf("2 - Descompactar\n");
        printf("3 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1 || opcao == 2) {
            printf("Arquivo de entrada: ");
            scanf("%s", entrada);
            printf("Arquivo de saida: ");
            scanf("%s", saida);

            if (opcao == 1)
                compactar(entrada, saida);
            else
                descompactar(entrada, saida);

            printf("Pronto!\n");
        }
    }

    return 0;
}
