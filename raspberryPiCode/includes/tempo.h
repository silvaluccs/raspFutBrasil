#ifndef TEMPO_H
#define TEMPO_H

#define max 1

typedef struct {
    int index;
    int tempo_minutos;
    char data_partida[15];
    char horario_partida[10];
} TEMPO_T;

extern TEMPO_T tempos[max];
extern int tamanho_tempo;

#endif
