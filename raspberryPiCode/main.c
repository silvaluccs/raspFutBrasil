#include "FreeRTOS.h"
#include "font.h"
#include "hardware/i2c.h"
#include "mqtt_connect.h"
#include "pico/stdlib.h"
#include "ssd1306.h"
#include "task.h"
#include <stdio.h>

#define led_pin_red 12
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

typedef struct {
  char time_casa[10];
  char time_fora[10];
  char placar_casa[3];
  char placar_fora[3];
  char tempo[6]; // ex: "78'", "FT", "HT"
} Jogo;

void formatar_placar(const Jogo *jogo, char *saida) {
  // Exemplo: "ATL 2 x 1 FLU"
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
  while (true) {
    ssd1306_fill(&ssd, !cor);
    ssd1306_draw_string(&ssd, "ATL 2 x 1 FLU", 10, 13);
    ssd1306_draw_string(&ssd, "TEMPO: 78'", 19, 25);
    ssd1306_send_data(&ssd);
    sleep_ms(1000);
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

  if (cyw43_arch_init()) {
    printf("Erro ao inicializar o Wi-Fi\n");

    return 1;
  }

  //
  printf("MQTT Client Example\n");
  printf("Connecting to %s\n", WIFI_SSID);
  printf("MQTT Server: %s\n", MQTT_SERVER);
  printf("Device Name: %s\n", MQTT_USERNAME);
  printf("LED Pin: %d\n", LED_PIN);

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_put(LED_PIN, 0);

  gpio_init(LED_RED_PIN);
  gpio_set_dir(LED_RED_PIN, GPIO_OUT);
  gpio_put(LED_RED_PIN, 0);

  gpio_init(LED_GREEN_PIN);
  gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
  gpio_put(LED_GREEN_PIN, 0);

  static MQTT_CLIENT_DATA_T state = {0};
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
