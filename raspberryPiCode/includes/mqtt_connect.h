#ifndef MQTT_CONNECT_H
#define MQTT_CONNECT_H

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
    mqtt_client_t* mqtt_client_inst;
    struct mqtt_connect_client_info_t mqtt_client_info;
    char topic[MQTT_TOPIC_LEN];
    char data[MQTT_TOPIC_LEN];
    ip_addr_t mqtt_server_address;
    bool connect_done;
} MQTT_CLIENT_DATA_T;

void mqtt_incoming_data_cb(void* arg, const u8_t* data, u16_t len,
    u8_t flags);

void mqtt_incoming_publish_cb(void* arg, const char* topic,
    u32_t tot_len);

void mqtt_connection_cb(mqtt_client_t* client, void* arg,
    mqtt_connection_status_t status);

void start_client(MQTT_CLIENT_DATA_T* state);

#endif
