#include <iostream>

int main() {
    int numeros[5];
    int soma = 0;

    for (int i = 0; i < 5; i++) {
        std::cout << "Insira um número: ";
        std::cin >> numeros[i];
        soma += numeros[i];
    }

    std::cout << "A soma dos números é: " << soma << std::endl;

    return 0;
}
