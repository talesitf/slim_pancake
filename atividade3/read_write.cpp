#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>

int main() {
    std::ifstream arquivoEntrada("texto.txt");
    if (!arquivoEntrada) {
        std::cout << "Erro ao abrir o arquivo de entrada!" << std::endl;
        return 1;
    }

    std::ofstream arquivoSaida("estatisticas.txt");
    if (!arquivoSaida) {
        std::cout << "Erro ao abrir o arquivo de saída!" << std::endl;
        return 1;
    }

    int totalPalavras = 0, totalLinhas = 0;
    std::map<std::string, int> frequenciaPalavras;
    std::string linha, palavra;

    while (std::getline(arquivoEntrada, linha)) {
        totalLinhas++;
        std::istringstream stream(linha);
        while (stream >> palavra) {
            totalPalavras++;
            frequenciaPalavras[palavra]++;
        }
    }

    int maiorFrequencia = 0;
    std::string palavraMaisFrequente;

    for (const auto& par : frequenciaPalavras) {
        if (par.second > maiorFrequencia) {
            maiorFrequencia = par.second;
            palavraMaisFrequente = par.first;
        }
    }

    arquivoSaida << "Total de palavras: " << totalPalavras << std::endl;
    arquivoSaida << "Total de linhas: " << totalLinhas << std::endl;
    arquivoSaida << "Média de palavras por linha: " << static_cast<double>(totalPalavras) / totalLinhas << std::endl;
    arquivoSaida << "Palavra mais frequente: " << palavraMaisFrequente << " (aparece " << maiorFrequencia << " vezes)" << std::endl;

    arquivoEntrada.close();
    arquivoSaida.close();

    std::cout << "Estatísticas gravadas no arquivo 'estatisticas.txt'." << std::endl;

    return 0;
}
