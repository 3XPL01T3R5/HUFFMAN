#include <stdio.h>
#include <string.h>
#include "compressModules.h"
#include "decompressModules.h"
#include <locale.h>

int main(int argc, char **argv) {
    setlocale(LC_ALL, "Portuguese");
    if (argc < 2 || argc > 4) {
        printf("Por favor, utilize os argumentos -c ou -d para escolher o que fazer, em seguida, fornceça a localização do arquivo de origem e destino.\n\nPara exibir a ajuda, utilize a opção -help\n");
        return 0;
    }
    if (!strcmp(argv[1], "-c") || !strcmp(argv[1], "-d")) {
        FILE* input = fopen(argv[2], "rb");
        int compress = 0;
        if (!strcmp(argv[1], "-c"))
            compress = 1;
        if (input) {
            FILE* output = fopen(argv[3], "wb");
            if (output) {
                if (compress)
                    compressFile(input, output);
                else
                    decompressFile(input, output);
                fclose(input);
                fclose(output);
            }
            else {
                fputs("Desculpe, o caminho de saída informado é inválido, ou não há privilégio suficiente para acessá-lo.\n",
                      stderr);
                fclose(input);
            }
        }
        else
            fputs("Desculpe, o caminho de entrada informado é inválido, ou não há privilégio suficiente para acessá-lo.\n", stderr);
    }
    else
        printf("Bem vindo ao Huff-It! v0.1 Edição Básica\n\nEste programa pode comprimir ou descomprimir arquivos codificados em huffman, para mais detalhes visite página do mestre Márcio comp208\n\n\nAs opções disponíveis são:\n\n-c: comprime um arquivo\n-d: descomprime um arquivo\n\nCada uma dessas opções segue de dois parâmetros indicando os caminhos do arquivo de origem e destino. O arquivo de destino não precisa existir. Cerifique-se de que você tem permissões para acessar os caminhos informados\n\n\nBOA COMPRESSÃO!\n");
    return 0;
}