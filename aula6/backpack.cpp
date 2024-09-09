#include <chrono> // Inclui a biblioteca para medição de tempo, usada para medir a performance do código.
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace std::chrono;

int backpack(int n, int W, const vector<int> &peso, const vector<int> &valores, int &peso_total) {

    if (n == 0 || W == 0) {
        return 0;
    } else if (peso[n - 1] > W) {
        return backpack(n - 1, W, peso, valores, peso_total);
    } else {
        // Tentamos incluir e não incluir o item, calculando o máximo
        int pesoSemItem = peso_total;               // Peso se não incluirmos o item
        int pesoComItem = peso_total + peso[n - 1]; // Peso se incluirmos o item
        int valorComItem = valores[n - 1] + backpack(n - 1, W - peso[n - 1], peso, valores, pesoComItem);

        int valorSemItem = backpack(n - 1, W, peso, valores, pesoSemItem);

        if (valorComItem > valorSemItem) {
            peso_total = pesoComItem;
            return valorComItem;
        } else {
            peso_total = pesoSemItem;
            return valorSemItem;
        }
    }
}

void write_output(const string &output) {
    ofstream file("IO_files/output.txt", ios::app);
    file << output << endl;
    file.close();
}

int main() {
    int n;
    int W;
    cin >> n >> W;
    vector<int> peso(n);
    vector<int> valores(n);
    for (int i = 0; i < n; i++) {
        cin >> peso[i] >> valores[i];
    }
    int peso_total = 0;

    auto inicio = high_resolution_clock::now();

    int result = backpack(n, W, peso, valores, peso_total);

    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<milliseconds>(fim - inicio).count();

    string output = "Peso ocupado: " + to_string(peso_total) + " Valor alcançado: " + to_string(result) + " dinheiros" + "\n" + to_string(duracao) + "ms";
    write_output(output);
    cout << output << endl;
}