#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LINE_LENGTH 100
#define OUTPUT_FILE_MEAN "PRIO_STATIC_VM.txt"
#define OUTPUT_FILE_VARIANCE "PRIO_STATIC_V.txt"

// Função para calcular a média
double calcular_media(double *valores, int count) {
    double soma = 0.0;
    for (int i = 0; i < count; i++) {
        soma += valores[i];
    }
    return soma / count;
}

// Função para calcular a variância
double calcular_variancia(double *valores, int count, double media) {
    double soma_quadrados = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = valores[i] - media;
        soma_quadrados += diff * diff;
    }
    return soma_quadrados / count;
}

int main(int argc, char *argv[]) {
    // Verifica se o arquivo foi especificado
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo.txt>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Abre o arquivo especificado para leitura
    FILE *arquivo = fopen(argv[1], "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return EXIT_FAILURE;
    }

    // Array para armazenar os valores e variável para contagem
    double valores[10]; // Ajuste o tamanho conforme necessário
    int count = 0;

    // Lê valores do arquivo
    char linha[MAX_LINE_LENGTH];
    while (fgets(linha, sizeof(linha), arquivo)) {
        // Converte a linha para um número double e armazena no array
        if (sscanf(linha, "%lf", &valores[count]) == 1) {
            count++;
        } else {
            fprintf(stderr, "Erro ao ler o valor da linha: %s", linha);
        }
    }

    // Fecha o arquivo
    fclose(arquivo);

    // Verifica se há valores para calcular a média e a variância
    if (count == 0) {
        fprintf(stderr, "Nenhum valor válido encontrado no arquivo.\n");
        return EXIT_FAILURE;
    }

    // Calcula a média
    double media = calcular_media(valores, count);

    // Calcula a variância
    double variancia = calcular_variancia(valores, count, media);

    // Abre o arquivo de saída para a média
    FILE *arquivo_saida_mean = fopen(OUTPUT_FILE_MEAN, "a");
    if (arquivo_saida_mean == NULL) {
        perror("Erro ao criar o arquivo de resultado da média");
        return EXIT_FAILURE;
    }

    // Escreve a média no arquivo de saída
    fprintf(arquivo_saida_mean, "%.2f\n", media);
    fclose(arquivo_saida_mean);

    // Abre o arquivo de saída para a variância
    FILE *arquivo_saida_variance = fopen(OUTPUT_FILE_VARIANCE, "a");
    if (arquivo_saida_variance == NULL) {
        perror("Erro ao criar o arquivo de resultado da variância");
        return EXIT_FAILURE;
    }

    // Escreve a variância no arquivo de saída
    fprintf(arquivo_saida_variance, "%.2f\n", variancia);
    fclose(arquivo_saida_variance);

    printf("A média dos valores foi salva em %s\n", OUTPUT_FILE_MEAN);
    printf("A variância dos valores foi salva em %s\n", OUTPUT_FILE_VARIANCE);

    return EXIT_SUCCESS;
}
