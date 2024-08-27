#include <iostream>
#include <string>

int main() {
    std::string texto;

    std::cout << "Insira uma string: ";
    std::getline(std::cin, texto);

    std::cout << "Número de caracteres na string: " << texto.length();

    return 0;
}
