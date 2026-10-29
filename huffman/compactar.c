#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compactar.h"
#include "fila.h"
#include "arvore.h"

// --- auxiliares ---

unsigned char ligar_bit(unsigned char c, int i)
{
    unsigned char mask = 1 << i;
    return mask | c;
}

void gerar_codigos(No *raiz, char codigos[256][257], char caminho[], int profundidade)
{
    if (eh_folha(raiz)) {
        caminho[profundidade] = '\0';
        strcpy(codigos[pegar_byte(raiz)], caminho);
        return;
    }
    caminho[profundidade] = '0';
    gerar_codigos(raiz->esquerda, codigos, caminho, profundidade + 1);

    caminho[profundidade] = '1';
    gerar_codigos(raiz->direita, codigos, caminho, profundidade + 1);
}

// --- compactar ---

void compactar(char *entrada, char *saida)
{
    FILE *arquivo = fopen(entrada, "rb");
    if (arquivo == NULL) {
        printf("Nao foi possivel abrir o arquivo %s\n", entrada);
        return;
    }
    FILE *compactado = fopen(saida, "wb");

    int frequencia[256] = {0};
    int c = fgetc(arquivo);
    while (c != EOF) {
        frequencia[c]++;
        c = fgetc(arquivo);
    }

    Fila *fila = criar_fila(comparar_frequencia);
    for (int i = 0; i < 256; i++) {
        if (frequencia[i] > 0) {
            inserir_ordenado(fila, criar_no(i, frequencia[i], NULL, NULL));
        }
    }

    // arquivo vazio
    if (fila->tamanho == 0) {
        fputc(0, compactado);
        fputc(0, compactado);
        fclose(arquivo);
        fclose(compactado);
        free(fila);
        return;
    }

    No *raiz = montar_arvore(fila);

    char codigos[256][257];
    char caminho[257];
    gerar_codigos(raiz, codigos, caminho, 0);

    long long total_bits = 0;
    for (int i = 0; i < 256; i++) {
        if (frequencia[i] > 0) {
            total_bits += frequencia[i] * strlen(codigos[i]);
        }
    }


    int lixo = 8 - (total_bits % 8);
    if (lixo == 8) { /// nao tem lixo
        lixo = 0;
    }

    // cabecalho de 16 bits: 3 bits de lixo + 13 bits do tamanho da arvore
    int tam_arvore = tamanho_arvore(raiz);
    int cabecalho = (lixo << 13) | tam_arvore;
    fputc(cabecalho >> 8, compactado);  // 8 bits de cima
    fputc(cabecalho & 255, compactado); // 8 bits de baixo

    escrever_arvore(raiz, compactado);

    rewind(arquivo);
    unsigned char byte = 0;
    int posicao = 7; // 7 6 5 4 3 2 1 0 -> 8 Bits

    c = fgetc(arquivo);
    while (c != EOF) {
        for (int i = 0; codigos[c][i] != '\0'; i++) {
            if (codigos[c][i] == '1') {
                byte = ligar_bit(byte, posicao);
            }
            posicao--;

            if (posicao < 0) { // byte cheio
                fputc(byte, compactado);
                byte = 0;
                posicao = 7;
            }
        }
        c = fgetc(arquivo);
    }
    if (posicao < 7) { 
        fputc(byte, compactado);
    }

    liberar_arvore(raiz);
    free(fila);
    fclose(arquivo);
    fclose(compactado);
}
