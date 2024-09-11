#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

using namespace std;
using namespace std::chrono;

struct Item {
    int peso;
    int valor;
    bool inSolution;
    int id;
};

void bigger_first(vector<Item> &itens, int n, int &W) {
    sort(itens.begin(), itens.end(), [](Item a, Item b) { return a.valor > b.valor; });
    for (int i = 0; i < n; i++) {
        if (itens[i].peso <= W) {
            itens[i].inSolution = true;
            W -= itens[i].peso;
        }
    }
}

void smaller_first(vector<Item> &itens, int n, int &W) {
    sort(itens.begin(), itens.end(), [](Item a, Item b) { return a.peso < b.peso; });
    for (int i = 0; i < n; i++) {
        if (itens[i].peso <= W) {
            itens[i].inSolution = true;
            W -= itens[i].peso;
        }
    }
}

vector<Item> gerarItensMaisValioso(int n, int valor_min, int valor_max, int peso_min, int peso_max) {
    vector<Item> itens;

    for (int i = 0; i < n; ++i) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        mt19937 generator(seed);

        uniform_int_distribution<int> valor_dist(valor_min, valor_max);
        uniform_int_distribution<int> peso_dist(peso_min, peso_max);
        int valor = valor_dist(generator);
        int peso = peso_dist(generator);
        itens.push_back({peso, valor, false, i});
    }
    return itens;
}

vector<Item> gerarItensMaisLeve(int n, int valor_min, int valor_max, int peso_min, int peso_max) {
    vector<Item> itens;
    for (int i = 0; i < n; ++i) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        mt19937 generator(seed);

        uniform_int_distribution<int> valor_dist(valor_min, valor_max);
        uniform_int_distribution<int> peso_dist(peso_min, peso_max);
        int valor = valor_dist(generator);
        int peso = peso_dist(generator);
        itens.push_back({peso, valor, false, i});
    }
    return itens;
}

void imprime_output(ofstream &file, const vector<Item> &itens, int n) {
    int totalWeight = 0;
    int totalValue = 0;
    for (int i = 0; i < n; i++) {
        if (itens[i].inSolution) {
            totalWeight += itens[i].peso;
            totalValue += itens[i].valor;
        }
    }
    file << "Peso total: " << totalWeight << " Valor total: " << totalValue << endl;
}

void write_file(const vector<Item> &itens, const string &filename, int n, int W) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << n << " " << W << endl;
        for (int i = 0; i < n; i++) {
            file << itens[i].peso << " " << itens[i].valor << endl;
        }
        file.close();
    } else {
        cout << "Não foi possível abrir o arquivo " << filename << endl;
    }
}

void reset(vector<Item> &itens, int n) {
    for (int i = 0; i < n; i++) {
        itens[i].inSolution = false;
    }
}

int main() {

    int valor_min_rich = 100, valor_max_rich = 800;
    int peso_min_rich = 5, peso_max_rich = 30;

    int valor_min_light = 10, valor_max_light = 200;
    int peso_min_light = 1, peso_max_light = 20;

    string filepath = "IO_files/output.txt";
    ofstream file(filepath, ios::app);
    if (file.is_open()) {
        int n = 10;
        int W = 100;

        // Caso Itens Mais Valiosos
        file << "Caso: Itens mais valiosos\n"
             << endl;
        vector<Item> itens_rich = gerarItensMaisValioso(n, valor_min_rich, valor_max_rich, peso_min_rich, peso_max_rich);
        string in_rich_filepath = "IO_files/in_BagRich.txt";
        write_file(itens_rich, in_rich_filepath, n, W);

        // Heurística bigger_first para Itens Mais Valiosos
        file << "Heurística: bigger_first\n";
        auto start = high_resolution_clock::now();
        bigger_first(itens_rich, n, W);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        file << "Tempo de execução: " << duration.count() << " microsegundos\n";
        file << "Resultado: ";
        imprime_output(file, itens_rich, n);
        file << endl;
        reset(itens_rich, n); // Reseta a solução para a próxima heurística

        // Heurística smaller_first para Itens Mais Valiosos
        W = 100; // Reseta capacidade
        file << "Heurística: smaller_first\n";
        start = high_resolution_clock::now();
        smaller_first(itens_rich, n, W);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        file << "Tempo de execução: " << duration.count() << " microsegundos\n";
        file << "Resultado: ";
        imprime_output(file, itens_rich, n);
        file << endl;
        reset(itens_rich, n); // Reseta a solução para o próximo caso

        // Caso Itens Mais Leves
        W = 100;
        file << "Caso: Itens mais leves\n"
             << endl;
        vector<Item> itens_light = gerarItensMaisLeve(n, valor_min_light, valor_max_light, peso_min_light, peso_max_light);
        string in_light_filepath = "IO_files/in_BagLight.txt";
        write_file(itens_light, in_light_filepath, n, W);

        // Heurística bigger_first para Itens Mais Leves
        file << "Heurística: bigger_first\n";
        start = high_resolution_clock::now();
        bigger_first(itens_light, n, W);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        file << "Tempo de execução: " << duration.count() << " microsegundos\n";
        file << "Resultado: ";
        imprime_output(file, itens_light, n);
        file << endl;
        reset(itens_light, n); // Reseta a solução para a próxima heurística

        // Heurística smaller_first para Itens Mais Leves
        W = 100;
        file << "Heurística: smaller_first\n";
        start = high_resolution_clock::now();
        smaller_first(itens_light, n, W);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        file << "Tempo de execução: " << duration.count() << " microsegundos\n";
        file << "Resultado: ";
        imprime_output(file, itens_light, n);
        file << endl;

        file.close();

    } else {
        cout << "Não foi possível abrir o arquivo " << filepath << endl;
    }

    return 0;
}
