#include <iostream>

int main() {
    int numeros[5];

    for (int i = 0; i < 5; i++) {
        std::cout << "Insira um número: ";
        std::cin >> numeros[i];
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4 - i; j++) {
            if (numeros[j] > numeros[j + 1]) {
                int temp = numeros[j];
                numeros[j] = numeros[j + 1];
                numeros[j + 1] = temp;
            }
        }
    }

    std::cout << "Vetor ordenado: ";
    for (int i = 0; i < 5; i++) {
        std::cout << numeros[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
