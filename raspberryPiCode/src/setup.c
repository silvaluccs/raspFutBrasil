#include "hardware/adc.h"
#include "stdbool.h"
#include <pico/stdlib.h>

void setup_joystick(uint pino_vrx, uint pino_vry) {
  adc_init();
  adc_gpio_init(pino_vrx);
  adc_gpio_init(pino_vry);
}

void setup_led(uint pino) {
  gpio_init(pino);
  gpio_set_dir(pino, GPIO_OUT);
  gpio_put(pino, 0);
}

void setup_botoes(uint pino) {
  gpio_init(pino);
  gpio_set_dir(pino, GPIO_IN);
  gpio_pull_up(pino);
}
