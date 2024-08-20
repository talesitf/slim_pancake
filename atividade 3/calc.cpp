#include <iostream>

int main() {
    double num1, num2;
    char operacao;

    std::cout << "Insira o primeiro número: ";
    std::cin >> num1;
    std::cout << "Insira a operação (+, -, *, /): ";
    std::cin >> operacao;
    std::cout << "Insira o segundo número: ";
    std::cin >> num2;


    switch (operacao) {
        case '+':
            std::cout << "Resultado: " << num1 + num2;
            break;
        case '-':
            std::cout << "Resultado: " << num1 - num2;
            break;
        case '*':
            std::cout << "Resultado: " << num1 * num2;
            break;
        case '/':
            if (num2 != 0)
                std::cout << "Resultado: " << num1 / num2;
            else
                std::cout << "Erro: Divisão por zero!";
            break;
        default:
            std::cout << "Operação inválida!";
            break;
    }

    return 0;
}
