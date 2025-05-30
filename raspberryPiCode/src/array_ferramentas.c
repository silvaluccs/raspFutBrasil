#include "array_ferramentas.h"

void criar_array(int tam) {

  jogos = malloc(tam * sizeof(Jogo));
  if (jogos == NULL) {

    fprintf(stderr, "Erro ao alocar memória.\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < tam; i++) {
    jogos[i].tem_dados = false;
  }

  tamanho_array = tam;
}

bool redimensionar_array(int novo_tamanho) {

  if (novo_tamanho == tamanho_array) {
    return 1; // não precisa redimensionar
  }

  Jogo *temp = realloc(jogos, novo_tamanho * sizeof(int));
  if (temp == NULL && novo_tamanho > 0) {
    return 0; // falha
  }

  jogos = temp;
  tamanho_array = novo_tamanho;
  return 1; // sucesso
}
