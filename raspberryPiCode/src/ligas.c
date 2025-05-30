#include "ligas.h"
#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>

void criar_liga(int tam) {

  ligas = malloc(tam * sizeof(Liga));
  if (ligas == NULL) {

    fprintf(stderr, "Erro ao alocar memória.\n");
    exit(EXIT_FAILURE);
  }

  tamanho_ligas = tam;
}

bool redimensionar_liga(int novo_tamanho) {

  if (novo_tamanho == tamanho_ligas) {
    return 1; // não precisa redimensionar
  }

  Liga *temp = realloc(ligas, novo_tamanho * sizeof(int));
  if (temp == NULL && novo_tamanho > 0) {
    return 0; // falha
  }

  ligas = temp;
  tamanho_ligas = novo_tamanho;
  return 1; // sucesso
}
