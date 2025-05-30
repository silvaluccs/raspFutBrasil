#include "FreeRTOS.h"
#include "debouce.h"
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

#define PINO_BOTAO_B 6
#define PINO_BOTAO_A 5

Jogo jogo;
static MQTT_CLIENT_DATA_T state = {0};
MENUS menu;

static uint32_t ultimo_tempo = 0;

int cursor = 0;

void gpio_irq_handler(uint gpio, uint32_t events) {

  if (!debouce(&ultimo_tempo)) {
    return;
  }

  char buffer[20];
  if (gpio == PINO_BOTAO_B) {
    if (menu == PARTIDA) {
      return;
    }

    total_jogos = 0;
    menu = PARTIDA;

    strcpy(jogo.status, jogos[cursor].status);
    strcpy(jogo.time_casa, jogos[cursor].time_casa);
    strcpy(jogo.time_fora, jogos[cursor].time_fora);
    strcpy(jogo.placar_casa, jogos[cursor].placar_casa);
    strcpy(jogo.placar_fora, jogos[cursor].placar_fora);

    sprintf(buffer, "%d", cursor);
    mqtt_publish(state.mqtt_client_inst, "/dados_tempo", buffer, strlen(buffer),
                 0, 100, NULL, NULL);
    return;
  }

  if (gpio == PINO_BOTAO_A) {
    if (menu == PARTIDAS) {

      return;
    }

    tamanho_tempo = 0;
    menu = PARTIDAS;
    sprintf(buffer, "%d", index_dados);

    mqtt_publish(state.mqtt_client_inst, "/dados", buffer, strlen(buffer), 0,
                 100, NULL, NULL);
    return;
  }
}

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

    if (total_jogos < 8 && menu == PARTIDAS) {

      int iterador = 0;
      while (total_jogos < 8) {
        char str[50] = "carregando [";
        int pontos = iterador % 3;

        for (int j = 0; j < pontos; j++) {
          strcat(str, ".");
        }

        strcat(str, "]   ");

        ssd1306_draw_string(&ssd, str, 5, 20);

        ssd1306_send_data(&ssd);
        iterador++;
      }

      continue;
    }

    if (menu == PARTIDA) {

      int iterador = 0;
      while (tamanho_tempo == 0) {
        char str[50] = "carregando [";
        int pontos = iterador % 3;

        for (int j = 0; j < pontos; j++) {
          strcat(str, ".");
        }

        strcat(str, "]   ");

        ssd1306_draw_string(&ssd, str, 5, 20);

        ssd1306_send_data(&ssd);
        iterador++;
      }
      ssd1306_fill(&ssd, !cor);

      formatar_placar(&jogos[cursor], buffer);
      ssd1306_draw_string(&ssd, buffer, 10, 10);

      if (strcmp(jogos[cursor].status, "Fim") == 0) {
        ssd1306_draw_string(&ssd, tempos[0].horario_partida, 42, 30);
        ssd1306_draw_string(&ssd, tempos[0].data_partida, 24, 40);
      } else if (tempos[0].tempo_minutos == -1) {
        ssd1306_draw_string(&ssd, tempos[0].horario_partida, 42, 30);
        ssd1306_draw_string(&ssd, tempos[0].data_partida, 24, 40);
      } else if (tempos[0].tempo_minutos > -1) {
        snprintf(buffer, sizeof(buffer), "%d'", tempos[0].tempo_minutos);
        ssd1306_draw_string(&ssd, buffer, 42, 30);
      }

      ssd1306_send_data(&ssd);
    }

    if (menu == PARTIDAS) {
      ssd1306_fill(&ssd, !cor);

      formatar_placar(&jogos[cursor], buffer);
      ssd1306_draw_string(&ssd, buffer, 10, 10);
      ssd1306_draw_string(&ssd, jogos[cursor].status, 45, 30);
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
  strcpy(jogo.status, "esperando");

  menu = PARTIDAS;
  setup_botoes(PINO_BOTAO_A);
  setup_botoes(PINO_BOTAO_B);

  gpio_set_irq_enabled_with_callback(PINO_BOTAO_A, GPIO_IRQ_EDGE_FALL, true,
                                     &gpio_irq_handler); // botao A
  gpio_set_irq_enabled_with_callback(PINO_BOTAO_B, GPIO_IRQ_EDGE_FALL, true,
                                     &gpio_irq_handler); // botao B

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
