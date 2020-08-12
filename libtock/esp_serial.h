#pragma once

#include "tock.h"

#define DRIVER_NUM_ESP_SERIAL 0x90009

#define CONNECTION_TYPE_COMMAND "AT+CIPMUX=1"
#define START_COMMAND "AT+CIPSTART="
#define CLOSE_COMMAND "AT+CIPCLOSE=5"
#define SEND_COMMAND "AT+CIPSEND="
#define WF_COMMAND "AT+CWJAP="
#define SET_WIFI_COMMAND "AT+CWMODE=1"
#define GET_IP_COMMAND "AT+CIFSR"

#define BIND_COMMAND_NUMBER 1
#define GENERAL_COMMAND_NUMBER 2
#define RECEIVE_COMMAND_NUMBER 3
#define ABORT_COMMAND_NUMBER 4
#define CLOSE_COMMAND_NUMBER 5
#define LINK_ID_COMMAND_NUMBER 6

#ifdef __cplusplus
extern "C" {
#endif

typedef void (user_callback)(char*, int);

uint8_t* get_tx_buffer (void);
uint8_t* get_rx_buffer (void);
uint8_t* get_user_buffer (void);

int subscribe_user_callback (user_callback* cb, void* ud);
// int connect_to_wifi(char* ssid, char* password, int link_id);
int get_esp_ip(void);
int bind (const char* ip_address, int port_dest, int* port_src, int* link_id);
int send_UDP_payload (size_t len, const char* str, int link_id);
int close_socket (int link_id);

int receive_command (int wait_yield);
int send_command (int command_num, int link_id);
int check_response (void);
int esp_command (int command_num, size_t data1, size_t data2);
int esp_allow (void* ptr, size_t buffer_type, size_t size);
int esp_subscribe (subscribe_cb cb, void* userdata, size_t cb_type);

#ifdef __cplusplus
}
#endif