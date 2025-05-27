// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "matriz.h"

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <N> <NUM_THREADS> <SCHEDULE>\n"
                        "  SCHEDULE = static | dynamic | guided\n",
                argv[0]);
        return EXIT_FAILURE;
    }

    int N           = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    char *sched_str = argv[3];
    omp_sched_t kind;

    if      (strcmp(sched_str, "static")  == 0) kind = omp_sched_static;
    else if (strcmp(sched_str, "dynamic") == 0) kind = omp_sched_dynamic;
    else if (strcmp(sched_str, "guided")  == 0) kind = omp_sched_guided;
    else {
        fprintf(stderr, "Escalonamento desconhecido: %s\n", sched_str);
        return EXIT_FAILURE;
    }

    omp_set_num_threads(num_threads);
    omp_set_schedule(kind, 1);  // chunk size = 1

    matriz_t *A = matriz_criar(N, N);
    matriz_t *B = matriz_criar(N, N);
    matriz_t *C = matriz_criar(N, N);

    matriz_preencher_rand(A);
    matriz_preencher_rand(B);

    matriz_multiplicar_openmp(A, B, C);

    // matriz_imprimir(C);  // descomente para verificar

    matriz_destruir(A);
    matriz_destruir(B);
    matriz_destruir(C);

    return EXIT_SUCCESS;
}
