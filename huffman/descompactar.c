#include <stdio.h>
#include <stdlib.h>
#include "descompactar.h"
#include "fila.h"
#include "arvore.h"

// --- auxiliares ---

// ve se o bit i do byte c ta ligado
int bit_ligado(unsigned char c, int i)
{
    unsigned char mask = 1 << i;
    return mask & c;
}

// --- descompactar ---

void descompactar(char *entrada, char *saida)
{
    FILE *compactado = fopen(entrada, "rb");
    if (compactado == NULL) {
        printf("Nao foi possivel abrir o arquivo %s\n", entrada);
        return;
    }
    // abre o arquivo de saida pra escrever em binario
    FILE *arquivo = fopen(saida, "wb");

    // conta quantos bytes o arquivo compactado tem
    int tamanho_arquivo = 0;
    while (fgetc(compactado) != EOF) {
        tamanho_arquivo++;
    }
    rewind(compactado); // volta pro inicio pra ler de novo

    // le o cabecalho
    unsigned char byte1 = fgetc(compactado);
    unsigned char byte2 = fgetc(compactado);
    int cabecalho = (byte1 << 8) | byte2; // junta os dois bytes em 16 bits
    int lixo = cabecalho >> 13;            // 3 bits de cima
    int tam_arvore = cabecalho & 8191;     // 13 bits de baixo (8191 em binario sao 13 uns)

    // tamanho da arvore 0 quer dizer que o arquivo original era vazio
    if (tam_arvore == 0) {
        fclose(compactado);
        fclose(arquivo);
        return;
    }

    // remonta a arvore a partir do arquivo
    No *raiz = ler_arvore(compactado);

    // tirando os 2 bytes do cabecalho e os bytes da arvore, o resto sao os dados
    int bytes_de_dados = tamanho_arquivo - 2 - tam_arvore;
    // le os dados bit a bit: 0 desce pra esquerda e 1 desce pra direita
    // quando chega numa folha, escreve o byte dela e volta pra raiz
    No *atual = raiz;

    for (int i = 0; i < bytes_de_dados; i++) {
        unsigned char byte = fgetc(compactado);

        int ultimo_bit = 0;
        if (i == bytes_de_dados - 1) {
            ultimo_bit = lixo; // no ultimo byte, ignora os bits de lixo
        }

        for (int bit = 7; bit >= ultimo_bit; bit--) {
            if (bit_ligado(byte, bit)) {
                atual = atual->direita;
            } else {
                atual = atual->esquerda;
            }

            if (eh_folha(atual)) {
                fputc(pegar_byte(atual), arquivo);
                atual = raiz;
            }
        }
    }

    liberar_arvore(raiz);
    fclose(compactado);
    fclose(arquivo);
}
