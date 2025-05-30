#ifndef JOGO_DADOS_H
#define JOGO_DADOS_H

#include "stdbool.h"
typedef struct {
    char time_casa[10];
    char time_fora[10];
    char placar_casa[3];
    char placar_fora[3];
    char status[25];
    bool tem_dados;

} Jogo;

extern Jogo jogo;
extern Jogo* jogos;
extern int tamanho_array;
extern int total_jogos;
extern int index_dados;

#endif
