#include <iostream>
#include <omp.h>
#include <random>
#include <vector>

int main() {
    std::vector<int> numerosAleatorios;
    numerosAleatorios.reserve(500000);

    // Motor de geração de números aleatórios
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister

    // Define o intervalo dos números aleatórios, por exemplo de 0 a 99999
    std::uniform_int_distribution<> distrib(0, 99999);

    for (int i = 0; i < 500000; i++) {
        numerosAleatorios.push_back(distrib(gen)); // Gera e adiciona um número aleatório ao vetor
    }

    int valorProcurado = 12345; // Valor a ser procurado
    int numThreads = 4;         // Número de threads
    int chunkSize = numerosAleatorios.size() / numThreads;

#pragma omp parallel num_threads(numThreads)
    {
#pragma omp single
        {
            for (int i = 0; i < numThreads; i++) {
                int start = i * chunkSize;
                int end = (i == numThreads - 1) ? numerosAleatorios.size() : start + chunkSize;

#pragma omp task firstprivate(start, end)
                {
                    int count = 0;
                    for (int j = start; j < end; j++) {
                        if (numerosAleatorios[j] == valorProcurado) {
                            count++;
                        }
                    }
#pragma omp critical
                    {
                        std::cout << "Thread " << omp_get_thread_num() << " encontrou " << count << " vezes o valor " << valorProcurado << std::endl;
                    }
                }
            }
        }
    }

    return 0;
}