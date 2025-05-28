#include "mqtt_connect.h"
#include "cJSON.h"
#include "jogo_dados.h"
#include "tempo.h"
#include <lwip/apps/mqtt.h>
#include <stdio.h>

Jogo jogos[MAXIMO_JOGOS];

TEMPO_T tempos[max];

int tamanho_tempo = 0;

int index_dados = 0;
int total_jogos = 0;
char buffer[10];

void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
  MQTT_CLIENT_DATA_T *state = (MQTT_CLIENT_DATA_T *)arg;

  strncpy(state->data, (const char *)data, len);
  state->data[len] = '\0';

  if (strcmp(state->topic, "/tempo_jogo") == 0) {

    cJSON *jsonDados = cJSON_Parse(state->data);

    cJSON *tempo = cJSON_GetObjectItemCaseSensitive(jsonDados, "tempo_partida");
    cJSON *horario =
        cJSON_GetObjectItemCaseSensitive(jsonDados, "horario_partida");

    cJSON *data_da_partida =
        cJSON_GetObjectItemCaseSensitive(jsonDados, "data_partida");

    tempos[tamanho_tempo].tempo_minutos = tempo->valueint;
    strcpy(tempos[tamanho_tempo].horario_partida, horario->valuestring);
    strcpy(tempos[tamanho_tempo].data_partida, data_da_partida->valuestring);
    ++tamanho_tempo;

    mqtt_publish(state->mqtt_client_inst, "/log", "chegou dados tempo",
                 strlen("chegou dados tempo"), 0, 100, NULL, NULL);
    return;
  }

  if (strcmp(state->topic, "/jogos") == 0) {

    cJSON *jsonData = cJSON_Parse(state->data);

    mqtt_publish(state->mqtt_client_inst, "/log", "chegou", strlen("chegou"), 0,
                 100, NULL, NULL);

    cJSON *status = cJSON_GetObjectItemCaseSensitive(jsonData, "status");
    cJSON *time_casa = cJSON_GetObjectItemCaseSensitive(jsonData, "time_casa");
    cJSON *time_fora = cJSON_GetObjectItemCaseSensitive(jsonData, "time_fora");
    cJSON *placar_casa =
        cJSON_GetObjectItemCaseSensitive(jsonData, "placar_casa");
    cJSON *placar_fora =
        cJSON_GetObjectItemCaseSensitive(jsonData, "placar_fora");

    jogos[index_dados].tem_dados = true;
    strcpy(jogos[index_dados].status, status->valuestring);
    strcpy(jogos[index_dados].time_casa, time_casa->valuestring);
    strcpy(jogos[index_dados].time_fora, time_fora->valuestring);
    strcpy(jogos[index_dados].placar_casa, placar_casa->valuestring);
    strcpy(jogos[index_dados].placar_fora, placar_fora->valuestring);

    cJSON_Delete(jsonData);

    ++total_jogos;
    if (index_dados == 8) {
      index_dados = 0;
      return;
    } else {
      index_dados++;
    }

    sprintf(buffer, "%d", index_dados);
    mqtt_publish(state->mqtt_client_inst, "/dados", buffer, strlen(buffer), 0,
                 100, NULL, NULL);
  }

  printf("Tópico: %s\n", state->topic);
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

    mqtt_sub_unsub(state->mqtt_client_inst, "/jogos", 1, NULL, state, 1);
    mqtt_sub_unsub(state->mqtt_client_inst, "/tempo_jogo", 1, NULL, state, 1);

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
