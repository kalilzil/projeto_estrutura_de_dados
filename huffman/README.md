# Huffman - Compactar e Descompactar

Para compilar:
    gcc -Wall -o huffman *.c

Arquivos:
- fila.c         -> Montar a fila de prioridade
- arvore.c       -> Montar a arvore, escrever/ler em pre-ordem, liberar
- compactar.c    -> Contar frequencias, gerar os codigos, escrever cabecalho + arvore + dados
- descompactar.c -> Ler o cabecalho, remontar a arvore e decodificar bit a bit
- main.c         -> menu

Cabecalho: [3 bits lixo][13 bits tamanho da arvore][arvore em pre-ordem][dados]
