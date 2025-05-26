#include "debouce.h"

bool debouce(uint32_t *ultimo_tempo) {
  // fazendo o debouce
  uint32_t tempo_atual = to_us_since_boot(get_absolute_time());

  bool debouce = tempo_atual - *ultimo_tempo > (200 * 1000);

  if (!debouce) { // caso nao tenha passado o intervalo ideal entre
                  // pressionar os botoes
    return false;
  }

  *ultimo_tempo = tempo_atual; // atualizando o tempo
  return true;
}
