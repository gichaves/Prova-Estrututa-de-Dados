#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef struct {
    float value;
    char sensor[10];
    long timest;
} ler;

// Captura data/hora e converte para timestamp
time_t CapturaTimestamp() {
    int dia, mes, ano, hora, min, seg;
    struct tm t;

    while (1) {
        printf("Digite a data e hora (dd mm aaaa hh mm ss): ");
        if (scanf("%d %d %d %d %d %d", &dia, &mes, &ano, &hora, &min, &seg) != 6) {
            while (getchar() != '\n');
            printf("Erro!\n");
            continue;
        }

        t.tm_year = ano - 1900;
        t.tm_mon = mes - 1;
        t.tm_mday = dia;
        t.tm_hour = hora;
        t.tm_min = min;
        t.tm_sec = seg;
        t.tm_isdst = -1;

        time_t timestamp = mktime(&t);
        if (timestamp == -1) {
            printf("Data invalida!\n");
        } else {
            return timestamp;
        }
    }
}

// Busca binária 
int buscaMaisProxima(ler *dados, int n, long alvo) {
    int esquerda = 0, direita = n - 1;
    int melhorIndice = 0;
    long menorDif = labs(dados[0].timest - alvo);

    while (esquerda <= direita) {
        int meio = (esquerda + direita) / 2;
        long dif = labs(dados[meio].timest - alvo);

        if (dif < menorDif) {
            menorDif = dif;
            melhorIndice = meio;
        }

        if (dados[meio].timest < alvo)
            esquerda = meio + 1;
        else if (dados[meio].timest > alvo)
            direita = meio - 1;
        else
            return meio; // valor 
    }

    return melhorIndice;
}

int main() {
    char nomeSensor[10];
    printf("Sensor: ");
    scanf("%s", nomeSensor);

    time_t alvo = CapturaTimestamp();

    char nomeArquivo[20];
    sprintf(nomeArquivo, "%s.txt", nomeSensor);

    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    ler dados[1000];
    int quantidade = 0;

    while (fscanf(arquivo, "%ld %s %f", &dados[quantidade].timest, dados[quantidade].sensor, &dados[quantidade].value) == 3) {
        quantidade++;
    }

    fclose(arquivo);

    if (quantidade == 0) {
        printf("Nenhuma leitura!\n");
        return 1;
    }

    int indice = buscaMaisProxima(dados, quantidade, alvo);
    long diferenca = labs(dados[indice].timest - alvo);

    
    char recebido[30], encontrado[30];
    struct tm *info;

    info = localtime(&alvo);
    strftime(recebido, 30, "%d/%m/%Y %H:%M:%S", info);

    info = localtime(&dados[indice].timest);
    strftime(encontrado, 30, "%d/%m/%Y %H:%M:%S", info);

    //resultados
    printf("\nData recebida: %s\n", recebido);
    printf("Sensor usado: %s\n", dados[indice].sensor);
    printf("Data e Hora mais proxima: %s\n", encontrado);
    printf("Valor: %.2f\n", dados[indice].value);
    printf("Diferenca de tempo: %ld segundos\n", diferenca);

    return 0;
}

