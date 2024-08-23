#include <iostream>
#include <vector>

using namespace std;

class Vector {
public:
    Vector(int tamanho);
    ~Vector();
    void inicializa(int valor);
    inline int get(int index) const;
    inline void set(int index, int valor);
    void inserir(int index, int valor);
    void remover(int index);
    void imprime() const;
    void multiplicarPorEscalar(int escalar);
    void somarVetor(const Vector &v);
    void subtrairVetor(const Vector &v);

private:
    int *dados;
    int tam;
    int capacidade;
    void redimensiona(int novaCapacidade);
};

// Implementação das funções...
Vector::Vector(int tamanho) : tam(0), capacidade(tamanho), dados(new int[tamanho]) {}

Vector::~Vector() {
    delete[] dados; // Libera a memória alocada
}

void Vector::inicializa(int valor) {
    cout << "Inicializando..." << endl;
    for (int i = 0; i < capacidade; i++) {
        dados[i] = valor;
    }
    tam = capacidade;
}

inline int Vector::get(int index) const {
    return dados[index];
}

inline void Vector::set(int index, int valor) {
    dados[index] = valor;
}

void Vector::inserir(int index, int valor) {
    if (tam == capacidade) {
        Vector::redimensiona(capacidade * 2);
    }
    for (int i = tam; i > index; i--) {
        dados[i] = dados[i - 1];
    }
    dados[index] = valor;
    tam++;
}

void Vector::remover(int index) {
    for (int i = index; i < tam - 1; i++) {
        dados[i] = dados[i + 1];
    }
    tam--;
    if (tam < capacidade / 4) {
        Vector::redimensiona(capacidade / 2);
    }
}

void Vector::imprime() const {
    cout << "Imprimindo..." << endl;
    for (int i = 0; i < tam; i++) {
        cout << dados[i] << " ";
    }
    cout << endl;
}

void Vector::redimensiona(int novaCapacidade) {
    int *novo = new int[novaCapacidade];
    for (int i = 0; i < tam; i++) {
        novo[i] = dados[i];
    }
    delete[] dados;
    dados = novo;
    capacidade = novaCapacidade;
}

void Vector::multiplicarPorEscalar(int escalar) {
    for (int i = 0; i < tam; i++) {
        dados[i] *= escalar;
    }
}

void Vector::somarVetor(const Vector &v) {
    for (int i = 0; i < tam; i++) {
        dados[i] += v.dados[i];
    }
}

void Vector::subtrairVetor(const Vector &v) {
    for (int i = 0; i < tam; i++) {
        dados[i] -= v.dados[i];
    }
}

int main() {

    cout << "Eu sou... VECTOR!!!!" << endl;

    Vector vec(5);
    vec.inicializa(0);
    vec.imprime();

    return 0;
}