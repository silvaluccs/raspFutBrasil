#ifndef TEMPO_H
#define TEMPO_H

#define max 2

typedef struct {
    char tempo_minutos[8];
    char data_partida[15];
    char horario_partida[10];
} TEMPO_T;

extern TEMPO_T tempos[max];
extern int tamanho_tempo;

#endif
