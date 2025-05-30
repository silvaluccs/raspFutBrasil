#ifndef SETUP_H
#define SETUP_H

#include "pico/stdlib.h"
#include <sys/types.h>

void setup_joystick(uint pino_vrx, uint pino_vry);
void setup_botoes(uint pino);
void setup_led(uint pino);

#endif
