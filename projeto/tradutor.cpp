#include <algorithm>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

// Função para ler sequência de DNA de um arquivo
std::string read_dna(const std::string &filename) {
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
        throw std::runtime_error("Erro ao abrir o arquivo DNA: " + filename);
    }

    return sequence;
}

// Função para inicializar o mapa de transcrição DNA -> RNA
std::unordered_map<char, char> initialize_transcription_map() {
    return {
        {'A', 'U'},
        {'T', 'A'}, // Timina -> Uracila
        {'C', 'G'},
        {'G', 'C'}};
}

// Função para transcrever DNA em RNA usando OpenMP
std::string transcribe_dna_to_rna(const std::string &dna_sequence, const std::unordered_map<char, char> &transcription_map) {
    std::string rna_sequence = dna_sequence;

#pragma omp parallel for
    for (size_t i = 0; i < dna_sequence.size(); ++i) {
        auto it = transcription_map.find(dna_sequence[i]);
        if (it != transcription_map.end()) {
            rna_sequence[i] = it->second;
        } else {
            rna_sequence[i] = 'N'; // Representa nucleotídeo desconhecido
        }
    }

    return rna_sequence;
}

// Função para inicializar o mapa de códons para aminoácidos
std::unordered_map<std::string, int> initialize_codon_map() {
    return {
        {"CCA", 1}, {"CCG", 1}, {"CCU", 1}, {"CCC", 1}, // Prolina
        {"UCU", 2},
        {"UCA", 2},
        {"UCG", 2},
        {"UCC", 2}, // Serina
        {"CAG", 3},
        {"CAA", 3}, // Glutamina
        {"ACA", 4},
        {"ACC", 4},
        {"ACU", 4},
        {"ACG", 4}, // Treonina
        {"AUG", 5}, // Metionina (Início)
        {"UGA", -1},
        {"UAA", -1},
        {"UAG", -1}, // STOP
        {"UGC", 6},
        {"UGU", 6}, // Cisteína
        {"GUG", 7},
        {"GUA", 7},
        {"GUC", 7},
        {"GUU", 7} // Valina
    };
}

// Função para traduzir RNA em sequências de aminoácidos completas
std::vector<std::vector<int>> translate_rna_complete(const std::string &rna_sequence, const std::unordered_map<std::string, int> &codon_map) {
    std::vector<std::vector<int>> proteins; // Armazena todas as proteínas
    std::vector<int> current_protein;       // Proteína em construção
    size_t sequence_length = rna_sequence.size();

    // Primeiro, traduz todos os códons em paralelo
    std::vector<int> amino_acid_list(sequence_length / 3, 0);

#pragma omp parallel for
    for (size_t i = 0; i < sequence_length - 2; i += 3) {
        std::string codon = rna_sequence.substr(i, 3);
        auto it = codon_map.find(codon);
        if (it != codon_map.end()) {
            amino_acid_list[i / 3] = it->second;
        } else {
            amino_acid_list[i / 3] = 0; // Código para aminoácido desconhecido
        }
    }

    // Agora, percorre a lista de aminoácidos para identificar proteínas
    for (size_t i = 0; i < amino_acid_list.size(); ++i) {
        int aa = amino_acid_list[i];
        if (aa == 5) { // Metionina (AUG) - Início
            current_protein.push_back(aa);
        } else if (aa == -1 && !current_protein.empty()) { // STOP
            proteins.push_back(current_protein);
            current_protein.clear();
        } else if (!current_protein.empty() && aa > 0) { // Adiciona aminoácido
            current_protein.push_back(aa);
        }
    }

    return proteins;
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

    // Verificar argumentos
    if (argc < 2) {
        if (rank == 0)
            std::cerr << "Uso: mpirun -np <n_processos> ./tradutor <arquivo_dna1.fa> <arquivo_dna2.fa> ...\n";
        MPI_Finalize();
        return 1;
    }

    // Lista de arquivos DNA
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

    // Inicializar mapas
    std::unordered_map<char, char> transcription_map = initialize_transcription_map();
    std::unordered_map<std::string, int> codon_map = initialize_codon_map();

    // Processar arquivos atribuídos
    int file_index = 1;
    for (const auto &arquivoDNA : assigned_files) {
        // Nome do arquivo de saída RNA e proteínas
        std::string arquivoRNA = "RNA_chr" + std::to_string(file_index) + ".fa";
        std::string aminoacidos = "aminoacidos_chr" + std::to_string(file_index) + ".txt";
        file_index++;

        try {
            // Ler sequência de DNA
            std::string sequencia_dna = read_dna(arquivoDNA);

            // Transcrever para RNA
            std::string sequencia_rna = transcribe_dna_to_rna(sequencia_dna, transcription_map);

            // Gravar RNA no arquivo de saída
            std::ofstream rnaFile(arquivoRNA);
            if (!rnaFile.is_open()) {
                std::cerr << "Erro ao abrir o arquivo de saída RNA: " << arquivoRNA << std::endl;
                continue;
            }

            // Escrever em formato FASTA
            rnaFile << ">RNA_" << arquivoDNA << "\n";
            // Opcional: quebrar linhas em 60 caracteres para conformidade com FASTA
            for (size_t i = 0; i < sequencia_rna.size(); i += 60) {
                rnaFile << sequencia_rna.substr(i, 60) << "\n";
            }
            rnaFile.close();

            // Traduzir sequência de RNA para proteínas
            std::vector<std::vector<int>> proteins = translate_rna_complete(sequencia_rna, codon_map);

            // Gravar proteínas no arquivo de saída
            std::ofstream aminoacidosFile(aminoacidos, std::ios::app);
            if (!aminoacidosFile.is_open()) {
                std::cerr << "Erro ao abrir o arquivo de saída de proteínas: " << aminoacidos << std::endl;
                continue;
            }

            for (const auto &protein : proteins) {
                for (int amino : protein) {
                    aminoacidosFile << amino << " ";
                }
                aminoacidosFile << std::endl;
            }

            aminoacidosFile.close();

            // Contar códons AUG
            int count_AUG = count_codons_openmp(sequencia_rna);

            // Gravar contagem de AUG no arquivo de saída
            std::string count_file = "count" + std::to_string(file_index - 1) + ".txt";
            std::ofstream countFile(count_file, std::ios::app);
            if (!countFile.is_open()) {
                std::cerr << "Erro ao abrir o arquivo de contagem AUG: " << count_file << std::endl;
                continue;
            }
            countFile << count_AUG << std::endl;
            countFile.close();

            if (rank == 0) {
                std::cout << "Arquivo processado: " << arquivoDNA << std::endl;
            }
        } catch (const std::exception &e) {
            std::cerr << "Erro ao processar o arquivo " << arquivoDNA << ": " << e.what() << std::endl;
            continue;
        }
    }

    MPI_Finalize();
    return 0;
}
