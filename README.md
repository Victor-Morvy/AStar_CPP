# AStar_CPP
Algorítmo IA - A*(star) pathfinding em CPP

![alt tag](https://github.com/Victor-Morvy/AStar_CPP/blob/main/image.png?raw=true)

## Introdução
Utilizando estudos, foi desenvolvido um algorítmo A*(star) pathfinding, vistos geralmente em jogos, onde o objetivo é achar o caminho mais rápido de A para B, mesmo com obstáculos, utiliza uma busca em forma de grafo do caminho mais curto entre dois pontos.
E com a bibliotéca gráfica PIG: https://github.com/PIGDevUff/PigDev, facilitar a leitura para o usuário.

## Objetivo
Através da biblioteca gráfica PIG, demonstrar valores de forma que o usuário leia com facilidade os resultados.

## Como utilizar
Copie o código da biblioteca gráfica PIG, e substitua as pastas 'src', abra o arquivo do code::blocks referente ao seu SO.
- W, A, S, D - Move o ponto A no campo;
- setas - Move o ponto B no campo;

## Como mudar o campo
Procure por "campo[TAMYCAMPO][TAMXCAMPO]" no main.cpp, você vai encontrar o campo:
```
{{0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
 {0, 0, 0, 0, 0, 1, 1, 1, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 2, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
```
Onde:
- 0 é caminho procurável;
- 1 é o caminho bloqueado (paredes);
- 2 é o ponto A;
- 3 é o ponto B;
- TAMYCAMPO é o tamanho do campo em y;
- TAMXCAMPO é o tamanho do campo em x;

`A procura por B;`
