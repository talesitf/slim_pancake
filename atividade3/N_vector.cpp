#include <iostream>

int main() {
    int numeros[10];
    int maior;

    for (int i = 0; i < 10; i++) {
        std::cout << "Insira um número: ";
        std::cin >> numeros[i];
    }

    maior = numeros[0];

    for (int i = 1; i < 10; i++) {
        if (numeros[i] > maior) {
            maior = numeros[i];
        }
    }

    std::cout << "O maior número é: " << maior;

    return 0;
}
