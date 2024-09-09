#include <algorithm>
#include <chrono> // Inclui a biblioteca para medição de tempo, usada para medir a performance do código.
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

using namespace std;
using namespace std::chrono;

struct Item {
    int peso;
    int valor;
};

int fill_first(vector<Item> &itens, int n, int W, int &peso_total) {
    if (n == 0 || W == 0) {
        return 0;
    } else if (itens[n - 1].peso > W) {
        return fill_first(itens, n - 1, W, peso_total);
    } else {
        peso_total += itens[n - 1].peso;
        return itens[n - 1].valor + fill_first(itens, n - 1, W - itens[n - 1].peso, peso_total);
    }
}

int prob_fill(vector<Item> &itens, int n, int W, int &peso_total) {
    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<double> distribution(0.0, 1.0);

    double prob = distribution(generator);

    if (n == 0 || W == 0) {
        return 0;
    } else if (itens[n - 1].peso > W || prob < 0.5) {
        return prob_fill(itens, n - 1, W, peso_total);
    } else {
        peso_total += itens[n - 1].peso;
        return itens[n - 1].valor + prob_fill(itens, n - 1, W - itens[n - 1].peso, peso_total);
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
    vector<Item> itens(n);
    int peso_total = 0;
    for (int i = 0; i < n; i++) {
        cin >> itens[i].peso >> itens[i].valor;
    }

    write_output("Fill First: \n");

    for (int i = 0; i < 5; i++) {
        write_output("Execução " + to_string(i + 1) + ": ");
        peso_total = 0;
        // shuffle the vector with a random seed
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        vector<Item> shuffled_vector = itens;
        shuffle(shuffled_vector.begin(), shuffled_vector.end(), default_random_engine(seed));

        // Start the timer
        auto inicio = high_resolution_clock::now();

        int result = fill_first(shuffled_vector, n, W, peso_total);

        // Stop the timer
        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<milliseconds>(fim - inicio).count();

        string output = "Peso ocupado: " + to_string(peso_total) + " Valor alcançado: " + to_string(result) + " dinheiros" + "\n" + to_string(duracao) + "ms \n";
        write_output(output);
        cout << output << endl;
    }

    write_output("Probabilistic Fill: \n");
    for (int i = 0; i < 5; i++) {
        write_output("Execução " + to_string(i + 1) + ": ");
        peso_total = 0;

        // Start the timer
        auto inicio = high_resolution_clock::now();

        int result = prob_fill(itens, n, W, peso_total);

        // Stop the timer
        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<milliseconds>(fim - inicio).count();

        string output = "Peso ocupado: " + to_string(peso_total) + " Valor alcançado: " + to_string(result) + " dinheiros" + "\n" + to_string(duracao) + "ms \n";
        write_output(output);
        cout << output << endl;
    }
    return 0;
}