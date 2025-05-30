#include "joystick.h"

void controle_joystick(int *cursor) {

  adc_select_input(1);

  uint16_t valor_eixo_x = adc_read();

  if (valor_eixo_x < 500 && *cursor > 0) {
    *cursor = *cursor - 1;
  }
  if (valor_eixo_x > 3800 && *cursor < 10) {
    *cursor = *cursor + 1;
  }
}
