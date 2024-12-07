#include <fstream>
#include <iostream>
#include <map>
#include <mpi.h>
#include <omp.h>
#include <string>
#include <vector>

// Função para ler arquivo FASTA
std::string read_fasta(const std::string &filename) {
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
        throw std::runtime_error("Erro ao abrir o arquivo FASTA.");
    }

    return sequence;
}

// Função para contar bases usando OpenMP
std::map<char, int> count_bases_openmp(const std::string &sequence) {
    std::map<char, int> counts = {{'A', 0}, {'T', 0}, {'C', 0}, {'G', 0}};
    std::vector<int> local_counts(4, 0); // [A, T, C, G]

#pragma omp parallel
    {
        std::vector<int> thread_counts(4, 0);

#pragma omp for
        for (size_t i = 0; i < sequence.size(); ++i) {
            char base = sequence[i];
            if (base == 'A')
                thread_counts[0]++;
            else if (base == 'T')
                thread_counts[1]++;
            else if (base == 'C')
                thread_counts[2]++;
            else if (base == 'G')
                thread_counts[3]++;
        }

// Combinar resultados dos threads
#pragma omp critical
        for (int i = 0; i < 4; ++i) {
            local_counts[i] += thread_counts[i];
        }
    }

    // Mapear contagens para 'A', 'T', 'C', 'G'
    counts['A'] = local_counts[0];
    counts['T'] = local_counts[1];
    counts['C'] = local_counts[2];
    counts['G'] = local_counts[3];

    return counts;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc - 1 < size) {
        if (rank == 0) {
            std::cerr << "Erro: Número de arquivos deve ser >= número de processos MPI.\n";
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Cada processo recebe um arquivo
    std::string filename = argv[rank + 1];
    std::string sequence = read_fasta(filename);

    // Contagem local com OpenMP
    std::map<char, int> local_counts = count_bases_openmp(sequence);

    // Preparar arrays para MPI_Reduce
    int local_counts_arr[4] = {local_counts['A'], local_counts['T'], local_counts['C'], local_counts['G']};
    int global_counts[4] = {0, 0, 0, 0};

    // Agregar resultados de todos os processos MPI
    MPI_Reduce(local_counts_arr, global_counts, 4, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Processo 0 exibe os resultados finais
    if (rank == 0) {
        std::cout << "Contagem Final:\n";
        std::cout << "A: " << global_counts[0] << "\n";
        std::cout << "T: " << global_counts[1] << "\n";
        std::cout << "C: " << global_counts[2] << "\n";
        std::cout << "G: " << global_counts[3] << "\n";
    }

    MPI_Finalize();
    return 0;
}
