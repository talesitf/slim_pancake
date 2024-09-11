# Problema da Mochila Cheia: Comparação entre Heurísticas

## Heurísticas

Você está utilizando duas heurísticas principais no problema da mochila cheia:

- **A do mais valioso** (implementada em `bigger_first`)
- **A do mais leve** (implementada em `smaller_first`)

A seguir, vou responder às suas perguntas de maneira detalhada.

---

## 1. Em quais situações a heurística do mais valioso é melhor?

A heurística do mais valioso se destaca em situações onde:

- Os **itens têm grandes diferenças de valor**, ou seja, alguns itens são extremamente valiosos e vale a pena incluí-los, mesmo que ocupem uma boa parte da capacidade da mochila.
- A **capacidade da mochila** é suficiente para carregar poucos itens de valor elevado, e não é necessário tentar preencher a mochila com muitos itens de valor moderado.

Esta abordagem é ideal quando os **itens valiosos são escassos**, mas seu valor compensa o peso que ocupam.

---

## 2. Em quais situações a heurística do mais leve é melhor?

A heurística do mais leve é eficiente em casos onde:

- Os itens têm **pesos baixos** e é possível incluir muitos itens na mochila, mesmo que os valores individuais não sejam muito altos.
- A capacidade da mochila é restrita, então priorizar itens leves maximiza a quantidade de itens dentro da mochila.

Se houver **vários itens leves com valores razoáveis**, esta abordagem tende a ser a mais eficaz.

---

## 3. Qual a complexidade computacional das abordagens?

Ambas as heurísticas seguem um padrão semelhante:

- **Ordenação**: Seja ordenando os itens por valor ou por peso, a complexidade da ordenação é de **O(n log n)**, onde \(n\) é o número de itens.
- **Inserção na mochila**: Após a ordenação, o processo de inserir os itens tem complexidade de **O(n)**.

Logo, a complexidade total de ambas as abordagens é de **O(n log n)**.

---

## 4. Quando uma é melhor que a outra?

- A **heurística do mais valioso** é melhor quando há **poucos itens muito valiosos**, e a capacidade da mochila permite carregar itens de maior peso.
  
- A **heurística do mais leve** é mais indicada quando existem **muitos itens pequenos**, permitindo preencher a mochila com um grande número de itens, maximizando o valor total carregado.

---

## 5. Alguma das heurísticas consegue obter o melhor valor possível?

Nenhuma das duas heurísticas garante a solução ótima, pois ambas são **aproximações gananciosas** que não exploram todas as combinações possíveis de itens.

- A heurística do mais valioso pode falhar se a melhor solução exigir a combinação de vários itens de menor valor.
- A heurística do mais leve pode falhar se houver poucos itens valiosos que compensariam carregar menos peso.

No entanto, essas heurísticas são úteis para **soluções rápidas** e práticas quando o número de itens é grande e o tempo é uma restrição.

---

## Arquivos Referenciados

- **Entrada para os itens mais valiosos**: `IO_files/in_BagRich.txt`
- **Entrada para os itens mais leves**: `IO_files/in_BagLight.txt`
- **Saída com resultados das heurísticas**: `IO_files/output.txt`

Esses arquivos foram utilizados para armazenar os itens gerados aleatoriamente e os resultados das heurísticas, como tempo de execução, peso total e valor total.
