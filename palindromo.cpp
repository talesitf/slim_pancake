#include <iostream>
#include <string>

int main() {
    std::string texto;
    std::cout << "Insira uma string: ";
    std::getline(std::cin, texto);

    bool palindromo = true;
    int n = texto.length();

    for (int i = 0; i < n / 2; i++) {
        if (texto[i] != texto[n - i - 1]) {
            palindromo = false;
            break;
        }
    }

    if (palindromo)
        std::cout << "A string é um palíndromo." << std::endl;
    else
        std::cout << "A string não é um palíndromo." << std::endl;

    return 0;
}
