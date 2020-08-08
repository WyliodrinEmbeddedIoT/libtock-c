#pragma once

#include "tock.h"

#define DRIVER_NUM_ESP_SERIAL 0x90009

#define START_COMMAND "AT+CIPSTART=\"UDP\""
#define CLOSE_COMMAND "AT+CIPCLOSE=5"
#define SEND_COMMAND "AT+CIPSEND="
#define WF_COMMAND "AT+CWJAP="
#define SET_WIFI_COMMAND "AT+CWMODE=1"
#define GET_IP_COMMAND "AT+CIFSR"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t* get_tx_buffer (void);
// uint8_t* get_rx_buffer (void); 

int connect_to_wifi(char* ssid, char* password);
int get_esp_ip();
int bind_socket (char* ip_address, size_t port);
int send_UDP_payload (size_t len, char* str);
int close_socket (void);

int fake_receive (void);

#ifdef __cplusplus
}
#endif