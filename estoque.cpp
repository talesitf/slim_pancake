#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // Add this line

void adicionarItem(std::vector<std::string>& estoque) {
    std::string item;
    std::cout << "Insira o nome do item para adicionar: ";
    std::cin >> item;
    estoque.push_back(item);
    std::cout << "Item adicionado com sucesso!";
}

void removerItem(std::vector<std::string>& estoque) {
    std::string item;
    std::cout << "Insira o nome do item para remover: ";
    std::cin >> item;

    auto it = std::find(estoque.begin(), estoque.end(), item);
    if (it != estoque.end()) {
        estoque.erase(it);
        std::cout << "Item removido com sucesso!";
    } else {
        std::cout << "Item não encontrado!";
    }
}

void visualizarEstoque(const std::vector<std::string>& estoque) {
    std::cout << "Itens no estoque:";
    for (const auto& item : estoque) {
        std::cout << "- " << item;
    }
}

int main() {
    std::vector<std::string> estoque;
    int opcao;

    do {
        std::cout << "\n1. Adicionar Item\n2. Remover Item\n3. Visualizar Estoque\n4. Sair\nEscolha uma opção: ";
        std::cin >> opcao;

        switch (opcao) {
            case 1:
                adicionarItem(estoque);
                break;
            case 2:
                removerItem(estoque);
                break;
            case 3:
                visualizarEstoque(estoque);
                break;
            case 4:
                std::cout << "Saindo...";
                break;
            default:
                std::cout << "Opção inválida!";
        }
    } while (opcao != 4);

    return 0;
}
