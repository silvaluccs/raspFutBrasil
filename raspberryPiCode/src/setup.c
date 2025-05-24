#include "mqtt_connect.h"
#include "stdbool.h"
#include <pico/stdlib.h>

bool init_mqtt() {

  if (cyw43_arch_init()) {
    printf("Erro ao inicializar o Wi-Fi\n");

    return false;
  }

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
}
