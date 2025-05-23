#include "mqtt_connect.h"

int main(void) {
  stdio_init_all();
  sleep_ms(5000);

  // Inicializa o Wi-Fi
  if (cyw43_arch_init()) {
    printf("Erro ao inicializar o Wi-Fi\n");

    return 1;
  }

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

  while (true) {
    cyw43_arch_poll();
  }

  return 0;
}
