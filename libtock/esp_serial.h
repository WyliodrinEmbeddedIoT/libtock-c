#pragma once

#include "tock.h"

#define DRIVER_NUM_ESP_SERIAL 0x90009

#define START_COMMAND "AT+CIPSTART=\"UDP\""
#define CLOSE_COMMAND "AT+CIPCLOSE=5"
#define SEND_COMMAND "AT+CIPSEND="

#ifdef __cplusplus
extern "C" {
#endif

uint8_t* get_tx_buffer (void);
// uint8_t* get_rx_buffer (void); 

int bind_socket (uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4, size_t port);
int send_UDP_payload (size_t len, char* str);
int close_socket (void);

int fake_receive (void);

#ifdef __cplusplus
}
#endif