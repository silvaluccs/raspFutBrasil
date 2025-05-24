#include "FreeRTOS.h"
#include "font.h"
#include "hardware/i2c.h"
#include "jogo_dados.h"
#include "mqtt_connect.h"
#include "pico/stdlib.h"
#include "ssd1306.h"
#include "task.h"
#include <lwip/apps/mqtt.h>
#include <stdio.h>

#define led_pin_red 12
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

Jogo jogo;
static MQTT_CLIENT_DATA_T state = {0};

void formatar_placar(const Jogo *jogo, char *saida) {

  if (strcmp(jogo->time_casa, "0") == 0 && strcmp(jogo->time_fora, "0") == 0 &&
      strcmp(jogo->placar_casa, "0") == 0 &&
      strcmp(jogo->placar_fora, "0") == 0 && strcmp(jogo->tempo, "0") == 0) {
    sprintf(saida, "empty");
    return;
  }

  sprintf(saida, "%.3s %s x %s %.3s", jogo->time_casa, jogo->placar_casa,
          jogo->placar_fora, jogo->time_fora);
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

  bool cor = true;

  char buffer[20];
  char buffer2[20];

  while (true) {
    ssd1306_fill(&ssd, !cor);

    formatar_placar(&jogo, buffer);

    if (strcmp(buffer, "empty") == 0) {
      ssd1306_draw_string(&ssd, "Esperando jogo", 0, 25);

      ssd1306_send_data(&ssd);
      // Publica uma mensagem indicando que está conectado
      mqtt_publish(state.mqtt_client_inst, "/dados", "esperando jogo",
                   strlen("esperando jogo"), 1, 10, NULL, NULL);

      sleep_ms(10000);
      continue;
    }
    ssd1306_draw_string(&ssd, buffer, 0, 10);

    snprintf(buffer2, sizeof(buffer2), "Tempo: %s'", jogo.tempo);
    ssd1306_draw_string(&ssd, buffer2, 0, 25);
    printf("Status: %s\n", jogo.status);
    ssd1306_send_data(&ssd);
  }
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
  strcpy(jogo.status, "0");

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

  //  xTaskCreate(vBlinkTask, "Blink Task", configMINIMAL_STACK_SIZE, NULL,
  //              tskIDLE_PRIORITY, NULL);
  xTaskCreate(vMqttTask, "MQTT Task", 512, NULL, 1, NULL);

  xTaskCreate(vDisplayTask, "Display Task", 512, NULL, 1, NULL);

  vTaskStartScheduler();
  panic_unsupported();
}
