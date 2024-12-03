#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        std::cerr << "Este programa requer exatamente 2 processos MPI." << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int bola = 0;
    int posses = 0;
    bool jogoContinua = true;
    std::srand(std::time(0) + rank); // Inicializa o gerador de números aleatórios

    while (jogoContinua) {
        if (rank == 0) {
            if (bola == 0) {
                bola = 1;
                posses++;
                MPI_Send(&bola, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            } else {
                MPI_Recv(&bola, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }

        } else if (rank == 1) {
            MPI_Recv(&bola, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            posses++;
            bola = 0;
            MPI_Send(&bola, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }

        if (posses >= 1000000) {
            double chance = (std::rand() % 100) / 100.0;
            if (chance < 0.7) {
                jogoContinua = false;
                std::cout << "Rank " << rank << " desistiu do jogo após " << posses << " posses." << std::endl;
                MPI_Abort(MPI_COMM_WORLD, 0);
            }
        }
    }

    MPI_Finalize();
    return 0;
}