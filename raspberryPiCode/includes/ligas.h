#ifndef LIGAS_H
#define LIGAS_H

#include "stdbool.h"
typedef struct {
    char nome[25];
} Liga;

extern int tamanho_ligas;
extern int ligas_carregadas;
extern Liga* ligas;

void criar_liga(int tam);
bool redimensionar_liga(int novo_tamanho);

#endif
