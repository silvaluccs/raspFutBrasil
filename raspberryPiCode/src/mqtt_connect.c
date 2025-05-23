#include "mqtt_connect.h"

void control_led(bool on) { gpio_put(LED_PIN, on); }

void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
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

void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {
  MQTT_CLIENT_DATA_T *state = (MQTT_CLIENT_DATA_T *)arg;
  strncpy(state->topic, topic, sizeof(state->topic));
}

void mqtt_connection_cb(mqtt_client_t *client, void *arg,
                        mqtt_connection_status_t status) {
  MQTT_CLIENT_DATA_T *state = (MQTT_CLIENT_DATA_T *)arg;
  if (status == MQTT_CONNECT_ACCEPTED) {
    state->connect_done = true;
    mqtt_sub_unsub(state->mqtt_client_inst, "/led", 1, NULL, state, 1);
    mqtt_publish(state->mqtt_client_inst, "/led/response", "Conectado ao MQTT",
                 strlen("Conectado ao MQTT"), 0, 0, NULL, NULL);
  }
}

void start_client(MQTT_CLIENT_DATA_T *state) {
  state->mqtt_client_inst = mqtt_client_new();
  cyw43_arch_lwip_begin();
  mqtt_client_connect(state->mqtt_client_inst, &state->mqtt_server_address,
                      1883, mqtt_connection_cb, state,
                      &state->mqtt_client_info);
  mqtt_set_inpub_callback(state->mqtt_client_inst, mqtt_incoming_publish_cb,
                          mqtt_incoming_data_cb, state);
  cyw43_arch_lwip_end();
}
