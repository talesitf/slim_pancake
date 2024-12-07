#include <fstream>
#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <string>
#include <vector>

// Função para ler sequência de RNA de um arquivo
std::string read_rna(const std::string &filename) {
    std::ifstream file(filename);
    std::string sequence, line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '>')
                continue; // Ignorar cabeçalhos
            sequence += line;
        }
        file.close();
    } else {
        throw std::runtime_error("Erro ao abrir o arquivo RNA.");
    }

    return sequence;
}

// Função para contar códons AUG em uma sequência usando OpenMP
int count_codons_openmp(const std::string &rna_sequence) {
    int count = 0;

#pragma omp parallel for reduction(+ : count)
    for (size_t i = 0; i < rna_sequence.size() - 2; ++i) {
        if (rna_sequence[i] == 'A' && rna_sequence[i + 1] == 'U' && rna_sequence[i + 2] == 'G') {
            count++;
        }
    }

    return count;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string sequence;
    size_t total_length;

    // Processo 0 lê a sequência de RNA
    if (rank == 0) {
        if (argc < 2) {
            std::cerr << "Uso: mpirun -np <n_processos> ./count_proteins <arquivo_rna>\n";
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        sequence = read_rna(argv[1]);
        total_length = sequence.size();
    }

    // Broadcast do tamanho total da sequência
    MPI_Bcast(&total_length, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    // Divisão da sequência entre processos MPI
    size_t chunk_size = total_length / size;
    size_t start = rank * chunk_size;
    size_t end = (rank == size - 1) ? total_length : start + chunk_size;

    // Cada processo recebe sua parte da sequência
    std::string local_sequence;
    if (rank == 0) {
        local_sequence = sequence.substr(start, end - start);
    } else {
        local_sequence.resize(end - start);
    }
    MPI_Bcast(local_sequence.data(), local_sequence.size(), MPI_CHAR, 0, MPI_COMM_WORLD);

    // Contagem local de códons AUG com OpenMP
    int local_count = count_codons_openmp(local_sequence);

    // Agregar contagens usando MPI_Reduce
    int global_count = 0;
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Processo 0 exibe o resultado final
    if (rank == 0) {
        std::cout << "Número total de proteínas inicializadas (AUG): " << global_count << "\n";
    }

    MPI_Finalize();
    return 0;
}
