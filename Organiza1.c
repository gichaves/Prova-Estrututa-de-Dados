#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SENSORES 500

typedef struct {
    long timest;
    char sensor[32];
    char valor[32]; 
} Leitura;


int compara(const void *a, const void *b) {
    Leitura *l1 = (Leitura *)a;
    Leitura *l2 = (Leitura *)b;
    return (l1->timest - l2->timest);
}


const char* detectarTipo(char *valor) {
    if (strcmp(valor, "true") == 0 || strcmp(valor, "false") == 0)
        return "bool";

    int ponto = 0, i = 0;

    if (valor[0] == '-' || valor[0] == '+') i = 1; // ignora sinal

    int len = strlen(valor);
    for (; i < len; i++) {
        if (valor[i] == '.') {
            ponto++;
        } else if (!isdigit(valor[i])) {
            return "string";
        }
    }

    if (ponto == 1) return "float";
    if (ponto == 0) return "int";
    return "string";
}

int main() {
    FILE *arquivo;
    char nomeArquivo[50];
    printf("Nome do arquivo: ");
    scanf("%s", nomeArquivo);

    arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    Leitura *dados = NULL;
    int quantidade = 0, capacidade = 0;
    char sensores[MAX_SENSORES][32];
    char tipos[MAX_SENSORES][10]; // armazena tipo por sensor
    int quantidadeSensor = 0;

    long ts;
    char sensor[32];
    char valor[32];

    while (fscanf(arquivo, "%ld %s %s", &ts, sensor, valor) == 3) {
        if (quantidade == capacidade) {
            capacidade += 100;
            dados = realloc(dados, capacidade * sizeof(Leitura));
        }

        dados[quantidade].timest = ts;
        strcpy(dados[quantidade].sensor, sensor);
        strcpy(dados[quantidade].valor, valor);
        quantidade++;

        
        int novo = 1;
        for (int i = 0; i < quantidadeSensor; i++) {
            if (strcmp(sensores[i], sensor) == 0) {
                novo = 0;
                break;
            }
        }

        if (novo) {
            strcpy(sensores[quantidadeSensor], sensor);
            strcpy(tipos[quantidadeSensor], detectarTipo(valor));
            quantidadeSensor++;
        }
    }

    fclose(arquivo);

    printf("Total de leituras: %d\n", quantidade);
    printf("Sensores identificados:\n");
    for (int i = 0; i < quantidadeSensor; i++) {
        printf("- %s (tipo: %s)\n", sensores[i], tipos[i]);
    }

   
    for (int i = 0; i < quantidadeSensor; i++) {
        Leitura *dadosSensor = malloc(quantidade * sizeof(Leitura));
        int cont = 0;

        for (int j = 0; j < quantidade; j++) {
            if (strcmp(dados[j].sensor, sensores[i]) == 0) {
                dadosSensor[cont++] = dados[j];
            }
        }

        qsort(dadosSensor, cont, sizeof(Leitura), compara);

        char saida[50];
        sprintf(saida, "%s.txt", sensores[i]);

        FILE *out = fopen(saida, "w");
        if (!out) {
            printf("Erro ao criar arquivo %s\n", saida);
            free(dadosSensor);
            continue;
        }

        for (int j = 0; j < cont; j++) {
            fprintf(out, "%ld %s\n", dadosSensor[j].timest, dadosSensor[j].valor);
        }

        fclose(out);
        free(dadosSensor);

        printf("Criado: %s com %d registros.\n", saida, cont);
    }

    free(dados);
    return 0;
}

