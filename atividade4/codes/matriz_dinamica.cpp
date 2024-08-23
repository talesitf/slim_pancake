#include <chrono>   // Inclui a biblioteca para medição de tempo, usada para medir a performance do código.
#include <iostream> // Inclui a biblioteca padrão de entrada e saída, usada para operações como std::cout.

using namespace std::chrono;
using namespace std;

int main() {
    int N = 1000; // Define o tamanho da matriz como N x N, onde N é 1000.

    // Alocação dinâmica de memória para uma matriz N x N.
    // Primeiro, aloca um array de ponteiros, onde cada ponteiro irá apontar para uma linha da matriz.
    int **matriz = new int *[N];

    // Para cada linha da matriz, aloca um array de inteiros de tamanho N.
    for (int i = 0; i < N; ++i) {
        matriz[i] = new int[N];
    }

    // Aqui você pode inicializar e somar os elementos da matriz.
    // Por exemplo, você pode preencher a matriz com valores e calcular a soma total.
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matriz[i][j] = i + j;
        }
    }

    long long soma = 0;
    auto inicio = high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            soma += matriz[i][j];
        }
    }

    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<milliseconds>(fim - inicio).count();

    // Liberação da memória alocada dinamicamente.
    // Primeiro, libera a memória alocada para cada linha (os arrays de inteiros).
    for (int i = 0; i < N; ++i) {
        delete[] matriz[i];
    }

    // Finalmente, libera a memória alocada para o array de ponteiros.
    delete[] matriz;

    cout << "Soma de todos os elementos: " << soma << endl;
    cout << "Tempo de execução: " << duracao << "ms" << endl;

    return 0; // Retorna 0, indicando que o programa terminou com sucesso.
}