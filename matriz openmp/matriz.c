// matriz.c
#include <stdlib.h>
#include <omp.h>
#include "matriz.h"

matriz_t *matriz_criar(int linhas, int colunas) {
    matriz_t *m = malloc(sizeof(*m));
    double *data = malloc(linhas * colunas * sizeof(*data));
    m->dados  = malloc(linhas * sizeof(*m->dados));
    m->linhas = linhas;
    m->colunas= colunas;
    for (int i = 0; i < linhas; i++)
        m->dados[i] = data + i * colunas;
    return m;
}

void matriz_destruir(matriz_t *m) {
    free(m->dados[0]);
    free(m->dados);
    free(m);
}

void matriz_preencher_rand(matriz_t *m) {
    for (int i = 0; i < m->linhas; i++)
        for (int j = 0; j < m->colunas; j++)
            m->dados[i][j] = (double)random();
}

void matriz_multiplicar_openmp(matriz_t *A, matriz_t *B, matriz_t *C) {
    int n = A->linhas, p = A->colunas, m = B->colunas;

    // zera C
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            C->dados[i][j] = 0.0;

    // usa schedule definido em omp_set_schedule()
    #pragma omp parallel for collapse(2) schedule(runtime)
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            double s = 0.0;
            for (int k = 0; k < p; k++)
                s += A->dados[i][k] * B->dados[k][j];
            C->dados[i][j] = s;
        }
}
