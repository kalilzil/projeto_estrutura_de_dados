# Huffman - Compactar e Descompactar

Para compilar:
    gcc -Wall -o huffman *.c

Para utilizar:

Linux/WSL ./huffman -> Escolhe o item do Menu

Para compactar:

-> Informe o nome do arquivo + extensão de entrada (.txt, .pdf, .jpeg, ...)
-> Informe o nome do arquivo de saida + .huff

Para descompactar:

-> Informe o nome do arquivo .huff
-> Informe o nome do arquivo de saida com a extensão original (.txt, .pdf, .jpeg, ...)

Arquivos:
- fila.c         -> Montar a fila de prioridade
- arvore.c       -> Montar a arvore, escrever/ler em pre-ordem, liberar
- compactar.c    -> Contar frequencias, gerar os codigos, escrever cabecalho + arvore + dados
- descompactar.c -> Ler o cabecalho, remontar a arvore e decodificar bit a bit
- main.c         -> Menu
- texto.txt      -> Exemplo dos slides de aula
- texto.huff     -> Arquivo de exemplo compactado

Cabecalho: [3 bits lixo][13 bits tamanho da arvore][arvore em pre-ordem][dados]
