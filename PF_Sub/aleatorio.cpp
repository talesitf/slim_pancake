#include <iostream>
#include <vector>
#include <random>

int main() {
	std::vector<int> numerosAleatorios;
	numerosAleatorios.reserve(500000);

	// Motor de geração de números aleatórios
	std::random_device rd;
	std::mt19937 gen(rd()); // Mersenne Twister

	// Define o intervalo dos números aleatórios, por exemplo de 0 a 99999
	std::uniform_int_distribution<> distrib(0, 99999);

	for (int i=0; i<500000; i++) {
		numerosAleatorios.push_back(distrib(gen)); // Gera e adiciona um número aleatório ao vetor
	}

	return 0;
}