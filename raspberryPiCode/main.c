#include "FreeRTOS.h"
#include "font.h"
#include "formatar.h"
#include "hardware/i2c.h"
#include "jogo_dados.h"
#include "joystick.h"
#include "menus.h"
#include "mqtt_connect.h"
#include "pico/stdlib.h"
#include "queue.h"
#include "setup.h"
#include "ssd1306.h"
#include "task.h"
#include "tempo.h"
#include <lwip/apps/mqtt.h>
#include <stdio.h>

#define led_pin_verde 11
#define led_pin_red 13
#define led_pin_azul 12
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

#define PINO_JOYSTICK_X 27
#define PINO_JOYSTICK_Y 26

Jogo jogo;
static MQTT_CLIENT_DATA_T state = {0};
MENUS menu;

int cursor = 0;

void vDisplayTask() {
  i2c_init(I2C_PORT, 100 * 1000);
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(I2C_SDA);
  gpio_pull_up(I2C_SCL);

  ssd1306_t ssd;
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
  ssd1306_config(&ssd);
  ssd1306_send_data(&ssd);
  setup_led(led_pin_verde);
  setup_led(led_pin_red);
  setup_led(led_pin_azul);

  bool cor = true;

  char buffer[20];

  mqtt_publish(state.mqtt_client_inst, "/dados", "0", strlen("0"), 0, 100, NULL,
               NULL);

  while (true) {

    ssd1306_fill(&ssd, !cor);

    if (total_jogos < 8) {
      ssd1306_draw_string(&ssd, "esperando jogos", 5, 20);
      ssd1306_send_data(&ssd);
      continue;
    }

    if (menu == PARTIDA) {

      if (tamanho_tempo == 0) {
        ssd1306_draw_string(&ssd, "sem dados", 5, 20);
        ssd1306_send_data(&ssd);
        continue;
      }

      ssd1306_draw_string(&ssd, tempos[0].data_partida, 5, 20);
      ssd1306_draw_string(&ssd, tempos[0].tempo_minutos, 5, 30);
      ssd1306_draw_string(&ssd, tempos[0].horario_partida, 5, 40);
      ssd1306_send_data(&ssd);
    }

    if (menu == PARTIDAS) {
      formatar_placar(&jogos[cursor], buffer);
      ssd1306_draw_string(&ssd, buffer, 10, 10);
      ssd1306_draw_string(&ssd, jogos[cursor].status, 45, 30);
      if (strcmp(jogos[cursor].status, "Fim") == 0) {
      } else if (strcmp(jogos[cursor].status, "Prog.") == 0) {
      }
      ssd1306_send_data(&ssd);
    }

    sleep_ms(1000);
  }
}

bool repeating_timer_callback_joystick(struct repeating_timer *t) {

  controle_joystick(&cursor);
  return true;
}

void vMqttTask() {

  while (true) {
    cyw43_arch_poll();
  }
}

int main() {
  // TODO: modulazirar a inicialização do wifi
  stdio_init_all();

  strcpy(jogo.time_casa, "0");
  strcpy(jogo.time_fora, "0");
  strcpy(jogo.placar_casa, "0");
  strcpy(jogo.placar_fora, "0");
  strcpy(jogo.tempo, "0");
  strcpy(jogo.status, "esperando");

  menu = PARTIDA;

  if (cyw43_arch_init()) {
    printf("Erro ao inicializar o Wi-Fi\n");

    return 1;
  }

  state.mqtt_client_info.client_id = MQTT_DEVICE_NAME;

  cyw43_arch_enable_sta_mode();

  if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD,
                                         CYW43_AUTH_WPA2_AES_PSK, 100000)) {
    printf("Falha ao conectar ao Wi-Fi\n");
    return 1;
  } else {
    printf("Connected.\n");
    // Read the ip address in a human readable way
    uint8_t *ip_address = (uint8_t *)&(cyw43_state.netif[0].ip_addr.addr);
    printf("Endereço IP %d.%d.%d.%d\n", ip_address[0], ip_address[1],
           ip_address[2], ip_address[3]);
  }

  cyw43_arch_lwip_begin();
  int err =
      dns_gethostbyname(MQTT_SERVER, &state.mqtt_server_address, NULL, &state);
  cyw43_arch_lwip_end();
  if (err == ERR_OK) {
    start_client(&state);
  }

  setup_joystick(PINO_JOYSTICK_X, PINO_JOYSTICK_Y);
  struct repeating_timer timer_joystick;
  add_repeating_timer_ms(100, repeating_timer_callback_joystick, NULL,
                         &timer_joystick);
  //  xTaskCreate(vBlinkTask, "Blink Task", configMINIMAL_STACK_SIZE, NULL,
  //              tskIDLE_PRIORITY, NULL);

  xTaskCreate(vMqttTask, "MQTT Task", 512, NULL, 1, NULL);

  xTaskCreate(vDisplayTask, "Display Task", 512, NULL, 1, NULL);

  vTaskStartScheduler();
  panic_unsupported();
}
