#include "lwip/altcp_tls.h"
#include <lwip/apps/mqtt.h>
#include <lwip/apps/mqtt_priv.h>
#include <lwip/dns.h>
#include <pico/cyw43_arch.h>
#include <pico/multicore.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <stdio.h>
#include <string.h>

#ifndef MQTT_SERVER
#error Need to define MQTT_SERVER
#endif

#ifndef MQTT_DEVICE_NAME
#define MQTT_DEVICE_NAME "pico"
#endif

#ifndef MQTT_TOPIC_LEN
#define MQTT_TOPIC_LEN 100
#endif

#define WIFI_SSID "JUNIOR"
#define WIFI_PASSWORD "030424Eg"
#define MQTT_SERVER "192.168.0.107"
#define MQTT_USERNAME "user"
#define MQTT_PASSWORD "2005"

#define LED_PIN 12
#define LED_RED_PIN 13
#define LED_GREEN_PIN 11

typedef struct {
  mqtt_client_t *mqtt_client_inst;
  struct mqtt_connect_client_info_t mqtt_client_info;
  char topic[MQTT_TOPIC_LEN];
  char data[MQTT_TOPIC_LEN];
  ip_addr_t mqtt_server_address;
  bool connect_done;
} MQTT_CLIENT_DATA_T;

static void control_led(bool on) { gpio_put(LED_PIN, on); }

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len,
                                  u8_t flags) {
  MQTT_CLIENT_DATA_T *state = (MQTT_CLIENT_DATA_T *)arg;
  strncpy(state->data, (const char *)data, len);
  state->data[len] = '\0';

  if (strcmp(state->data, "on") == 0) {
    control_led(true);
    gpio_put(LED_RED_PIN, 0);
    gpio_put(LED_GREEN_PIN, 0);
    mqtt_publish(state->mqtt_client_inst, "/led/response",
                 "Comando on foi recebido", strlen("Comando on foi recebido"),
                 0, 0, NULL, NULL);
  } else if (strcmp(state->data, "off") == 0) {
    mqtt_publish(state->mqtt_client_inst, "/led/response",
                 "Comando off foi recebido", strlen("Comando off foi recebido"),
                 0, 0, NULL, NULL);
    control_led(false);
    gpio_put(LED_RED_PIN, 0);
    gpio_put(LED_GREEN_PIN, 0);
  } else if (strcmp(state->data, "white") == 0) {
    control_led(false);
    gpio_put(LED_RED_PIN, 1);
    gpio_put(LED_GREEN_PIN, 1);
    control_led(true);
    mqtt_publish(state->mqtt_client_inst, "/led/response",
                 "Comando white foi recebido",
                 strlen("Comando white foi recebido"), 0, 0, NULL, NULL);
  } else {
    mqtt_publish(state->mqtt_client_inst, "/led/response",
                 "Comando desconhecido", strlen("Comando desconhecido"), 0, 0,
                 NULL, NULL);
  }
}

static void mqtt_incoming_publish_cb(void *arg, const char *topic,
                                     u32_t tot_len) {
  MQTT_CLIENT_DATA_T *state = (MQTT_CLIENT_DATA_T *)arg;
  strncpy(state->topic, topic, sizeof(state->topic));
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg,
                               mqtt_connection_status_t status) {
  MQTT_CLIENT_DATA_T *state = (MQTT_CLIENT_DATA_T *)arg;
  if (status == MQTT_CONNECT_ACCEPTED) {
    state->connect_done = true;
    mqtt_sub_unsub(state->mqtt_client_inst, "/led", 1, NULL, state, 1);
    mqtt_publish(state->mqtt_client_inst, "/led/response", "Conectado ao MQTT",
                 strlen("Conectado ao MQTT"), 0, 0, NULL, NULL);
  }
}

static void start_client(MQTT_CLIENT_DATA_T *state) {
  state->mqtt_client_inst = mqtt_client_new();
  cyw43_arch_lwip_begin();
  mqtt_client_connect(state->mqtt_client_inst, &state->mqtt_server_address,
                      1883, mqtt_connection_cb, state,
                      &state->mqtt_client_info);
  mqtt_set_inpub_callback(state->mqtt_client_inst, mqtt_incoming_publish_cb,
                          mqtt_incoming_data_cb, state);
  cyw43_arch_lwip_end();
}

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
