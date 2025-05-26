#include <complex>
#include <iostream>
#include <mpi.h>
#include <omp.h> 

using namespace std;

int main(int argc, char **argv) {
    int max_row, max_column, max_n;
    int id, size;
    MPI_Init(&argc, &argv); // Inicializa o MPI

    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Número de processos
    MPI_Comm_rank(MPI_COMM_WORLD, &id);    // ID do processo

    int dados_enviar[3];

    if (id == 0) {
        cin >> max_row;
        cin >> max_column;
        cin >> max_n;
        dados_enviar[0] = max_row;
        dados_enviar[1] = max_column;
        dados_enviar[2] = max_n;
    }

    MPI_Bcast(&dados_enviar, 3, MPI_INT, 0, MPI_COMM_WORLD);

    max_row = dados_enviar[0];
    max_column = dados_enviar[1];
    max_n = dados_enviar[2];

    char *dados = (char *) malloc(sizeof(char) * max_row * max_column);
    char **mat = (char **) malloc(sizeof(char*) * max_row);

    for (int i = 0; i < max_row; i++) {
        mat[i] = &dados[i * max_column];
    }

    // Cálculo do intervalo de linhas para cada processo
    int linhas_base = max_row / size;
    int resto = max_row % size;
    int inicio = id * linhas_base + min(id, resto);
    int linhas_processar = linhas_base + (id < resto ? 1 : 0);
    int fim = inicio + linhas_processar;

    // Paralelização com OpenMP
    #pragma omp parallel for schedule(runtime)
    for (int r = inicio; r < fim; ++r) {
        for (int c = 0; c < max_column; ++c) {
            complex<float> z;
            int n = 0;
            while (abs(z) < 2 && ++n < max_n)
                z = pow(z, 2) + decltype(z)((float)c * 2 / max_column - 1.5, (float)r * 2 / max_row - 1);
            mat[r][c] = (n == max_n ? '#' : '.');
        }
    }

    // Vetores para MPI_Gatherv
    int *recvcounts = nullptr;
    int *displs = nullptr;
    if (id == 0) {
        recvcounts = new int[size];
        displs = new int[size];
        for (int i = 0, pos = 0; i < size; ++i) {
            int linhas = linhas_base + (i < resto ? 1 : 0);
            recvcounts[i] = linhas * max_column;
            displs[i] = pos;
            pos += linhas * max_column;
        }
    }

    int sendcount = linhas_processar * max_column;

    MPI_Gatherv(
        &mat[inicio][0], sendcount, MPI_CHAR,
        (id == 0 ? &mat[0][0] : nullptr), recvcounts, displs, MPI_CHAR,
        0, MPI_COMM_WORLD
    );

    if (id == 0) {
        for (int r = 0; r < max_row; ++r) {
            for (int c = 0; c < max_column; ++c)
                cout << mat[r][c];
            cout << '\n';
        }
        delete[] recvcounts;
        delete[] displs;
    }

    MPI_Finalize(); // Finaliza o MPI

    // Libera memória alocada
    free(mat);
    free(dados);

    return 0;
}

