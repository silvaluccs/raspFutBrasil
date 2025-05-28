#ifndef JOGO_DADOS_H
#define JOGO_DADOS_H

#include "stdbool.h"
#define MAXIMO_JOGOS 9
typedef struct {
    char time_casa[10];
    char time_fora[10];
    char placar_casa[3];
    char placar_fora[3];
    char status[25];
    bool tem_dados;

} Jogo;

extern Jogo jogo;
extern Jogo jogos[MAXIMO_JOGOS];
extern int total_jogos;
extern int index_dados;

#endif
