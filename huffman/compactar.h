#ifndef COMPACTAR_H
#define COMPACTAR_H

// compacta o arquivo entrada e salva o resultado em saida (.huff)
// formato do .huff: [3 bits de lixo][13 bits do tamanho da arvore][arvore][dados]
void compactar(char *entrada, char *saida);

#endif
