#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SENSORES 100
#define TOTAL_LEITURAS 2000

// Converte string 
time_t parse_data(const char *data) {
    struct tm t;
    int d, m, a, h, min, s;
    sscanf(data, "%d %d %d %d %d %d", &d, &m, &a, &h, &min, &s);
    t.tm_mday = d;
    t.tm_mon = m - 1;
    t.tm_year = a - 1900;
    t.tm_hour = h;
    t.tm_min = min;
    t.tm_sec = s;
    t.tm_isdst = -1;
    return mktime(&t);
}

time_t gerar_timestamp(time_t inicio, time_t fim) {
    return inicio + rand() % (fim - inicio + 1);
}

void gerar_valor(const char *tipo, char *saida) {
    if (strcmp(tipo, "int") == 0) {
        sprintf(saida, "%d", rand() % 1000);
    } else if (strcmp(tipo, "float") == 0) {
        sprintf(saida, "%.2f", (rand() % 100000) / 100.0);
    } else if (strcmp(tipo, "bool") == 0) {
        sprintf(saida, (rand() % 2) ? "true" : "false");
    } else if (strcmp(tipo, "string") == 0) {
        int len = rand() % 10 + 5;
        for (int i = 0; i < len; i++) {
            saida[i] = 'A' + rand() % 26;
        }
        saida[len] = '\0';
    } else {
        strcpy(saida, "NULO");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso: %s \"dd mm aaaa hh mm ss\" \"dd mm aaaa hh mm ss\" SENSOR:TIPO ...\n", argv[0]);
        return 1;
    }

    srand(time(NULL));

    time_t inicio = parse_data(argv[1]);
    time_t fim = parse_data(argv[2]);
    if (fim <= inicio) {
        printf("A data final deve ser após a inicial.\n");
        return 1;
    }

    int totalSensores = argc - 3;
    char sensores[MAX_SENSORES][20];
    char tipos[MAX_SENSORES][10];

    for (int i = 0; i < totalSensores; i++) {
        char *token = strtok(argv[i + 3], ":");
        if (!token) {
            printf("Erro no sensor %s\n", argv[i + 3]);
            return 1;
        }
        strcpy(sensores[i], token);
        token = strtok(NULL, ":");
        if (!token) {
            printf("Erro no tipo do sensor %s\n", argv[i + 3]);
            return 1;
        }
        strcpy(tipos[i], token);
    }

    FILE *arquivo = fopen("leitura_teste.txt", "w");
    if (!arquivo) {
        perror("Erro ao criar o arquivo");
        return 1;
    }

    for (int i = 0; i < totalSensores; i++) {
        for (int j = 0; j < TOTAL_LEITURAS; j++) {
            time_t ts = gerar_timestamp(inicio, fim);
            char valor[50];
            gerar_valor(tipos[i], valor);
            fprintf(arquivo, "%ld %s %s\n", ts, sensores[i], valor);
        }
    }

    fclose(arquivo);
    printf("Arquivo 'leitura_teste.txt' criado com sucesso com %d sensores e %d leituras por sensor.\n", totalSensores, TOTAL_LEITURAS);
    return 0;
}
