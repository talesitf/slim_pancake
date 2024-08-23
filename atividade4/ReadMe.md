# Relatório aula 4

## 1. Introdução

Neste relatório, será apresentado o desenvolvimento da atividade 3.2, onde será implementado:

- Passagem de parãmetros por referência vs. passagem de parâmetros por ponteiro.
- Utilização do modificador const para imutabilidade de variáveis.
- Alocação de memória dinâmica ( manual vs. automatização via bibliotecas como std::vector).
- Manipulação de vetores

## 2. Exercícios

### Exercício 1 - Passagem de Parâmetros e Manipulação de Arquivos

[ex1](/atividade4/arquive_to_Vector.cpp)

Neste exercício, utilizamos amabas as formas de passagem de parâmetros, por referência e por ponteiro, para manipular um arquivo de texto. O arquivo de texto contém uma lista de números inteiros, onde o objetivo é ler o arquivo, armazenar os números em um vetor e dobrar os números. O resultado da operação é armazenado em uma variável passada por referência ou por ponteiros. Em seguida, o vetor resultante é escrito em um novo arquivo.

[Arquivo de entrada](/atividade4/entrada.txt)

[Arquivo de saída por referencia](/atividade4/saida_referencia.txt)

Devido à mesma variável ser passada em ambas as funções e ser utilizado o termo 'push_back' para escrita nos vetores, o arquivo de saída por ponteiro tem dobro de linhas em relação ao arquivo de saída por referência.

### Exercício 2 - Const Correctness

[ex2](/atividade4/mult_matrix.cpp)

Neste exercício, implementamos uma função que multiplica duas matrizes. A função recebe duas matrizes e um vetor de saída. Para garantir que as matrizes de entrada não sejam alteradas, utilizamos o modificador const. A função é implementada de forma que a multiplicação das matrizes seja feita de forma eficiente, sem a necessidade de percorrer todas as linhas e colunas.

### Exercício 3 - Alocação Dinâmica de Memória

[ex3 sem biblioteca](/atividade4/matriz_dinamica.cpp)

[ex3 com biblioteca](/atividade4/matriz_dinamica_vector.cpp)

Neste exercício, implementamos uma matriz de tamanho variável. A matriz é alocada dinamicamente e o tamanho é definido pelo usuário. Implementamos a alocação de memória de duas formas: manualmente e utilizando a biblioteca std::vector. A implementação com a biblioteca std::vector é mais simples e segura, pois a alocação e desalocação de memória são feitas de forma automática. Porém, ao utilizar a biblioteca para alocação de memória, há uma leve perda de desempenho em relação à alocação manual.

## Exercício 4 - Manipulação de Vetores

[ex4](/atividade4/VECTOR.cpp)

Para finalizar os exercícios da ativiade 4, implementamos uma clase que simula um vetor. A classe possui métodos para adicionar elementos, remover elementos, acessar elementos e imprimir o vetor. A classe é implementada de forma que a alocação de memória seja feita de forma dinâmica, garantindo que o vetor possa crescer conforme necessário. Além disso, a classe é implementada de forma que a memória alocada seja desalocada corretamente, evitando vazamento de memória.

Além disso, foi interessante utilizar o comando inline para a implementação de métodos da classe, melhorando a eficiência do código ao evitar chamadas de memória desnecessárias.

## 3. Conclusão

O desenvolvimento dessa atividade foi importante para entender as diferentes forças do C++ em relação a alocação de memória e passagem de parâmetros. A alocação de memória dinâmica é uma ferramenta poderosa, mas deve ser utilizada com cuidado para evitar vazamento de memória. Além disso, a passagem de parâmetros por referência e por ponteiro é uma forma eficiente de evitar a cópia desnecessária de variáveis, mas deve ser utilizada com cuidado para garantir a integridade dos dados.

ALém disso, foi possível conhecer novas formas de POO, como a implementação de classes e métodos inline, que podem melhorar a eficiência do código e facilitar a manutenção do mesmo.

###### Observação: O nome em maiúsculo do arquivo VECTOR.cpp foi proposital para referência a um vilão de filme de criança.
