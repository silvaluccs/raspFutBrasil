#include "mqtt_connect.h"
#include "jogo_dados.h"
#include <lwip/apps/mqtt.h>
#include <stdio.h>

void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
  MQTT_CLIENT_DATA_T *state = (MQTT_CLIENT_DATA_T *)arg;

  strncpy(state->data, (const char *)data, len);
  state->data[len] = '\0';

  printf("Tópico: %s\n", state->topic);

  if (strcmp(state->topic, "/jogo/time_casa") == 0) {
    strncpy(jogo.time_casa, state->data, sizeof(jogo.time_casa));
  }
  if (strcmp(state->topic, "/jogo/time_fora") == 0) {
    strncpy(jogo.time_fora, state->data, sizeof(jogo.time_fora));
  }

  if (strcmp(state->topic, "/jogo/placar_casa") == 0) {
    strncpy(jogo.placar_casa, state->data, sizeof(jogo.placar_casa));
  }

  if (strcmp(state->topic, "/jogo/placar_fora") == 0) {
    strncpy(jogo.placar_fora, state->data, sizeof(jogo.placar_fora));
  }

  if (strcmp(state->topic, "/jogo/tempo") == 0) {
    strncpy(jogo.tempo, state->data, sizeof(jogo.tempo));
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

    mqtt_sub_unsub(state->mqtt_client_inst, "/jogo/time_casa", 1, NULL, state,
                   1);
    mqtt_sub_unsub(state->mqtt_client_inst, "/jogo/time_fora", 1, NULL, state,
                   1);
    mqtt_sub_unsub(state->mqtt_client_inst, "/jogo/placar_casa", 1, NULL, state,
                   1);
    mqtt_sub_unsub(state->mqtt_client_inst, "/jogo/placar_fora", 1, NULL, state,
                   1);
    mqtt_sub_unsub(state->mqtt_client_inst, "/jogo/tempo", 1, NULL, state, 1);

    // Publica uma mensagem indicando que está conectado
    mqtt_publish(state->mqtt_client_inst, "/log", "Conectado ao MQTT",
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
