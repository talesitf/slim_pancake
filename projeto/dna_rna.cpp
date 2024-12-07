#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <mpi.h>
#include <omp.h>
#include <stdexcept>
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
            // Remover possíveis espaços e concatenar
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
            sequence += line;
        }
        file.close();
    } else {
        throw std::runtime_error("Erro ao abrir o arquivo FASTA: " + filename);
    }

    return sequence;
}

// Função para transcrever DNA em RNA usando OpenMP
std::string transcribe_dna_to_rna(const std::string &dna_sequence) {
    std::string rna_sequence = dna_sequence;
    std::map<char, char> gene = {{'A', 'A'}, {'T', 'U'}, {'C', 'C'}, {'G', 'G'}};

#pragma omp parallel for
    for (size_t i = 0; i < dna_sequence.size(); ++i) {
        char nucleotide = dna_sequence[i];
        // Verifica se o nucleotídeo é válido antes da substituição
        auto it = gene.find(nucleotide);
        if (it != gene.end()) {
            rna_sequence[i] = it->second;
        } else {
            rna_sequence[i] = 'N'; // Representa nucleotídeo desconhecido
        }
    }

    return rna_sequence;
}

// Função para salvar sequência em arquivo FASTA
void save_rna_to_file(const std::string &filename, const std::string &rna_sequence) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Erro ao salvar o arquivo RNA: " + filename);
    }

    file << ">RNA_sequence\n"; // Cabeçalho do arquivo
    // Opcional: quebrar linhas em 60 caracteres para conformidade com FASTA
    for (size_t i = 0; i < rna_sequence.size(); i += 60) {
        file << rna_sequence.substr(i, 60) << "\n";
    }

    file.close();
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Verificar se foram passados pelo menos 22 arquivos
    if (argc < 2) {
        if (rank == 0)
            std::cerr << "Uso: mpirun -np <n_processos> ./dna_to_rna <arquivo1.fasta> <arquivo2.fasta> ... <arquivo22.fasta>\n";
        MPI_Finalize();
        return 1;
    }

    // Lista de arquivos FASTA de entrada
    std::vector<std::string> all_files;
    for (int i = 1; i < argc; ++i) {
        all_files.push_back(argv[i]);
    }

    int total_files = all_files.size();
    std::vector<std::string> assigned_files;

    // Distribuir arquivos entre processos MPI de forma cíclica
    for (int i = rank; i < total_files; i += size) {
        assigned_files.push_back(all_files[i]);
    }

    // Processar arquivos atribuídos
    for (const auto &arquivoDNA : assigned_files) {
        // Extrair o nome base do arquivo para nomear os arquivos de saída
        std::string base_filename = arquivoDNA.substr(arquivoDNA.find_last_of("/\\") + 1); // Remove o caminho

        // Nome dos arquivos de saída
        std::string arquivoRNA = "RNA_" + base_filename;
        std::string aminoacidos = "aminoacidos_" + base_filename + ".txt";

        try {
            // Ler sequência de DNA
            std::string sequencia_dna = read_fasta(arquivoDNA);
            if (sequencia_dna.empty()) {
                std::cerr << "Arquivo " << arquivoDNA << " está vazio ou não contém sequências válidas.\n";
                continue;
            }

            // Transcrever para RNA
            std::string sequencia_rna = transcribe_dna_to_rna(sequencia_dna);

            // Gravar RNA no arquivo de saída
            save_rna_to_file(arquivoRNA, sequencia_rna);

            // Mensagem de progresso
            if (rank == 0) {
                std::cout << "Arquivo transcrito: " << arquivoDNA << " -> " << arquivoRNA << std::endl;
            }
        } catch (const std::exception &e) {
            std::cerr << "Erro ao processar o arquivo " << arquivoDNA << ": " << e.what() << std::endl;
            continue;
        }
    }

    MPI_Finalize();
    return 0;
}
