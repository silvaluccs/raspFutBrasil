#include "formatar.h"
#include <stdio.h>

void formatar_placar(const Jogo *jogo, char *saida) {

  {

    if (jogo->placar_casa == -1) {

      sprintf(saida, "%.3s   x   %.3s", jogo->time_casa, jogo->time_fora);
      return;
    }

    sprintf(saida, "%.3s %d x %d %.3s", jogo->time_casa, jogo->placar_casa,
            jogo->placar_fora, jogo->time_fora);
  }
}
