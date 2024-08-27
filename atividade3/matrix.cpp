#include <iostream>

int main() {
    int matriz[3][3];
    int soma = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << "Insira um número para a posição [" << i << "][" << j << "]: ";
            std::cin >> matriz[i][j];
        }
    }

    for (int i = 0; i < 3; i++) {
        soma += matriz[i][i];
    }

    std::cout << "A soma dos elementos da diagonal principal é: " << soma << std::endl;

    return 0;
}
