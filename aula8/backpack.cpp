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
    int id;
    bool inSolution;
};

void prob_fill(vector<Item> &itens, int n, int &W) {
    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<double> distribution(0.0, 1.0);

    double prob = distribution(generator);

    for (int i = 0; i < n; i++) {
        if (itens[i].peso <= W && prob < 0.2) {
            itens[i].inSolution = true;
            W -= itens[i].peso;
            cout << "Item " << itens[i].id << " adicionado" << endl;
            prob = distribution(generator);
        } else {
            prob = distribution(generator);
        }
    }
}

void full_backpack(vector<Item> &itens, int n, int &W) {
    cout << "Preenchendo mochila" << endl;
    for (int i = 0; i < n; i++) {
        if (itens[i].peso <= W && itens[i].inSolution == false) {
            itens[i].inSolution = true;
            W -= itens[i].peso;
            cout << "Item " << itens[i].id << " adicionado" << endl;
        }
    }
}

bool try_substitute(vector<Item> &itens, int n, int &W) {
    for (int i = 0; i < n; i++) {
        if (!itens[i].inSolution) {
            Item newItem = itens[i];
            for (int j = 0; j < n; j++) {
                if (itens[j].inSolution) {
                    Item oldItem = itens[j];
                    if (newItem.valor > oldItem.valor && W + oldItem.peso > newItem.peso) {
                        itens[j].inSolution = false;
                        itens[i].inSolution = true;
                        W += itens[j].peso;
                        W -= itens[i].peso;
                        cout << "Item " << itens[i].id << " adicionado" << endl;
                        cout << "Item " << itens[j].id << " removido" << endl;
                        full_backpack(itens, n, W);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void subsstitute(vector<Item> &itens, int n, int &W) {
    cout << "Substituindo itens" << endl;

    bool canChange = true;
    while (canChange) {
        canChange = try_substitute(itens, n, W);
    }
}

void hill_climbing(vector<Item> &itens, int n, int &W) {
    cout << "Hill Climbing" << endl;
}

void write_output(vector<Item> &itens, int n) {
    int W = 0;
    int totalValue = 0;

    for (int i = 0; i < n; i++) {
        if (itens[i].inSolution) {
            W += itens[i].peso;
            totalValue += itens[i].valor;
        }
    }

    cout << W << " " << totalValue << endl;
}

int main() {
    int n, W;
    cin >> n >> W;

    cout << "n: " << n << " W: " << W << endl;
    vector<Item> itens(n);

    for (int i = 0; i < n; i++) {
        cin >> itens[i].peso >> itens[i].valor;
        itens[i].id = i;
        itens[i].inSolution = false;
    }

    auto start = high_resolution_clock::now();
    prob_fill(itens, n, W);
    full_backpack(itens, n, W);
    subsstitute(itens, n, W);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Tempo: " << duration.count() << " microsegundos" << endl;

    write_output(itens, n);

    return 0;
}