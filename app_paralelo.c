
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ARQUIVO    "bovespa_stocks.csv"
#define REPETICOES 10

// Guardando em um struct o que vamos usar: fechamento e volume
typedef struct {
    double fechamento;
    double volume;
} Registro;

// Separando dados por mês: meses[0] = janeiro até meses[11] = dezembro
Registro *meses[12];
long      qtd_mes[12];
long      capacidade[12];

// Resultados de cada mes
double media[12], maior[12], menor[12], vol_total[12];

// Qual thread calculou cada mês (só para mostrar na tela)
int thread_do_mes[12];

const char *NOMES[12] = {
    "Janeiro", "Fevereiro", "Marco", "Abril", "Maio", "Junho",
    "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"
};

// Lê o CSV e coloca cada registro no vetor do seu mês
int ler_csv(const char *nome)
{
    FILE *fp = fopen(nome, "r");
    if (fp == NULL) {
        printf("Erro ao abrir %s\n", nome);
        return 0;
    }

    for (int m = 0; m < 12; m++) {
        capacidade[m] = 1024;
        qtd_mes[m] = 0;
        meses[m] = malloc(capacidade[m] * sizeof(Registro));
    }

    char linha[256];
    fgets(linha, 256, fp); // Pra pular o cabeçalho

    while (fgets(linha, 256, fp) != NULL) {
        char data[32], ticker[16];
        double adj, fech, maxi, mini, abert, vol;

        int lidos = sscanf(linha, "%31[^,],%15[^,],%lf,%lf,%lf,%lf,%lf,%lf",
                           data, ticker, &adj, &fech, &maxi, &mini, &abert, &vol);

        if (lidos != 8 || fech <= 0) continue;

        int m = (data[5] - '0') * 10 + (data[6] - '0') - 1; // Subtraimos 1 para corresponder ao indice dos meses 0-11

        if (qtd_mes[m] == capacidade[m]) {
            capacidade[m] *= 2;
            meses[m] = realloc(meses[m], capacidade[m] * sizeof(Registro));
        }

        meses[m][qtd_mes[m]].fechamento = fech;
        meses[m][qtd_mes[m]].volume     = vol;
        qtd_mes[m]++;
    }

    fclose(fp);
    return 1;
}

// Calcula as estatisticas de um mês
void calcular_mes(int m)
{
    double soma = 0, vol = 0;
    double mx = meses[m][0].fechamento;
    double mn = meses[m][0].fechamento;

    for (long i = 0; i < qtd_mes[m]; i++) {
        double f = meses[m][i].fechamento;
        soma += f;
        vol  += meses[m][i].volume;
        if (f > mx) mx = f;
        if (f < mn) mn = f;
    }

    media[m]     = soma / qtd_mes[m];
    maior[m]     = mx;
    menor[m]     = mn;
    vol_total[m] = vol;
}

int main(void)
{
    if (!ler_csv(ARQUIVO)) return 1;

    // Medindo o tempo
    double inicio = omp_get_wtime();

    for (int r = 0; r < REPETICOES; r++) {
        // Fazendo a cada uma das 12 threads receber o valor de um dos meses (equivalente a uma volta do for)
        #pragma omp parallel for num_threads(12)
        for (int m = 0; m < 12; m++) {
            calcular_mes(m);
            thread_do_mes[m] = omp_get_thread_num();
        }
    }

    double tempo = (omp_get_wtime() - inicio) / REPETICOES;

    // Resultados
    printf("=== PARALELO (12 threads) ===\n");
    printf("%-10s %9s %10s %10s %8s %16s\n",
           "Mes", "Registros", "Media", "Maior", "Menor", "Volume total");
    for (int m = 0; m < 12; m++) {
        printf("%-10s %9ld %10.2f %10.2f %8.2f %16.0f\n",
               NOMES[m], qtd_mes[m], media[m], maior[m], menor[m], vol_total[m]);
    }

    // Mostrando qual thread fez qual mês
    printf("\nDivisao do trabalho:\n");
    for (int m = 0; m < 12; m++) {
        printf("  Thread %2d -> %s\n", thread_do_mes[m] + 1, NOMES[m]);
    }

    printf("\nTempo paralelo: %.6f s\n", tempo);

    for (int m = 0; m < 12; m++) free(meses[m]);
    return 0;
}