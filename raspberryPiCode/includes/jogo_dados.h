#ifndef JOGO_DADOS_H
#define JOGO_DADOS_H

typedef struct {
    char time_casa[10];
    char time_fora[10];
    char placar_casa[3];
    char placar_fora[3];
    char tempo[6]; // ex: "78'", "FT", "HT"
    char status[20];
} Jogo;

extern Jogo jogo;

#endif
